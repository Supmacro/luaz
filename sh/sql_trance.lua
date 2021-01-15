

local db = sql.driver("xgci.drv1")

local conn = db:connect()
local stmt = conn:prepare("INSERT INTO TEST VALUES(?, ?);");

local param = {
    stmt:bind_create(sql.type.INTEGER, 8),
    stmt:bind_create(sql.type.CHAR, 32)
}

stmt:bind_param(unpack(param))

param[1]:set(666)
param[2]:set("SEX")

stmt:execute()

stmt:close()
conn:close()

