
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "io.h"

void IO_print_strcat(int cap, ...)
{
    va_list   L;
    
    va_start(L, cap);

    char *p = NULL;
    int j;

    for(j = 0; j < cap; j++)
    {
        char *tmp = va_arg(L, char *);
        if(!tmp){
            PRT_ERROR("Unable to get next value in 'va_arg()'");
            PRT_TAIL_CHR;
            exit(-1);
        }

        if(!p){
            p = (char *)calloc(1, strlen(tmp) + 2);
            if(!p){
                PRT_ERROR("Not enough free memory");
                PRT_TAIL_CHR;
                exit(-1);
            }

            strcpy(p, tmp);
            continue;
        }

        char *rp = (char *)calloc(1, strlen(tmp) + strlen(p) + 1);
        if(!rp){
            PRT_ERROR("Not enough free memory");
            PRT_TAIL_CHR;
            exit(-1);
        }

        strcpy(rp, p);
        strcat(rp, tmp);

        free(p);
        p = rp;
    }

    va_end(L);
    if(p)
        PRT_ERROR(p);

    free(p);
}


