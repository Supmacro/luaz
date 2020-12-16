
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "xgsql.h"


static xugu_conn_t  xugu;

/* The xugusql_connect() function is used to establish 
 * a new connection with the specified XUGU database server
 */
int xugusql_connect()
{
    ATRBT rp[] = {
                {&xugu.hddbc, XGCI_ATTR_SESS_CHARSET, charset},
                {&xugu.hdsvr, XGCI_ATTR_SRV_IP, host_ip},
                {&xugu.hdsvr, XGCI_ATTR_SRV_PORT, host_port},
                {&xugu.hdsvr, XGCI_ATTR_SRV_DBNAME, dbname},
    };
   
    int rc = XGCIHandleAlloc(NULL, &xugu.hdenv, HT_ENV);
    if(rc < 0){
        xugu.ebody.hd = NULL;
        return -1; 
    }

    rc = XGCIHandleAlloc(xugu.hdenv, &xugu.hdsvr, HT_SERVER); 
    if(rc < 0){
        xugu.ebody.hd = xugu.hdenv;
        return -1;
    }

    rc = XGCIHandleAlloc(xugu.hdsvr, &xugu.hddbc, HT_SESSION);
    if(rc < 0){
        xugu.ebody.hd = xugu.hdsvr;
        return -1;
    }

    int j, sz = sizeof(rp)/sizeof(ATRBT);
    for(j = 0; j < sz; j++)
    {
        db_value_t db = xugu.plg[rp[j].opt];
        XGCIHANDLE hd = *(rp[j].hd);
        XGCIHandleAttrSet(hd, rp[j].type, db.value, XGCI_NTS);
    }

    rc = XGCISessionBegin(xugu.hddbc, xugu.plg[user_name].value, 
                    xugu.plg[user_passwd].value);
    if(rc < 0){
        xugu.ebody.hd = xugu.hddbc;
        return -1;
    }

    return 0; 
}


/* recycle connection resources*/
void xugusql_disconnect()
{
    int rc = XGCISessionEnd(xugu.hddbc);
    rc = XGCIHandleFree(xugu.hddbc);
    rc = XGCIHandleFree(xugu.hdsvr);
    rc = XGCIHandleFree(xugu.hdenv);
}


/* In the structured programming process, when the previous layer of 
 * function call fails, the function xugusql_error() is used to obtain 
 * relevant error diagnosis information */
void xugusql_error()
{
    struct xgci_err *pe = &xugu.ebody;
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


void xugusql_register_plg(db_value_t **pval)
{
    *pval = xugu.plg;
}

