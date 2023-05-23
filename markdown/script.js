angular.module('app', ['hc.marked'])

angular.module('app').config(function(markedProvider) {
    markedProvider.setOptions({
        gfm: true,
        tables: true,
        highlight: function(code) {
            return hljs.highlightAuto(code).value;
        }
    });
})

angular.module('app').controller('ctrl', function($scope) {
    $scope.my_markdown = '## Code Visualizer\n' +
        '\n' +
        'a code visualizer tool write by me,the basic principle is the gcc compilation option.\n' +
        '\n' +
        '```c\n' +
        'void main_constructor( void )\n' +
        '        __attribute__ ((no_instrument_function, constructor));\n' +
        '\n' +
        'void main_destructor( void )\n' +
        '        __attribute__ ((no_instrument_function, destructor));\n' +
        '\n' +
        'void __cyg_profile_func_enter( void *, void * )\n' +
        '        __attribute__ ((no_instrument_function));\n' +
        '\n' +
        'void __cyg_profile_func_exit( void *, void * )\n' +
        '        __attribute__ ((no_instrument_function));\n' +
        '```\n' +
        '\n' +
        'these four functions will be called before and after executing the function, as long as the processing logic is written,code detection can be realized.\n' +
        '\n' +
        '```c\n' +
        '#include <execinfo.h>\n' +
        '```\n' +
        '\n' +
        'the header file contains a function that can print the function call stack,function pointer and names can be obtained by string matching\n' +
        '\n' +
        '```c\n' +
        'void main_constructor( void )\n' +
        '{\n' +
        '    pid = getpid();\n' +
        '    fp = fopen("graph.dot", "w+");\n' +
        '    if (fp == NULL) exit(-1);\n' +
        '    //setenv("MALLOC_TRACE","graph.dot",1);\n' +
        '    fprintf(fp, "digraph %s {\\n\\n", "backtrace" );\n' +
        '    //fprintf(fp, "rankdir=LR;\\n" );\n' +
        '    fprintf(fp, "node [shape=box, style=rounded ]\\n" );\n' +
        '    ptrace(PTRACE_ATTACH,pid,NULL,NULL);\n' +
        '\n' +
        '}\n' +
        '```\n' +
        '\n' +
        'open a file to write a scripts in .dot language before entering the main function\n' +
        '\n' +
        '```c\n' +
        'void __cyg_profile_func_enter(void* func_address, void* call_site)\n' +
        '{\n' +
        '        mtrace();\n' +
        '        ptrace(PTRACE_GETREGSET,pid,NULL,NULL);\n' +
        '        //void* arg = (void*)ptrace(PTRACE_PEEKTEXT,getpid(),regs.pc,NULL);\n' +
        '        long int arg = ptrace(PTRACE_PEEKDATA,pid,(void*)regs.sp,NULL);\n' +
        '        if((void*)arg!=NULL)printf("arg %ld\\n",arg);\n' +
        '        start = clock();\n' +
        '\n' +
        '}\n' +
        '```\n' +
        '\n' +
        'log enter time and memory status of each function\n' +
        '\n' +
        '```c\n' +
        'void __cyg_profile_func_exit( void *func_address, void *call_site )\n' +
        '{\n' +
        '        end = clock();\n' +
        '        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;\n' +
        '        void* trace[128];\n' +
        '        int size = backtrace(trace, 128);\n' +
        '        char** symbols = backtrace_symbols(trace, size);\n' +
        '        char call_name[32];\n' +
        '        char called_name[32];\n' +
        '        for (int i = 0; i < size; i++) {\n' +
        '        char func_name[32];\n' +
        '        void * func_addr;\n' +
        '        unsigned int func_off;\n' +
        '        sscanf(symbols[i],"%*[^[][%p[^]]%*[^\\n]",&func_addr);\n' +
        '        sscanf(symbols[i],"%*[^(](%[^+]+%x%*[^)]%*[^\\n]",func_name,&func_off);\n' +
        '        func_name[strlen(func_name)] = 0;\n' +
        '        if (func_address+func_off == func_addr){\n' +
        '            memcpy(called_name,func_name,32);\n' +
        '        }\n' +
        '        else if(func_addr == call_site){\n' +
        '            memcpy(call_name,func_name,32);\n' +
        '        }\n' +
        '        //printf("get stash: %d %s\\n",i,symbols[i]);\n' +
        '        }\n' +
        '        fprintf( fp, "  %s [shape = record,label=\\"{func name:%s|mtrace log:|",called_name,called_name);\n' +
        '        fprintf( fp, "  }\\"]\\n");\n' +
        '        fprintf( fp, "  %s -> %s ",call_name,called_name);\n' +
        '        free(symbols);\n' +
        '        muntrace();\n' +
        '        fprintf( fp, " [label=\\"time_used:%dus\\" fontsize=\\"10\\"]\\n",(int)(1000000*cpu_time_used));\n' +
        '}\n' +
        '```\n' +
        '\n' +
        'log exit time and memory status of each function\n' +
        '\n' +
        'code under test:\n' +
        '\n' +
        'main.c\n' +
        '\n' +
        '```c\n' +
        '#include <stdio.h>\n' +
        '#include <stdlib.h>\n' +
        '\n' +
        'void bar(int arg)\n' +
        '{\n' +
        '    printf("Inside bar()%d\\n",arg);\n' +
        '    foo();\n' +
        '}\n' +
        '\n' +
        'int main()\n' +
        '{\n' +
        '    char* name = malloc(20);\n' +
        '    int num = 8;\n' +
        '    bar(num);\n' +
        '    foo();\n' +
        '    return 0;\n' +
        '}\n' +
        '\n' +
        '```\n' +
        '\n' +
        'other.c\n' +
        '\n' +
        '```c\n' +
        '#include <stdio.h>\n' +
        'void foo()\n' +
        '{\n' +
        '    printf("Inside foo()\\n");\n' +
        '}\n' +
        '\n' +
        '```\n' +
        '\n' +
        'make.sh\n' +
        '\n' +
        '```sh\n' +
        'gcc -g -rdynamic -finstrument-functions -fsanitize=address main.c gcc.c other.c -o main\n' +
        './main\n' +
        'dot -Tpng graph.dot -o graph.png\n' +
        '```\n' +
        '\n' +
        'the effect is as follows:\n' +
        '\n' +
        '![../graph.png](../graph.png)\n' +
        '\n' +
        'all codes:\n' +
        '\n' +
        '```c\n' +
        '#define _GNU_SOURCE\n' +
        '#include <stdio.h>\n' +
        '#include <stdlib.h>\n' +
        '#include <dlfcn.h>\n' +
        '#include <link.h>\n' +
        '#include <execinfo.h>\n' +
        '#include <stdio.h>\n' +
        '#include <string.h>\n' +
        '#include <time.h>\n' +
        '#include <unistd.h>\n' +
        '#include <mcheck.h>\n' +
        '#include <sys/ptrace.h>\n' +
        '#include <sys/user.h>\n' +
        '\n' +
        'void main_constructor( void )\n' +
        '        __attribute__ ((no_instrument_function, constructor));\n' +
        '\n' +
        'void main_destructor( void )\n' +
        '        __attribute__ ((no_instrument_function, destructor));\n' +
        '\n' +
        'void __cyg_profile_func_enter( void *, void * )\n' +
        '        __attribute__ ((no_instrument_function));\n' +
        '\n' +
        'void __cyg_profile_func_exit( void *, void * )\n' +
        '        __attribute__ ((no_instrument_function));\n' +
        '\n' +
        'FILE* fp;\n' +
        'pid_t pid;\n' +
        'clock_t start,end;\n' +
        'double cpu_time_used;\n' +
        'struct user_regs_struct regs;\n' +
        '\n' +
        'void main_constructor( void )\n' +
        '{\n' +
        '    pid = getpid();\n' +
        '    fp = fopen("graph.dot", "w+");\n' +
        '    if (fp == NULL) exit(-1);\n' +
        '    //setenv("MALLOC_TRACE","graph.dot",1);\n' +
        '    fprintf(fp, "digraph %s {\\n\\n", "backtrace" );\n' +
        '    //fprintf(fp, "rankdir=LR;\\n" );\n' +
        '    fprintf(fp, "node [shape=box, style=rounded ]\\n" );\n' +
        '    ptrace(PTRACE_ATTACH,pid,NULL,NULL);\n' +
        '    \n' +
        '}\n' +
        '\n' +
        '\n' +
        'void main_destructor( void )\n' +
        '{\n' +
        '    ptrace(PTRACE_DETACH,pid,NULL,NULL);\n' +
        '    fprintf( fp, "\\n}\\n" );\n' +
        '    fclose( fp );\n' +
        '}\n' +
        '\n' +
        'void __cyg_profile_func_enter(void* func_address, void* call_site)\n' +
        '{\n' +
        '\tmtrace();\n' +
        '\tptrace(PTRACE_GETREGSET,pid,NULL,NULL);\n' +
        '\t//void* arg = (void*)ptrace(PTRACE_PEEKTEXT,getpid(),regs.pc,NULL);\n' +
        '\tlong int arg = ptrace(PTRACE_PEEKDATA,pid,(void*)regs.sp,NULL);\n' +
        '    \tif((void*)arg!=NULL)printf("arg %ld\\n",arg);\n' +
        '\tstart = clock();\n' +
        '\n' +
        '}\n' +
        'void __cyg_profile_func_exit( void *func_address, void *call_site )\n' +
        '{\n' +
        '\tend = clock();\n' +
        '\tcpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;\n' +
        '\tvoid* trace[128];\n' +
        '    \tint size = backtrace(trace, 128);\n' +
        '    \tchar** symbols = backtrace_symbols(trace, size);\n' +
        '    \tchar call_name[32];\n' +
        '    \tchar called_name[32];\n' +
        '    \tfor (int i = 0; i < size; i++) {\n' +
        '        char func_name[32];\n' +
        '        void * func_addr;\n' +
        '        unsigned int func_off;\n' +
        '        sscanf(symbols[i],"%*[^[][%p[^]]%*[^\\n]",&func_addr);\n' +
        '        sscanf(symbols[i],"%*[^(](%[^+]+%x%*[^)]%*[^\\n]",func_name,&func_off);\n' +
        '        func_name[strlen(func_name)] = 0;\n' +
        '        if (func_address+func_off == func_addr){\n' +
        '            memcpy(called_name,func_name,32);\n' +
        '        }\n' +
        '        else if(func_addr == call_site){\n' +
        '            memcpy(call_name,func_name,32);\n' +
        '        }\n' +
        '        //printf("get stash: %d %s\\n",i,symbols[i]);\n' +
        '    \t}\n' +
        '\tfprintf( fp, "  %s [shape = record,label=\\"{func name:%s|mtrace log:|",called_name,called_name);\n' +
        '\tfprintf( fp, "  }\\"]\\n");\n' +
        '\tfprintf( fp, "  %s -> %s ",call_name,called_name);\n' +
        '\tfree(symbols);\n' +
        '\tmuntrace();\n' +
        '        fprintf( fp, " [label=\\"time_used:%dus\\" fontsize=\\"10\\"]\\n",(int)(1000000*cpu_time_used));\n' +
        '}\n' +
        '```\n' +
        '\n'
    //     Tab Support for Text-area
    function enableTab(id) {
        var el = document.getElementById(id);
        el.onkeydown = function(e) {
            if (e.keyCode === 9) {
                var val = this.value,
                    start = this.selectionStart,
                    end = this.selectionEnd;
                this.value = val.substring(0, start) + '\t' + val.substring(end);

                this.selectionStart = this.selectionEnd = start + 1;

                return false;

            }
        };
    };
    enableTab('txt')
})
