
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lz_option.h"
#include "lz_odbc.h"



static type_mapp_t type_ODBC[] = {

        TYPE_OPT(I_TINYINT        ,  SQL_TINYINT                  , SQL_C_TINYINT),
        TYPE_OPT(I_SMALLINT       ,  SQL_SMALLINT                 , SQL_C_SHORT),
        TYPE_OPT(I_INTEGER        ,  SQL_INTEGER                  , SQL_C_LONG),
        TYPE_OPT(I_BIGINT         ,  SQL_BIGINT                   , SQL_C_SBIGINT),
        TYPE_OPT(I_FLOAT          ,  SQL_FLOAT                    , SQL_C_FLOAT),
        TYPE_OPT(I_DOUBLE         ,  SQL_DOUBLE                   , SQL_C_DOUBLE),
        TYPE_OPT(I_NUMERIC        ,  SQL_NUMERIC                  , SQL_C_NUMERIC),
        TYPE_OPT(I_CHAR           ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_VARCHAR        ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_CLOB           ,  SQL_LONGVARCHAR              , SQL_C_CHAR),
        TYPE_OPT(I_GUID           ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_BOOLEAN        ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_BLOB           ,  SQL_LONGVARBINARY            , SQL_C_BINARY),
        TYPE_OPT(I_BINARY         ,  SQL_BINARY                   , SQL_C_BINARY),
        TYPE_OPT(I_DATE           ,  SQL_DATE                     , SQL_C_DATE),
        TYPE_OPT(I_DATETIME       ,  SQL_DATETIME                 , SQL_C_CHAR),
        TYPE_OPT(I_DATETIME_TZONE ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_TIME           ,  SQL_TIME                     , SQL_C_TIME),
        TYPE_OPT(I_TIME_TZONE     ,  SQL_CHAR                     , SQL_C_CHAR),
        TYPE_OPT(I_YEAR           ,  SQL_INTERVAL_YEAR            , SQL_C_CHAR),
        TYPE_OPT(I_MONTH          ,  SQL_INTERVAL_MONTH           , SQL_C_CHAR),
        TYPE_OPT(I_DAY            ,  SQL_INTERVAL_DAY             , SQL_C_CHAR),
        TYPE_OPT(I_HOUR           ,  SQL_INTERVAL_HOUR            , SQL_C_CHAR),
        TYPE_OPT(I_MINUTE         ,  SQL_INTERVAL_MINUTE          , SQL_C_CHAR),
        TYPE_OPT(I_SECOND         ,  SQL_INTERVAL_SECOND          , SQL_C_CHAR),
        TYPE_OPT(I_Y2M            ,  SQL_INTERVAL_YEAR_TO_MONTH   , SQL_C_CHAR),
        TYPE_OPT(I_D2H            ,  SQL_INTERVAL_DAY_TO_HOUR     , SQL_C_CHAR),
        TYPE_OPT(I_D2M            ,  SQL_INTERVAL_DAY_TO_MINUTE   , SQL_C_CHAR),
        TYPE_OPT(I_D2S            ,  SQL_INTERVAL_DAY_TO_SECOND   , SQL_C_CHAR),
        TYPE_OPT(I_H2M            ,  SQL_INTERVAL_HOUR_TO_MINUTE  , SQL_C_CHAR),
        TYPE_OPT(I_H2S            ,  SQL_INTERVAL_HOUR_TO_SECOND  , SQL_C_CHAR),
        TYPE_OPT(I_M2S            ,  SQL_INTERVAL_MINUTE_TO_SECOND, SQL_C_CHAR)

};


static param_iotype pio[] = {
                                {PIO_INPUT        , SQL_PARAM_INPUT},
                                {PIO_INPUT_OUTPUT , SQL_PARAM_INPUT_OUTPUT},
                                {PIO_OUTPUT       , SQL_PARAM_OUTPUT}
        };


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
    struct error *pe = &conp->ebody;

    rc = SQLSetEnvAttr(conp->hdenv, SQL_ATTR_ODBC_VERSION, 
                            (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_ENV;
        pe->hd = conp->hdenv;
        return rc;
    }

    rc = SQLAllocHandle(SQL_HANDLE_DBC, conp->hdenv, &conp->hddbc);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_ENV;
        pe->hd = conp->hdenv;
        return rc;
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
    rc = SQLConnect(conp->hddbc, conp->plg[host_dsn].value, SQL_NTS, 
          conp->plg[user_name].value, SQL_NTS, conp->plg[user_passwd].value, SQL_NTS);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_DBC;
        pe->hd = conp->hddbc;
        return rc;
    }

    return rc;
}


/* SQLPrepare prepares the SQL string to be executed. 
 * After preparing the statement, the application will use the statement handle to 
 * refer to the statement in the subsequent function call. 
 * You can re-execute the pre-defined statement associated with the statement handle 
 * by calling SQLExecute until the application releases the statement with the 
 * SQL_DROP option and calls SQLFreeStmt, until it is used in a call to SQLPrepare, 
 * SQLExecDirect or (SQLColumns, SQLTables, etc.) The statement handle.
 * */
int odbc_prepare(void *voip, char *sql)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = SQLAllocHandle(SQL_HANDLE_STMT, conp->hddbc, &conp->hdstmt); 
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_DBC;
        pe->hd = conp->hddbc;
        return rc;
    }

    rc = SQLPrepare(conp->hdstmt, sql, SQL_NTS);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_STMT;
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc; 
}


/*
 * SQLBindParameter binds the buffer to the parameter marker in the SQL statement. 
 * SQLBindParameter supports binding to unicode C data types, 
 * even if the base driver does not support unicode data. 
 * */
int odbc_bindparam(void *voip, db_param_t *param, int no)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = SQLBindParameter(conp->hdstmt, no, pio[param->iot].iotype, type_ODBC[param->type].type, 
                    type_ODBC[param->type].sql_type, 
                    param->len_max, 0, param->value, param->len_max, (SQLLEN*)param->len);
    if(rc < 0){
        pe->hdtype = SQL_HANDLE_STMT;
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc;
}


/*
 * SQLExecute executes the statement prepared by SQLPrepare. 
 * After the application processes or discards the results from the call to SQLExecute, 
 * the application can call SQLExecute again with the new parameter values. 
 * */
int odbc_execute(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct error *pe = &conp->ebody;
   
    int rc = SQLExecute(conp->hdstmt);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_STMT;
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc;
}


/* SQLFreeHandle is used to release the statement handle */
int odbc_freehandle(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct error *pe = &conp->ebody;

    int rc = SQLFreeHandle(SQL_HANDLE_STMT, conp->hdstmt);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_STMT;
        pe->hd = conp->hdstmt;
        return rc;
    }

    return rc;
}


/* 
 * SQLDisconnect closes the connection associated with a specific connection handle.
 * If the application calls SQLDisconnect before releasing all the statements 
 * associated with the connection, the driver will release these statements and all 
 * descriptors that have been explicitly allocated on the connection after 
 * successfully disconnecting from the data source.
 * */
int odbc_disconnect(void *voip)
{
    odbc_conn_t *conp = (odbc_conn_t *)voip;
    struct error *pe = &conp->ebody;
    
    int rc = SQLDisconnect(conp->hddbc);
    if(rc != SQL_SUCCESS){
        pe->hdtype = SQL_HANDLE_DBC;
        pe->hd = conp->hddbc;
        return rc;
    }

    SQLFreeHandle(SQL_HANDLE_DBC, conp->hddbc);
    SQLFreeHandle(SQL_HANDLE_ENV, conp->hdenv);

    return rc;
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
    struct error *pe = &conp->ebody;

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
   
    db->dbsize = sizeof(odbc_conn_t);
    db->dbconn = calloc(1, db->dbsize);
    if(!db->dbconn)
    {
        PRT_ERROR("Not enough free memory");
        PRT_TAIL_CHR;
        exit(-1);
    }

    db->dbval = ((odbc_conn_t *)db->dbconn)->plg;
}
