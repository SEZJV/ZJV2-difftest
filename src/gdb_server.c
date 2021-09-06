#include <arpa/inet.h>
#include <malloc.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "monitor.h"
#include "gdb_proto.h"
#include "reg.h"

jmp_buf gdb_mode_top_caller;


void gdb_server_mainloop(int servfd) {
  struct gdb_conn *conn = gdb_begin_server(servfd);
  while (1) {
    size_t size = 0;
    char *data = (void *)gdb_recv(conn, &size);

    char *resp = "OK";
    if (resp) {
      gdb_send(conn, (void *)resp, strlen(resp));
    } else {
      gdb_send(conn, (void *)"", 0);
    }
    free(data);
  }
  free(conn);
}
