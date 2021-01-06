
#include <odbcinst.h>
#include <sqlext.h>
#include <sqltypes.h>

#include "../io.h"

struct odbc_err {
    SQLSMALLINT hdtype;
    SQLHANDLE   hd;
};

typedef struct {
    
    SQLHANDLE   hdenv;
    SQLHANDLE   hddbc;
    SQLHANDLE   hdstmt;

    struct odbc_err  ebody;
    db_value_t       plg[odbceof];

}odbc_conn_t;


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
int odbc_connect(void *);


/* <-> SQLPrepare prepares the SQL string to be executed. 
 * After preparing the statement, the application will use the statement handle to 
 * refer to the statement in the subsequent function call. 
 * You can re-execute the pre-defined statement associated with the statement handle 
 * by calling SQLExecute until the application releases the statement with the 
 * SQL_DROP option and calls SQLFreeStmt, until it is used in a call to SQLPrepare, 
 * SQLExecDirect or (SQLColumns, SQLTables, etc.) The statement handle.
 * */
int odbc_prepare(void *voip, char *sql);

/* 
 * SQLDisconnect closes the connection associated with a specific connection handle.
 * If the application calls SQLDisconnect before releasing all the statements 
 * associated with the connection, the driver will release these statements and all 
 * descriptors that have been explicitly allocated on the connection after 
 * successfully disconnecting from the data source.
 * */
void odbc_disconnect(void *);


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
void odbc_error(void *);
