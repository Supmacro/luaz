

local ffi = require("ffi")

ffi.cdef [[

    void luaz_safe_free(char *p);
    
]]

cdate = ffi.gc(ffi.C.malloc(100), ffi.C.luaz_safe_free)
cdate = nil

