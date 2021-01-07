

local db = sql.driver("xgci.drv1")

local conn = db:connect()
local stmt = conn:prepare("INSERT INTO TEST VALUES(1, 'Luaz');");

stmt:execute()

stmt:close()
conn:close()

