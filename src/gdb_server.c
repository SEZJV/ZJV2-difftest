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

static char target_xml[] =
    "l<?xml version=\"1.0\"?>"
    "<!DOCTYPE target SYSTEM \"gdb-target.dtd\">"
    "<target>"
    "<architecture>mips</architecture>"
    "<xi:include href=\"mips-32bit.xml\"/>"
    "</target>";

static char mips_32bit_xml[] =
    "l<?xml version=\"1.0\"?>\n"
    "<!-- Copyright (C) 2010-2017 Free Software "
    "Foundation, Inc.\n"
    "\n"
    "     Copying and distribution of this file, with or "
    "without modification,\n"
    "     are permitted in any medium without royalty "
    "provided the copyright\n"
    "     notice and this notice are preserved.  -->\n"
    "\n"
    "<!-- MIPS32 with CP0 -->\n"
    "\n"
    "<!DOCTYPE target SYSTEM \"gdb-target.dtd\">\n"
    "<feature name=\"org.gnu.gdb.mips.32bit\">\n"
    "  <xi:include href=\"mips-32bit-cpu.xml\"/>\n"
    "  <xi:include href=\"mips-32bit-cp0.xml\"/>\n"
    "</feature>";

static char mips_32bit_cpu_xml[] =
    "l<?xml version=\"1.0\"?>\n"
    "<!-- Copyright (C) 2010-2015 Free Software "
    "Foundation, Inc.\n"
    "\n"
    "     Copying and distribution of this file, with or "
    "without modification,\n"
    "     are permitted in any medium without royalty "
    "provided the copyright\n"
    "     notice and this notice are preserved.  -->\n"
    "\n"
    "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
    "<feature name=\"org.gnu.gdb.mips.cpu\">\n"
    "  <reg name=\"zero\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"at\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"v0\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"v1\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"a0\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"a1\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"a2\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"a3\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t0\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t1\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t2\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t3\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t4\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t5\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t6\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t7\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s0\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s1\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s2\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s3\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s4\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s5\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s6\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"s7\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t8\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"t9\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"k0\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"k1\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"gp\" bitsize=\"32\" "
    "type=\"data_ptr\"/>\n"
    "  <reg name=\"sp\" bitsize=\"32\" "
    "type=\"data_ptr\"/>\n"
    "  <reg name=\"fp\" bitsize=\"32\" "
    "type=\"data_ptr\"/>\n"
    "  <reg name=\"ra\" bitsize=\"32\" type=\"int32\"/>\n"
    "</feature>\n";

static char mips_32bit_cp0_xml[] =
    "l<?xml version=\"1.0\"?>\n"
    "<!-- Copyright (C) 2010-2015 Free Software "
    "Foundation, Inc.\n"
    "\n"
    "     Copying and distribution of this file, with or "
    "without modification,\n"
    "     are permitted in any medium without royalty "
    "provided the copyright\n"
    "     notice and this notice are preserved.  -->\n"
    "\n"
    "<!DOCTYPE feature SYSTEM \"gdb-target.dtd\">\n"
    "<feature name=\"org.gnu.gdb.mips.cp0\">\n"
    "  <reg name=\"sr\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"lo\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"hi\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"bad\" bitsize=\"32\" type=\"int32\"/>\n"
    "  <reg name=\"cause\" bitsize=\"32\" "
    "type=\"int32\"/>\n"
    "  <reg name=\"pc\" bitsize=\"32\" "
    "type=\"code_ptr\"/>\n"
    "  <reg name=\"epc\" bitsize=\"32\" "
    "type=\"code_ptr\"/>\n"
    "</feature>\n";

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
