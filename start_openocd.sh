#!/bin/bash
openocd -f board/stm32f7discovery.cfg -c gdb_port 3333 -c telnet_port 4444 -c tcl_port 6666 -c "tpiu config internal itm.fifo uart off 168000000" -c "itm port 1 1"

