

local db = sql.driver("xgci.drv1")

local conn = db:connect()
local name = db:driver_name()
print(name)

conn:close()

db = nil


