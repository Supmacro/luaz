
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
#define SQL_BUF_SIZE 1024

typedef struct{ 
    XGCIHANDLE  *hd; /* handle ptr */ 
    int    type;     /* attr type  */
    void  *val;      /* attr value */
    
}ATRBT;

enum dsname {
    host_ip,
    host_port,
    dbname,
    user_name,
    user_passwd,
    eof,
};

typedef struct{
    void  *val;
    int    rcode;
    int    size;
    int    length;
}column;


typedef struct{
    column  *pcol;    
    int      cap;    
}row;

typedef struct {
   
    char  *login[eof];

    XGCIHANDLE  hdenv;
    XGCIHANDLE  hdsvr;
    XGCIHANDLE  hddbc;
    XGCIHANDLE  hdstmt;
    XGCIHANDLE  hderr;

}connect;


typedef struct {
    enum dsname key;
    const char *name;
    char       *value;

}KV;

int xugusql_connect(connect *pconn);
void xugusql_disconnect(connect *pconn);
int xugusql_error(XGCIHANDLE hd);


