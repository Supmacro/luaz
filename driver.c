
#include <stdlib.h>
#include <stdio.h>

#include "dblist.h"
#include "option.h"

#include "io.h"
#include "driver.h"


extern void  opt_driver_name(const char *, char *, char *);

db_driver_t *db_loaded(const char *lname)
{
    char name[16] = {0}, s_name[32] = {0};

    opt_driver_name(lname, name, s_name);
    drv_t *pd = dl_find_db_drv(name, s_name);
    if(!pd)
    {
        PRT_ERROR("The given driver name does not exist"); 
        PRT_TAIL_CHR;
        exit(-1);
    }

    return &pd->drv;
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

    if(pd->ops.drv_connect(pd->dbconn)){
        pd->ops.drv_error(pd->dbconn);
    }

    pconn->pdb = (void *)pd;
    return pconn;
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

    pstmt->pdb = pconn->pdb;
    pstmt->sql = (char *)sql;

    db_driver_t *pd = (db_driver_t *)pstmt->pdb;


}


void db_stmt_destroy(db_stmt_t *pstmt)
{
    if(!pstmt->Isclose)
    {
        
    }

    free(pstmt);
}


void db_disconnect(db_conn_t *pconn)
{
    db_driver_t *pd = (db_driver_t *)pconn->pdb;

    pd->ops.drv_disconnect(pd->dbconn);
    pconn->Isdown = 1;
}


void db_conn_destroy(db_conn_t *pconn)
{
    if(!pconn->Isdown)
    {     
        db_driver_t *pd = (db_driver_t *)pconn->pdb;
        pd->ops.drv_disconnect(pd->dbconn);
    }

    free(pconn);
}


void db_destroy(db_driver_t *pd)
{
    if(!pd){
        PRT_ERROR("Invalid null pointer parameter");
        PRT_TAIL_CHR;
        exit(-1);
    }

    int j;
    for(j = 0; j < pd->vcap; j++)
    {
        db_value_t *pv = pd->dbval + j;
        
        if(pv->value)
            free(pv->value);
        pv->length = 0;
    }

    if(pd->dbconn){
        free(pd->dbconn); 
        pd->dbconn = null;
    }

    char name[16] = {0}, s_name[32] = {0};
    
    opt_driver_name(pd->lname, name, s_name);
    drv_t *drv = dl_find_db_drv(name, s_name);
    if(!drv)
    {
        PRT_ERROR("The given driver name does not exist"); 
        PRT_TAIL_CHR;
        exit(-1);
    }

    if(pd->lname){
        free(pd->lname);
        pd->lname = null;
    }

    DSR_LIST_DELETE(&drv->list);
    free(drv);
}


