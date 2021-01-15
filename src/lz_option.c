
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "lz_option.h"
#include "lz_xgsql.h"


/* 
 * opt_login_argv() is used to parse the content of the parameter item 
 * given by the user and process it according to the data type of the 
 * parameter key value information. 
 * Each supported and valid key value information will be saved globally.
 */
db_value_t *opt_login_argv(KV opt, db_value_t *pval)
{
    if(!pval)
        return NULL;

    switch(opt.type)
    {
        case OPT_STRING:
            pval->length = strlen(opt.value);
            pval->value = (void *)strdup(opt.value);
            if(!pval->value){
                PRT_ERROR("Not enough free memory");
                PRT_TAIL_CHR;
                exit(-1);
            }

            break;
        case OPT_INT:

            pval->length = sizeof(int);
            pval->value = calloc(1, pval->length);
            if(!pval->value){
                PRT_ERROR("Not enough free memory");
                PRT_TAIL_CHR;
                exit(-1);
            }

/* For the character set in the connection option, currently 
 * supported character sets are GBK, GB2312, UTF8
 * */
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

    *s = '\0';
}


