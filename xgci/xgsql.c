
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "../option.h"
#include "../dblist.h"
#include "xgsql.h"


/* The sql_connect() function is used to establish 
 * a new connection with the specified XUGU database server
 */
int sql_connect(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;

    ATRBT rp[] = {
                {&conp->hddbc, XGCI_ATTR_SESS_CHARSET, charset},
                {&conp->hdsvr, XGCI_ATTR_SRV_IP, host_ip},
                {&conp->hdsvr, XGCI_ATTR_SRV_PORT, host_port},
                {&conp->hdsvr, XGCI_ATTR_SRV_DBNAME, dbname},
    };

    int rc = XGCIHandleAlloc(NULL, &conp->hdenv, HT_ENV);
    if(rc < 0){
        pe->hd = NULL;
        return rc; 
    }

    rc = XGCIHandleAlloc(conp->hdenv, &conp->hdsvr, HT_SERVER); 
    if(rc < 0){
        pe->hd = conp->hdenv;
        return rc;
    }

    rc = XGCIHandleAlloc(conp->hdsvr, &conp->hddbc, HT_SESSION);
    if(rc < 0){
        pe->hd = conp->hdsvr;
        return rc;
    }

    int j, sz = sizeof(rp)/sizeof(ATRBT);
    for(j = 0; j < sz; j++)
    {
        db_value_t db = conp->plg[rp[j].opt];
        XGCIHANDLE hd = *(rp[j].hd);
        XGCIHandleAttrSet(hd, rp[j].type, db.value, XGCI_NTS);
    }

    rc = XGCISessionBegin(conp->hddbc, conp->plg[user_name].value, 
                    conp->plg[user_passwd].value);
    if(rc < 0){
        pe->hd = conp->hddbc;
        return rc;
    }

    return rc; 
}


int sql_prepare(void *voip, char *sql)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = XGCIHandleAlloc(conp->hddbc, &conp->hdstmt, HT_STATEMENT);
    if(rc < 0){
        pe->hd = conp->hddbc;
        return rc;
    }

    rc = XGCIPrepare(conp->hdstmt, sql, XGCI_NTS);
    if(rc < 0){
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc;
}


int sql_execute(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = XGCIExecute(conp->hdstmt);
    if(rc < 0){
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc;
}


/* SQLHandleFree is used to release the statement handle */
int sql_freehandle(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = XGCIHandleFree(conp->hdstmt);
    if(rc < 0){
        pe->hd = conp->hdstmt;
        return rc;
    }

    conp->hdstmt = NULL;
    return rc;
}


/* recycle connection resources*/
int sql_disconnect(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;
    
    int rc = XGCISessionEnd(conp->hddbc);
    if(rc < 0){
        pe->hd = conp->hddbc;
        return rc;
    }

    rc = XGCIHandleFree(conp->hddbc);
    rc = XGCIHandleFree(conp->hdsvr);
    rc = XGCIHandleFree(conp->hdenv);

    return rc;
}


/* In the structured programming process, when the previous layer of 
 * function call fails, the function sql_error() is used to obtain 
 * relevant error diagnosis information */
void sql_error(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    struct error *pe = &conp->ebody;
    
    if(pe->hd == NULL)
    {
        PRT_ERROR("Unknown exception");
        PRT_TAIL_CHR;
        exit(-1);
    }

    XGCIErrors(pe->hd, pe->errcode, pe->errmsg, &pe->length);
    PRT_ERROR(pe->errmsg);
    PRT_TAIL_CHR;
    exit(-1);
}


void sql_register_plg(void *voip)
{
    db_driver_t *db = (db_driver_t *)voip;

    db->dbsize = sizeof(xugu_conn_t);
    db->dbconn = calloc(1, db->dbsize);
    if(!db->dbconn)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    db->dbval = ((xugu_conn_t *)db->dbconn)->plg;
}

