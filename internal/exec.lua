

local db = sql.driver("xgci.drv1")

local conn = db:connect()
local stmt = conn:prepare("INSERT INTO TEST VALUES(1, 'Luaz');");

--stmt:bindParam({sql.type.CHAR, sql.PIO_INPUT, 64, value},
--               {sql.type.CHAR, sql.PIO_INPUT_OUTPUT, 64, value},
--               {sql.type.CHAR, sql.PIO_OUTPUT, 64, value})
stmt:execute()

stmt:close()
conn:close()


print(type(conn))
