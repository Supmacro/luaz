

driver = {

    odbc = {
            drv1 = {
                host_dsn = "SQL",
                user_name = "qxj",
                user_passwd = "Aa120110",
            },

            drv2 = {
                host_dsn = "odbc",
                user_name = "qxj",
                user_passwd = "Aa120110",
            }
    },

    xgci = {
            drv1 = {
                host_ip = "127.0.0.1",
                host_port = "5138",
                dbname = "SYSTEM",
                user_name = "SYSDBA",
                user_passwd = "SYSDBA",
            },

            drv2 = {
                host_ip = "192.168.2.40",
                host_port = "5139",
                dbname = "SYSTEM",
                user_name = "SYSDBA",
                user_passwd = "SYSDBA",
            
            }
    }

}

return  driver 
