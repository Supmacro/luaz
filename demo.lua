

local db = sql.driver("xgci.drv1")
local SQL = "insert into test values(?,?,?,?,?,?,?)"

local conn = db:connect()
local stmt = conn:prepare(SQL);

local params = {
    param_create(sql.type.INTEGER, 8):set(1),
    param_create(sql.type.CHAR, 32):set("SQL"), 
    param_create(sql.type.BOOLEAN, 1):set(false),
    param_create(sql.type.FLOAT, 4):set(0.024),
    param_create(sql.type.DOUBLE, 8):set(7.12),
    param_create(sql.type.NUMERIC, 8):set("20.20"),
    param_create(sql.type.DATETIME, 20):set("2021-01-28 00:00:00")
}

stmt:bind_param(unpack(params))
stmt:execute()

for k, v in ipairs(params) do
    print(v:get())
end

stmt:close()
conn:close()

