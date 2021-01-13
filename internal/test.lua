

local fd = io.open("/home/git/luaz/internal/bin", "rb")

assert(fd)
local data = fd:read("*a")


