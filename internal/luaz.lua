

local ffi = require("ffi")

ffi.cdef [[
    int db_connect(const char *);
]]


local rc = ffi.C.db_connect("xgci.drv2");
print(rc)
