#!/bin/bash
gcc main.c tpool/tpool.c heap/heap.c mpool/mpool.c table/table.c sock/sock.c react/react.c ring/ring.c -D__DEBUG__ -D__EVSIZE__=50 -g
