
/* 
 * +====================================================================+
 * | XGCI (Xugu DB C Call Interface).                                   |
 * +--------------------------------------------------------------------+                                                      
 * |                                                                    |
 * |               ____ ____   ________   _______   ____                |
 * |              |    |    | /        | /       \ |    |               |
 * |               \       / |    _____||    _____||    |               |
 * |               /       \ |    |_   ||         ||    |               |
 * |              |____|____| \________| \_______/ |____|               |                              
 * |                                                                    |
 * |                                                                    |
 * |    XGCI is a dedicated C/C++ programming language access interface |                                                          
 * | for xugu database, independently developed by the company, with    |
 * | high performance and high stability.                               |
 * |    Copyright © Xugu Weiye Technology Co., Ltd.                     |
 * |    All rights reserved.                                            |                    
 * +--------------------------------------------------------------------+                                                      
 */

#include "xgci.h"

#include "list.h"
#include "pipe.h"
#define SQL_BUF_SIZE 1024

#define I_YEAR    I_INTERVAL_YEAR
#define I_MONTH   I_INTERVAL_MONTH
#define I_DAY     I_INTERVAL_DAY
#define I_HOUR    I_INTERVAL_HOUR
#define I_MINUTE  I_INTERVAL_MINUTE
#define I_SECOND  I_INTERVAL_SECOND
#define I_Y2M     I_INTERVAL_YEAR_TO_MONTH
#define I_D2H     I_INTERVAL_DAY_TO_HOUR
#define I_D2M     I_INTERVAL_DAY_TO_MINUTE
#define I_D2S     I_INTERVAL_DAY_TO_SECOND
#define I_H2M     I_INTERVAL_HOUR_TO_MINUTE
#define I_H2S     I_INTERVAL_HOUR_TO_SECOND
#define I_M2S     I_INTERVAL_MINUTE_TO_SECOND


typedef struct {
    enum ioty     pkey;
    short         iotype;

}param_iotype;

typedef struct{
    enum typeinx  typekey;
    short         type;
    short         sql_type;

}type_mapp_t;

#define TYPE_OPT(k, t, s) { \
                                .typekey = (k),  \
                                .type = (t),     \
                                .sql_type = (s)}

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
int sql_bindparam(void *voip, db_param_t *param, int no);
int sql_execute(void *voip);

int sql_freehandle(void *voip);
int sql_disconnect(void *);

void sql_error(void *);

