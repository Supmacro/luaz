
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "dblist.h"
#include "option.h"

#include "./xgci/xgsql.h"

db_value_t *opt_login_argv(KV opt, db_value_t *pval)
{
    if(!pval)
        return NULL;

    switch(opt.type)
    {
        case OPT_STRING:
            pval->length = strlen(opt.value);
            pval->value = (void *)strdup(opt.value);
            break;
        case OPT_INT:

            pval->length = sizeof(int);
            pval->value = calloc(1, pval->length);
            
            if(opt.item == charset)
            {
                char up[256] = {0};

                strcpy(up, opt.value);
                opt_upper(up);

                if(!strcmp(up, "GBK"))
                {
                    *((int *)pval->value) = GBK;
                }else if(!strcmp(up, "GB2312"))
                {
                    *((int *)pval->value) = GB2312;
                }else {
                    *((int *)pval->value) = UTF8;
                }

                break;
            }
    
           *((int*)pval->value) = atoi(opt.value);
            break;
        case OPT_BOOLEAN:
        case OPT_FLOAT:
        case OPT_NULL:
            pval->length = 0;
            pval->value = NULL;
            break;
    }

    return pval;
}


char *opt_upper(char *str)
{
    int j = 0;

    for(;str[j] != '\0'; j++)
    {
        if(str[j] >= 'a' && str[j] <= 'z' )
            str[j] = str[j] - 32;
        
    }

    return str;
}


void opt_driver_name(const char *lname, char *d, char *sd) 
{
    int j, len = strlen(lname);
    char *s;

    for(j = 0, s = d; j < len; j++){
        
        if(lname[j] == '.'){
            *s = '\0';
            s = sd;
            continue;
        }

        *s++ = lname[j];
    }
}


