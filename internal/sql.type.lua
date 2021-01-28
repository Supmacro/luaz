

local ffi = require("ffi")

ffi.cdef [[

    typedef enum {
        PIO_INPUT,
        PIO_INPUT_OUTPUT,
        PIO_OUTPUT
    }param_type;

    typedef enum {

        TINYINT = 1,
        SMALLINT,
        INTEGER,
        BIGINT,
        FLOAT,
        DOUBLE,
        NUMERIC,

        CHAR,
        VARCHAR,
        CLOB,

        GUID,
        BOOLEAN,
        BLOB,
        BINARY,

        DATE,
        DATETIME,
        DATETIME_WITH_TIME_ZONE,
        TIME,
        TIME_WITH_TIME_ZONE,
        
        INTERVAL_YEAR,
        INTERVAL_MONTH,
        INTERVAL_DAY,
        INTERVAL_HOUR,
        INTERVAL_MINUTE,
        INTERVAL_SECOND,
        INTERVAL_YEAR_TO_MONTH,
        INTERVAL_DAY_TO_HOUR,
        INTERVAL_DAY_TO_MINUTE,
        INTERVAL_DAY_TO_SECOND,
        INTERVAL_HOUR_TO_MINUTE,
        INTERVAL_HOUR_TO_SECOND,
        INTERVAL_MINUTE_TO_SECOND,

    }data_type;

]]


sql.type = {

    TINYINT = ffi.C.TINYINT,
    SMALLINT = ffi.C.SMALLINT,
    INTEGER = ffi.C.INTEGER,
    BIGINT = ffi.C.BIGINT,
    FLOAT = ffi.C.FLOAT,
    DOUBLE = ffi.C.DOUBLE,
    NUMERIC = ffi.C.NUMERIC,
    
    CHAR = ffi.C.CHAR,
    VARCHAR = ffi.C.VARCHAR,

    CLOB = ffi.C.CLOB,
    GUID = ffi.C.GUID,
    BOOLEAN = ffi.C.BOOLEAN,
    BLOB = ffi.C.BLOB,
    BINARY = ffi.C.BINARY,

    DATE = ffi.C.DATE,
    DATETIME = ffi.C.DATETIME,
    DATETIME_WITH_TIME_ZONE = ffi.C.DATETIME_WITH_TIME_ZONE,
    TIME = ffi.C.TIME,
    TIME_WITH_TIME_ZONE = ffi.C.TIME_WITH_TIME_ZONE,

    INTERVAL_YEAR = ffi.C.INTERVAL_YEAR,
    INTERVAL_MONTH = ffi.C.INTERVAL_MONTH,
    INTERVAL_DAY = ffi.C.INTERVAL_DAY,
    INTERVAL_HOUR = ffi.C.INTERVAL_HOUR,
    INTERVAL_MINUTE = ffi.C.INTERVAL_MINUTE,
    INTERVAL_SECOND = ffi.C.INTERVAL_SECOND,
    INTERVAL_YEAR_TO_MONTH = ffi.C.INTERVAL_YEAR_TO_MONTH,
    INTERVAL_DAY_TO_HOUR = ffi.C.INTERVAL_DAY_TO_HOUR,
    INTERVAL_DAY_TO_MINUTE = ffi.C.INTERVAL_DAY_TO_MINUTE,
    INTERVAL_DAY_TO_SECOND = ffi.C.INTERVAL_DAY_TO_SECOND,
    INTERVAL_HOUR_TO_MINUTE = ffi.C.INTERVAL_HOUR_TO_MINUTE,
    INTERVAL_HOUR_TO_SECOND = ffi.C.INTERVAL_HOUR_TO_SECOND,
    INTERVAL_MINUTE_TO_SECOND = ffi.C.INTERVAL_MINUTE_TO_SECOND,

    PARAM_INPUT = ffi.C.PIO_INPUT,
    PARAM_INPUT_OUTPUT = ffi.C.PIO_INPUT_OUTPUT,
    PARAM_OUTPUT = ffi.C.PIO_OUTPUT
}


sql.map = {

        {type = sql.type.TINYINT  , def = "int64_t[1]", size = 8},
        {type = sql.type.SMALLINT , def = "int64_t[1]", size = 8},
        {type = sql.type.INTEGER  , def = "int64_t[1]", size = 8},
        {type = sql.type.BIGINT   , def = "int64_t[1]", size = 8},
        {type = sql.type.FLOAT    , def = "float[1]"  , size = 4},
        {type = sql.type.DOUBLE   , def = "double[1]" , size = 8},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CLOB     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.BOOLEAN  , def = "char[1]"   , size = 1},
        {type = sql.type.BLOB     , def = "char[?]"   , size = 0},
        {type = sql.type.BINARY   , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0}, 
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0},
        {type = sql.type.CHAR     , def = "char[?]"   , size = 0}
} 

