

local ffi = require("ffi")

ffi.cdef [[

    typedef struct{
        void   *value;
        int    *len;
        int     len_max;

        int     iot;
        int     rcode;
        short   type;
    
    }db_param_t;

    typedef struct{
        int  (*drv_connect)(void *);
        int  (*drv_prepare)(void *, char *);
        int  (*drv_bindparam)(void *, db_param_t *, int);
        int  (*drv_execute)(void *);
        int  (*drv_freestmt)(void *);

        void (*drv_disconnect)(void *);
        void (*drv_error)(void *);
        void (*drv_register)(void *);
    }op_t;

    typedef struct{
        void    *value;
        int      length;
    }db_value_t;

    typedef struct{
        char    *lname;
        
        void    *dbconn;
        int      dbsize;

        short    vcap;
        op_t     ops;

        db_value_t  *dbval;

    }db_driver_t;

    typedef struct{
        void    *pdb;
        short    Isdown;

        void    *connect;
        int      concap;

    }db_conn_t;

    typedef struct{
        void    *pdb;
        char    *sql;

        void    *connect;
        int      concap;

        short   Ispre;
        short   Isclose;

    }db_stmt_t;

    db_driver_t *db_loaded(const char *);
    void db_destroy(db_driver_t *pd);
    
    db_conn_t *db_connect(db_driver_t *);
    void db_disconnect(db_conn_t *);
    void db_conn_destroy(db_conn_t *);

    db_stmt_t *db_prepare(db_conn_t *, const char *);
    void db_bind_parameter(db_stmt_t *, db_param_t *, int);
    void db_execute(db_stmt_t *);
    void db_stmt_close(db_stmt_t *);
    void db_stmt_destroy(db_stmt_t *);
]]


local db_driver = ffi.typeof('db_driver_t *')
local driver = {}

function sql.driver(driver_name)
    local db = ffi.C.db_loaded(driver_name)
    return ffi.gc(db, ffi.C.db_destroy)
end

function driver.connect(self)
    local conn = ffi.C.db_connect(self)
    return ffi.gc(conn, ffi.C.db_conn_destroy)
end

function driver.name(self)
    return ffi.string(self.lname)
end

--driver metamethod
local driver_meta = {
    __index = driver,
    __gc = ffi.C.db_destroy,
    __tostring = function() return '<db_driver>' end
}

ffi.metatype("db_driver_t", driver_meta)


--driver connect
local db_conn = ffi.typeof('db_conn_t *')
local connect = {}

function connect.prepare(self, query)
    local stmt = ffi.C.db_prepare(self, query)
    return ffi.gc(stmt, ffi.C.db_stmt_destroy) 
end

function connect.close(self)
    ffi.C.db_disconnect(self)
end

local connect_meta = {
    __index = connect,
    __gc = ffi.C.db_conn_destroy,
    __tostring = function() return '<db_conn>' end
}

ffi.metatype("db_conn_t", connect_meta)


--driver statement
local db_stmt = ffi.typeof('db_stmt_t *')
local param_meta = {}


function param_meta.set(self, value, ...)

    local bind = sql.map[self.type]

    if value == nil then
            self.data_len[0] = 0
            return self 
    end

    if bind.def == "char[?]" 
    then
            local len = #value 
            if select('#', ...) > 0 then
                    len = select('1', ...)
            end 

            self.data_len[0] = len == 0 and 1 or len
            self.type = bind.type
            ffi.copy(self.value, value, self.data_len[0])
    else
            self.data_len[0] = bind.size
            self.type = bind.type
            self.value[0] = value
    end

    return self
    
end


function param_meta.get(self)
    
    if self.data_len[0] <= 0 then
            return nil
    end

    if self.type == sql.type.CHAR 
    then
            return ffi.string(self.value, self.data_len[0])
    elseif self.type == sql.type.TINYINT or
           self.type == sql.type.SMALLINT or
           self.type == sql.type.INTEGER or
           self.type == sql.type.BIGINT 
    then
            return tonumber(self.value[0])
    elseif self.type == sql.type.FLOAT or
           self.type == sql.type.DOUBLE or 
           self.type == sql.type.BOOLEAN
    then
           --return tonumber(ffi.cast("double", self.value[0]))
           return self.value[0]
    end

end


local parameter = {
    __index = param_meta, 
    __tostring = function() return '<param>' end
}


function param_create(btype, max_len, ...)

    local param = setmetatable({}, parameter)
    local PIO = sql.type.PARAM_INPUT

    if select('#', ...) > 0 then
            PIO = select(1, ...)
            if PIO ~= sql.type.PARAM_INPUT and 
               PIO ~= sql.type.PARAM_INPUT_OUTPUT and
               PIO ~= sql.type.PARAM_OUTPUT 
            then
                    error("The IO type of the binding parameter " .. 
                            "can only be PARAM_INPUT, PARAM_INPUT_OUTPUT, PARAM_OUTPUT.")
            end
    end

    if not btype or not max_len then
            error("When binding parameters, give at least the data type of " .. 
                    "the bound parameter and the maximum number of bytes of memory" ..
                                        " space expected to be occupied.")
    end

    local bind = sql.map[btype]

    if bind.def == "char[?]" then
            param.max_len = max_len
            param.type = btype
            param.value = ffi.new(bind.def, max_len)
            param.pio = PIO
    else
            param.max_len = bind.size
            param.type = btype
            param.value = ffi.new(bind.def)
            param.pio = PIO
    end

    param.data_len = ffi.new("int[1]")
    return param

end


local stmt = {}

function stmt.bind_param(self, ...)

    local nparam = select("#", ...)
    assert(nparam >= 1, "The number of parameter bindings is at least 1")
    
    local params = ffi.new(ffi.typeof('db_param_t[?]'), nparam)
    for i, param in pairs({...}) 
    do
            params[i-1].len_max = param.max_len
            params[i-1].len = param.data_len
            params[i-1].type = param.type
            params[i-1].value = param.value
            params[i-1].iot = param.pio         
    end

    return ffi.C.db_bind_parameter(self, params, nparam)
end


function stmt.execute(self)
    ffi.C.db_execute(self) 
end


function stmt.close(self)
    ffi.C.db_stmt_close(self)
end


local smeta = {
    __index = stmt,
    __tostring = function() return '<db_stmt>' end
}

ffi.metatype("db_stmt_t", smeta)



