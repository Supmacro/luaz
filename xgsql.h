
/* 
 * +================================================================+
 * | XGCI (Xugu DB C Call Interface).                               |
 * +----------------------------------------------------------------+                                                      
 * |                                                                |
 * |             ____ ____   ________   _______   ____              |
 * |            |    |    | /        | /       \ |    |             |
 * |             \       / |    _____||    _____||    |             |
 * |             /       \ |    |_   ||         ||    |             |
 * |            |____|____| \________| \_______/ |____|             |                              
 * |                                                                |
 * | Copyright © Xugu Weiye Technology Co., Ltd.                    |
 * | All rights reserved.                                           |                    
 * +----------------------------------------------------------------+                                                      
 */

#include "xgci.h"
#include "io.h"

#include "rows.h"

#define SQL_BUF_SIZE 1024

typedef struct{ 
    XGCIHANDLE *hd;    /* handle ptr */ 
    int         type;  /* attr type  */
    short       opt; 
}ATRBT;

/*
enum xgci_opt {
    host_ip,
    host_port,
    dbname,
    user_name,
    user_passwd,
    charset,
    xgcieof,
};
*/

struct xgci_err{

    XGCIHANDLE  hd;

    char        errcode[7];
    char        errmsg[256];
    unsigned    length;
};

typedef struct {

    XGCIHANDLE  hdenv;
    XGCIHANDLE  hdsvr;
    XGCIHANDLE  hddbc;
    XGCIHANDLE  hdstmt;
    
    struct xgci_err ebody;
    db_value_t      plg[xugueof];

}xugu_conn_t;

#define UTF8    XGCI_CHARSET_UTF8
#define GBK     XGCI_CHARSET_GBK
#define GB2312  XGCI_CHARSET_GB2312

int xugusql_connect();
void xugusql_disconnect();
void xugusql_error();


