cmake_minimum_required(VERSION 3.25)
project(class C)

set(CMAKE_C_STANDARD 99)

add_executable(class main.c heap/heap.c tpool/tpool.c mpool/mpool.c sock/sock.c react/react.c table/table.c ring/ring.c)

target_compile_options(class PRIVATE -D__EVSIZE__=50)