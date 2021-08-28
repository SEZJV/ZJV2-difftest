#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "qemu.h"
#include "reg.h"

/* only for debug, print the packets */
#if 0
#define CAT(a, b) CAT_IMPL(a, b)
#define CAT_IMPL(a, b) a##b

#define gdb_send(conn, buf, size)       \
  ({                                    \
    printf("send:'%s'\n", (char *)buf); \
    gdb_send(conn, buf, size);          \
  })

#define gdb_recv(conn, size)                               \
  ({                                                       \
    char *CAT(s, __LINE__) = (void *)gdb_recv(conn, size); \
    printf("recv:'%s'\n", (char *)CAT(s, __LINE__));       \
    (uint8_t *)CAT(s, __LINE__);                           \
  })
#endif

int qemu_start(const char *elf, int port) {
    char remote_s[100];
    const char *exec = "qemu-system-riscv64";
    snprintf(remote_s, sizeof(remote_s), "tcp::%d", port);

    execlp(exec, exec, "-S", "-gdb", remote_s, "-kernel", elf, "-M", "virt", "-m", "64M", "-nographic", NULL);
    return -1;
}

qemu_conn_t *qemu_connect(int port) {
    qemu_conn_t *conn = NULL;
    while (
            (conn = gdb_begin_inet("127.0.0.1", port)) == NULL) {
        usleep(1);
    }

    return conn;
}

bool qemu_memcpy_to_qemu_small(
        qemu_conn_t *conn, uint32_t dest, void *src, int len) {
    char *buf = (char *) malloc(len * 2 + 128);
    assert(buf != NULL);
    int p = sprintf(buf, "M0x%x,%x:", dest, len);
    int i;
    for (i = 0; i < len; i++) {
        p += sprintf(buf + p, "%c%c",
                     hex_encode(((uint8_t *) src)[i] >> 4),
                     hex_encode(((uint8_t *) src)[i] & 0xf));
    }

    gdb_send(conn, (const uint8_t *) buf, strlen(buf));
    free(buf);

    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    bool ok = !strcmp((const char *) reply, "OK");
    free(reply);

    return ok;
}

bool qemu_memcpy_to_qemu(
        qemu_conn_t *conn, uint32_t dest, void *src, int len) {
    const int mtu = 1500;
    bool ok = true;
    while (len > mtu) {
        ok &= qemu_memcpy_to_qemu_small(conn, dest, src, mtu);
        dest += mtu;
        src += mtu;
        len -= mtu;
    }
    ok &= qemu_memcpy_to_qemu_small(conn, dest, src, len);
    return ok;
}

bool qemu_getregs(qemu_conn_t *conn, qemu_regs_t *r) {
    gdb_send(conn, (const uint8_t *) "g", 1);
    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);

    int i;
    uint8_t *p = reply;
    uint8_t c;
    for (i = 0; i < sizeof(qemu_regs_t) / sizeof(uint32_t);
         i++) {
        c = p[8];
        p[8] = '\0';
        r->array[i] = gdb_decode_hex_str(p);
        p[8] = c;
        p += 8;
    }

    free(reply);

    return true;
}

bool qemu_setregs(qemu_conn_t *conn, qemu_regs_t *r) {
    int len = sizeof(qemu_regs_t);
    char *buf = (char *) malloc(len * 2 + 128);
    assert(buf != NULL);
    buf[0] = 'G';

    void *src = r;
    int p = 1;
    int i;
    for (i = 0; i < len; i++) {
        p += sprintf(buf + p, "%c%c",
                     hex_encode(((uint8_t *) src)[i] >> 4),
                     hex_encode(((uint8_t *) src)[i] & 0xf));
    }

    gdb_send(conn, (const uint8_t *) buf, strlen(buf));
    free(buf);

    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    bool ok = !strcmp((const char *) reply, "OK");
    free(reply);

    return ok;
}

bool qemu_single_step(qemu_conn_t *conn) {
    char buf[] = "vCont;s:1";
    gdb_send(conn, (const uint8_t *) buf, strlen(buf));
    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    free(reply);
    return true;
}

void qemu_break(qemu_conn_t *conn, uint32_t entry) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Z0,%08x,4", entry);
    gdb_send(conn, (const uint8_t *) buf, strlen(buf));

    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    free(reply);
}

void qemu_remove_breakpoint(
        qemu_conn_t *conn, uint32_t entry) {
    char buf[32];
    snprintf(buf, sizeof(buf), "z0,%08x,4", entry);
    gdb_send(conn, (const uint8_t *) buf, strlen(buf));

    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    free(reply);
}

void qemu_continue(qemu_conn_t *conn) {
    char buf[] = "vCont;c:1";
    gdb_send(conn, (const uint8_t *) buf, strlen(buf));
    size_t size;
    uint8_t *reply = gdb_recv(conn, &size);
    free(reply);
}

void qemu_disconnect(qemu_conn_t *conn) {
    gdb_end(conn);
}
