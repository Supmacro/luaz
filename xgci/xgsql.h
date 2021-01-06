
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
#include "../io.h"

#define SQL_BUF_SIZE 1024


typedef struct{ 
    XGCIHANDLE *hd;    /* handle ptr */ 
    int         type;  /* attr type  */
    short       opt; 
}ATRBT;

struct error {

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

    struct error ebody;
    db_value_t      plg[xgcieof];

}xugu_conn_t;

#define UTF8    XGCI_CHARSET_UTF8
#define GBK     XGCI_CHARSET_GBK
#define GB2312  XGCI_CHARSET_GB2312

int sql_connect(void *);
int sql_prepare(void *voip, char *sql);
int sql_execute(void *voip);

int sql_freehandle(void *voip);
int sql_disconnect(void *);

void sql_error(void *);

