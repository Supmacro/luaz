

local ffi = require("ffi")

ffi.cdef [[

    typedef enum {
        PIO_INPUT,
        PIO_INPUT_OUTPUT,
        PIO_OUTPUT
    }param_type;

    typedef enum {

        TINYINT,
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
    INTERVAL_MINUTE_TO_SECOND = ffi.C.INTERVAL_MINUTE_TO_SECOND
}

--[[
sql = {
    PARAM_INPUT = ffi.C.PIO_INPUT,
    PARAM_INPUT_OUTPUT = ffi.C.PIO_INPUT_OUTPUT.
    PARAM_OUTPUT = ffi.C.PIO_OUTPUT
} --]]

--[[
sql.map = {

    {sql.type.TINYINT   , max_len =  1,   ctype = 'char[?]'   },
    {sql.type.SMALLINT  , max_len =  2,   ctype = 'short[?]'  },
    {sql.type.INTEGER   , max_len =  4,   ctype = 'int[?]'    },
    {sql.type.BIGINT    , max_len =  8,   ctype = 'int64_t[?]'},
    {sql.type.FLOAT     , max_len =  4,   ctype = 'float[?]'  },
    {sql.type.DOUBLE    , max_len =  8,   ctype = 'double[?]' },
    {sql.type.NUMERIC   , max_len = -1,   ctype = 'char[?]'   },
    {sql.type.CHAR      , max_len = -1,   ctype = 'char[?]'   },
    {sql.type.VARCHAR   , max_len = -1,   ctype = 'char[?]'   },
    {sql.type.CLOB      , max_len = -1,   ctype = 'char[?]'   },
    {sql.type.GUID      , max_len = -1,   ctype = 'char[?]'   },
    {sql.type.BOOLEAN   , max_len =  1,   ctype = 'char[?]'   },
    {sql.type.BLOB      , max_len = -1,   ctype = 'char[?]'   },

}
--]]




