
enum typeinx {

    I_TINYINT,
    I_SMALLINT,
    I_INTEGER,
    I_BIGINT,
    I_FLOAT,
    I_DOUBLE,
    I_NUMERIC,

    I_CHAR,
    I_VARCHAR,
    I_CLOB,

    I_GUID,
    I_BOOLEAN,
    I_BLOB,
    I_BINARY,

    I_DATE,
    I_DATETIME,
    I_DATETIME_TZONE,
    I_TIME,
    I_TIME_TZONE,

    I_INTERVAL_YEAR,
    I_INTERVAL_MONTH,
    I_INTERVAL_DAY,
    I_INTERVAL_HOUR,
    I_INTERVAL_MINUTE,
    I_INTERVAL_SECOND,
    I_INTERVAL_YEAR_TO_MONTH,
    I_INTERVAL_DAY_TO_HOUR,
    I_INTERVAL_DAY_TO_MINUTE,
    I_INTERVAL_DAY_TO_SECOND,
    I_INTERVAL_HOUR_TO_MINUTE,
    I_INTERVAL_HOUR_TO_SECOND,
    I_INTERVAL_MINUTE_TO_SECOND
};


typedef struct{
    
    void   *pdb;
    short   Isdown;

    void   *connect;
    int     concap;

}db_conn_t;


typedef struct{

    void   *pdb;
    char   *sql;

    void   *connect;
    int     concap;

    short  Ispre;
    short  Isclose;

}db_stmt_t;


typedef struct{

    void   *value;
    int     len;
    int     len_max;

    int     IOtype;
    int     rcode;
    short   type;
    short   sql_type;

}db_param_t;



