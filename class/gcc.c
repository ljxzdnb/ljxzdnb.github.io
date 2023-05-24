#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <link.h>
#include <execinfo.h>
#include <stdio.h>
#include <string.h>

void main_constructor( void )
        __attribute__ ((no_instrument_function, constructor));

void main_destructor( void )
        __attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter( void *, void * )
        __attribute__ ((no_instrument_function));

void __cyg_profile_func_exit( void *, void * )
        __attribute__ ((no_instrument_function));

FILE* fp;
void print_trace(void* func_address, void* call_site)
{
    void* trace[128];
    int size = backtrace(trace, 128);
    char** symbols = backtrace_symbols(trace, size);
    if(call_site+8 != NULL){
	    int arg = *(int*)(call_site+8);
	    printf("arg:%d\n",arg);
    }
    char call_name[32];
    char called_name[32];
    for (int i = 0; i < size; i++) {
        char func_name[32];
        void * func_addr;
        unsigned int func_off;
        sscanf(symbols[i],"%*[^[][%p[^]]%*[^\n]",&func_addr);
        sscanf(symbols[i],"%*[^(](%[^+]+%x%*[^)]%*[^\n]",func_name,&func_off);
        func_name[strlen(func_name)] = 0;
        if (func_address+func_off == func_addr){
            memcpy(called_name,func_name,32);
        }
        else if(func_addr == call_site){
            memcpy(call_name,func_name,32);
        }
//        printf("get stash: %p %d %s\n",func_addr, func_off, func_name);
    }
    fprintf( fp, "  %s -> %s [fontsize=\"10\"]\n",call_name,called_name);
    free(symbols);
}

void main_constructor( void )
{
    fp = fopen("graph.dot", "w+");
    if (fp == NULL) exit(-1);
    fprintf(fp, "digraph %s {\n\n", "backtrace" );
}


void main_destructor( void )
{
    fprintf( fp, "\n}\n" );
    fclose( fp );
}

void __cyg_profile_func_enter(void* func_address, void* call_site)
{
	if(func_address != print_trace){
	print_trace(func_address, call_site);
	}
}
void __cyg_profile_func_exit( void *this, void *callsite )
{
}

