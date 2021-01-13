
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sl_list.h"
#include "sl_option.h"

#include "sl_pipe.h"


extern void  opt_driver_name(const char *, char *, char *);

db_driver_t *db_loaded(const char *lname)
{
    char name[16] = {0}, s_name[32] = {0};

    opt_driver_name(lname, name, s_name);
    drv_t *p = dl_find_db_drv(name, s_name);
    if(!p)
    {
        IO_print_strcat(6, "Cannot find the driver named ',", name, ".", s_name, 
           "'. Please carefully confirm the parameter information of 'sql.driver()'. ", 
         "This parameter needs to be configured in the configuration file'configure.lua' ", 
            "before it can be used legally.");
        PRT_TAIL_CHR;
        exit(-1);
    }

    return &p->drv;
}


void db_destroy(db_driver_t *pd)
{
    if(!pd){
        IO_print_strcat(2, "GC received a null pointer, the pointer type is'db_driver_t *',",
                       " and the calling function is db_destroy");
        PRT_TAIL_CHR;
        exit(-1);
    }

    /* -- No need to do anything -- */
}


db_conn_t *db_connect(db_driver_t *pd)
{
    db_conn_t *pconn = (db_conn_t *)calloc(1, sizeof(db_conn_t));
    if(!pconn)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    pconn->concap = pd->dbsize;
    pconn->connect = calloc(1, pconn->concap);
    if(!pconn->connect)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    memcpy(pconn->connect, pd->dbconn, pd->dbsize);
    if(pd->ops.drv_connect(pconn->connect)){
        pd->ops.drv_error(pconn->connect);
    }

    pconn->pdb = (void *)pd;
    return pconn;
}


void db_disconnect(db_conn_t *pconn)
{
    db_driver_t *pd = (db_driver_t *)pconn->pdb;

    if(pd->ops.drv_disconnect(pconn->connect)){
         pd->ops.drv_error(pconn->connect);
    }

    pconn->Isdown = 1;
}


void db_conn_destroy(db_conn_t *pconn)
{
    if(!pconn->Isdown)
    {     
        IO_print_strcat(2, "Before GC, there are unclosed connections, ",
                     "please check the execution script information carefully");
        PRT_TAIL_CHR;
        exit(-1);
    }

    if(pconn->connect)
        free(pconn->connect);

    free(pconn);
}


db_stmt_t *db_prepare(db_conn_t *pconn, const char *sql)
{
    db_stmt_t *pstmt = (db_stmt_t *)calloc(1, sizeof(db_stmt_t));
    if(!pstmt)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    pstmt->concap = pconn->concap;
    pstmt->pdb = pconn->pdb;
   
    if(sql){
        pstmt->sql = strdup(sql);
        if(!pstmt->sql)
        {
            PRT_ERROR("Not enough free memory");
            PRT_TAIL_CHR;
            exit(-1);
        }
    }else
        pstmt->sql = NULL;
    
    pstmt->connect = calloc(1, pstmt->concap);
    if(!pstmt->connect)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }
   
    memcpy(pstmt->connect, pconn->connect, pconn->concap);
    db_driver_t *pd = (db_driver_t *)pstmt->pdb;
    
    if(pd->ops.drv_prepare(pstmt->connect, pstmt->sql)){
        pd->ops.drv_error(pstmt->connect);
    }

    pstmt->Ispre = 1;
    pstmt->Isclose = 0;

    return pstmt;
}


void db_bind_parameter(db_stmt_t *pstmt, db_param_t *params, int pcap)
{
    db_driver_t *pd = (db_driver_t *)pstmt->pdb;

    int j;

    for(j = 0; j < pcap; j++)
    {
        if(pd->ops.drv_bindparam(pstmt->connect, params+j, j+1)){
            pd->ops.drv_error(pstmt->connect);
        }
    }
}


void db_execute(db_stmt_t *pstmt)
{
    db_driver_t *pd = (db_driver_t *)pstmt->pdb;
    
    if(pd->ops.drv_execute(pstmt->connect)){
        pd->ops.drv_error(pstmt->connect);
    }
}


void db_stmt_close(db_stmt_t *pstmt)
{
    db_driver_t *pd = (db_driver_t *)pstmt->pdb;

    if(pd->ops.drv_freestmt(pstmt->connect)){
        pd->ops.drv_error(pstmt->connect);
    }

    pstmt->Ispre = 0;
    pstmt->Isclose = 1;
}


void db_stmt_destroy(db_stmt_t *pstmt)
{
    if(!pstmt->Isclose)
    {
        IO_print_strcat(2, "Before GC, there are unreleased statement handles, ",
                        "please check the execution script information carefully");
        PRT_TAIL_CHR;
        exit(-1);
    }

    if(pstmt->sql){
        free(pstmt->sql);
        pstmt->sql = NULL;
    }

    if(pstmt->connect)
        free(pstmt->connect);

    free(pstmt);
}





