
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rows.h"
#include "dlist.h"
#include "odbc.h"


int odbc_connect(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
/* SQLAllocHandle allocates an environment, connection, statement or descriptor 
 * handle. This function is a generic function used to allocate handles that 
 * replace the ODBC 2.0 functions SQLAllocConnect, 
 * SQLAllocEnv and SQLAllocStmt. Allows applications to call SQLAllocHandle 
 * for use with ODBC 2. x When the driver calls SQLAllocHandle, it will be mapped 
 * to SQLAllocConnect, 
 * SQLAllocEnv or SQLAllocStmt in the driver manager as needed.
 * */
    int rc = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &conp->hdenv);
    if(rc != SQL_SUCCESS){
        PRT_ERROR("Unknown exception");
        PRT_TAIL_CHR;
        exit(-1);
    }

/*
 * The application can call SQLSetEnvAttr only when the connection handle is 
 * not allocated in the environment. All environment attributes successfully set 
 * by the environment's application will remain until SQLFreeHandle is called on 
 * the environment. Multiple environment handles can be allocated simultaneously 
 * in ODBC 2.x.
 * */
    rc = SQLSetEnvAttr(conp->hdenv, SQL_ATTR_ODBC_VERSION, 
                            (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
    if(rc != SQL_SUCCESS){

        conp->ebody.hdtype = SQL_HANDLE_ENV;
        conp->ebody.hd = conp->hdenv;
        return -1;
    }

    rc = SQLAllocHandle(SQL_HANDLE_DBC, conp->hdenv, &conp->hddbc);
    if(rc != SQL_SUCCESS){
        conp->ebody.hdtype = SQL_HANDLE_ENV;
        conp->ebody.hd = conp->hdenv;
        return -1;
    }

/*
 * SQLConnect is the simplest connection function. It requires a data source name 
 * and accepts an optional user ID and password. 
 * It is suitable for applications that hard-code the data source name and do not 
 * require a user ID or password. 
 * It is also suitable for applications that want to control their "look" or have 
 * no user interface. 
 * Such applications can use SQLDataSources to generate a list of data sources, 
 * prompt the user to enter the data source, user ID and password, and then call 
 * SQLConnect.
 * */
    rc = SQLConnect(conp->hddbc, conp->plg[odbcdsn].value, SQL_NTS, 
          conp->plg[odbcuid].value, SQL_NTS, conp->plg[odbcpwd].value, SQL_NTS);
    if(rc != SQL_SUCCESS){
        conp->ebody.hdtype = SQL_HANDLE_DBC;
        conp->ebody.hd = conp->hddbc;
        return -1;
    }

    return 0;
}


/* 
 * SQLDisconnect closes the connection associated with a specific connection handle.
 * If the application calls SQLDisconnect before releasing all the statements 
 * associated with the connection, the driver will release these statements and all 
 * descriptors that have been explicitly allocated on the connection after 
 * successfully disconnecting from the data source.
 * */
void odbc_disconnect(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    
    int rc = SQLDisconnect(conp->hddbc);
    if(rc != SQL_SUCCESS){
        
        conp->ebody.hdtype = SQL_HANDLE_DBC;
        conp->ebody.hd = conp->hddbc;
        odbc_error(&conp->ebody);
    }

    SQLFreeHandle(SQL_HANDLE_DBC, conp->hddbc);
    SQLFreeHandle(SQL_HANDLE_ENV, conp->hdenv);
}


/*
 * The application calls SQLGetDiagRec or SQLGetDiagField to retrieve diagnostic 
 * information. These functions accept an environment, connection, statement, 
 * or descriptor handle, and return diagnosis from the last function that used 
 * the handle. When using this handle to call a new function, 
 * the diagnosis recorded on the specific handle will be discarded. If the function 
 * returns multiple diagnostic records, the application will call these functions 
 * multiple times; 
 * retrieve the total number of status records by calling SQLGetDiagField for 
 * the header record (record 0) with the SQL_DIAG_NUMBER option.
 * */
void odbc_error(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct odbc_err *pe = &conp->ebody;

    SQLLEN  numRecs = 0;
    SQLGetDiagField(pe->hdtype, pe->hd, 0, SQL_DIAG_NUMBER, &numRecs, 0, 0);
    
    SQLINTEGER NativeError;
    SQLSMALLINT J = 1, msgLength;

    while(J <= numRecs)
    {
        SQLCHAR sqlState[6] = {0}, msg[SQL_MAX_MESSAGE_LENGTH] = {0};
        SQLGetDiagRec(pe->hdtype, pe->hd, J, sqlState, &NativeError, msg, sizeof(msg), &msgLength);
        
        PRT_ERROR(msg);
        PRT_TAIL_CHR;
        J++;
    }
        
    exit(-1);
}


void odbc_register_plg(void *voip)
{
    db_driver_t *db = (db_driver_t *)voip;
    
    db->dbconn = calloc(1, sizeof(odbc_conn_t));
    if(!db->dbconn)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    db->dbval = ((odbc_conn_t *)db->dbconn)->plg;
}
