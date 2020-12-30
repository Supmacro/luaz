

local ffi = require("ffi")

ffi.cdef [[
    void malloc(size_t ); 
    void luaz_safe_free(void *);
]]

str='world'
print(string.format("Hello, %s", str))

