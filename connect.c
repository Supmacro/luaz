
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "deps.h"
#include "connect.h"

/* The xugusql_connect() function is used to establish 
 * a new connection with the specified XUGU database server
 */
int xugusql_connect(connect *pconn)
{
    int port = atoi(pconn->login[host_port]);

    ATRBT exp[] = {
            {&pconn->hdsvr, XGCI_ATTR_SRV_IP, pconn->login[host_ip]},
            {&pconn->hdsvr, XGCI_ATTR_SRV_PORT, &port},
            {&pconn->hdsvr, XGCI_ATTR_SRV_DBNAME, pconn->login[dbname]},
            {&pconn->hddbc, XGCI_ATTR_SESS_VERSION, "301"},
    };
   
    int rc = XGCIHandleAlloc(NULL, &pconn->hdenv, HT_ENV);
    if(rc < 0){
        pconn->hderr = NULL;
        return -1; 
    }

    rc = XGCIHandleAlloc(pconn->hdenv, &pconn->hdsvr, HT_SERVER); 
    if(rc < 0){
        pconn->hderr = pconn->hdenv;
        return -1;
    }

    rc = XGCIHandleAlloc(pconn->hdsvr, &pconn->hddbc, HT_SESSION);
    if(rc < 0){
        pconn->hderr = pconn->hdsvr;
        return -1;
    }

    int j, sz = sizeof(exp)/sizeof(ATRBT);
    for(j = 0; j < sz; j++){
        XGCIHandleAttrSet(*(exp[j].hd), 
                exp[j].type, exp[j].val, XGCI_NTS);
    }

    rc = XGCISessionBegin(pconn->hddbc, 
                 pconn->login[user_name], pconn->login[user_passwd]);
    if(rc < 0){
        pconn->hderr = pconn->hddbc;
        return -1;
    }

    return 0; 
}


/* recycle connection resources*/
void xugusql_disconnect(connect *pconn)
{
    int rc = XGCISessionEnd(pconn->hddbc);
    rc = XGCIHandleFree(pconn->hddbc);
    rc = XGCIHandleFree(pconn->hdsvr);
    rc = XGCIHandleFree(pconn->hdenv);
}


/* In the structured programming process, when the previous layer of 
 * function call fails, the function xugusql_error() is used to obtain 
 * relevant error diagnosis information */
int xugusql_error(XGCIHANDLE hd)
{
    char      msg[512] = {0}, errcode[7]={0};
    unsigned  length;

    if(hd == NULL)
    {
        PRT_ERROR("Unknown exception");
        PRT_TAIL_CHR;
        exit(-1);
    }

    XGCIErrors(hd, errcode, msg, &length);
    
    PRT_ERROR(msg);
    PRT_TAIL_CHR;
    exit(-1);
}


