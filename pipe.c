
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "pipe.h"


void pipe_stdout(int n, ...)
{
    va_list   L;
    
    char error[2048] = {0};
    int j, length = 0;

    va_start(L, n);

    for(j = 0; j < n; j++)
    {
        char *rp = va_arg(L, char *);
        if(!rp)
        {
            PRT_ERROR("Unable to get next value in 'va_arg()'");
            PRT_TAIL_CHR;
            exit(-1);
        }

        length += strlen(rp);
        if(length >= 2048)
        {
            PRT_ERROR("");
            PRT_TAIL_CHR;
            exit(-1);
        }

        strcat(error, rp);
    }

    va_end(L);

    if(strlen(error))
        PRT_ERROR(error);
}


void  pipe_topath(char top[], char pgname[])
{
    char pid_link[256] = {0};
    int  length;

    sprintf(pid_link, "/proc/%d/exe", getpid());
    if((length = readlink(pid_link, top, 255)) < 0)
    {
        PRT_ERROR("Unable to get top-level path");
        PRT_TAIL_CHR;
        exit(1);
    }

    top[length] = '\0';
    char *p = rindex(top, '/');
    if(p)
    {
        p[0] = '\0';
        strcpy(pgname, p+1);
    }
}


