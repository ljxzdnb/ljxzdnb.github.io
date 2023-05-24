#!/bin/bash
gcc json.c ../tpool/tpool.c ../heap/heap.c ../mpool/mpool.c ../table/table.c ../sock/sock.c ../react/react.c ../ring/ring.c -Wno-unknown-pragmas -D__DEBUG__ -D__EVSIZE__=50 
