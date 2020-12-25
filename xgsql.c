
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "rows.h"
#include "dlist.h"
#include "xgsql.h"


/* The xugusql_connect() function is used to establish 
 * a new connection with the specified XUGU database server
 */
int xugusql_connect(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;

    ATRBT rp[] = {
                {&conp->hddbc, XGCI_ATTR_SESS_CHARSET, charset},
                {&conp->hdsvr, XGCI_ATTR_SRV_IP, host_ip},
                {&conp->hdsvr, XGCI_ATTR_SRV_PORT, host_port},
                {&conp->hdsvr, XGCI_ATTR_SRV_DBNAME, dbname},
    };
   
    int rc = XGCIHandleAlloc(NULL, &conp->hdenv, HT_ENV);
    if(rc < 0){
        conp->ebody.hd = NULL;
        return -1; 
    }

    rc = XGCIHandleAlloc(conp->hdenv, &conp->hdsvr, HT_SERVER); 
    if(rc < 0){
        conp->ebody.hd = conp->hdenv;
        return -1;
    }

    rc = XGCIHandleAlloc(conp->hdsvr, &conp->hddbc, HT_SESSION);
    if(rc < 0){
        conp->ebody.hd = conp->hdsvr;
        return -1;
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
        conp->ebody.hd = conp->hddbc;
        return -1;
    }

    return 0; 
}


/* recycle connection resources*/
void xugusql_disconnect(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    
    int rc = XGCISessionEnd(conp->hddbc);
    rc = XGCIHandleFree(conp->hddbc);
    rc = XGCIHandleFree(conp->hdsvr);
    rc = XGCIHandleFree(conp->hdenv);
}


/* In the structured programming process, when the previous layer of 
 * function call fails, the function xugusql_error() is used to obtain 
 * relevant error diagnosis information */
void xugusql_error(void *voip)
{
    xugu_conn_t *conp = (xugu_conn_t *)voip;
    
    struct xgci_err *pe = &conp->ebody;
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


void xugusql_register_plg(void *voip)
{
    db_driver_t *db = (db_driver_t *)voip;

    db->dbconn = calloc(1, sizeof(xugu_conn_t));
    if(!db->dbconn)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    db->dbval = ((xugu_conn_t *)db->dbconn)->plg;
}

