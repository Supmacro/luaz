

enum optype {
    OPT_NULL,
    OPT_STRING,
    OPT_INT,
    OPT_BOOLEAN,
    OPT_FLOAT,
    OPT_END
};


enum xgci_opt {
    host_dsn = 0,
    host_ip = 0,
    user_name,
    user_passwd,
    dbname,
    odbceof = 3,
    host_port,
    charset,
    xgcieof
};

typedef struct {
    const char  *drv_name;
    short        item;
    const char  *desc;
    const char  *name;
    char        *value;
    enum optype  type;
}KV;

#define OPT(d, k, e, n, v, t) {         \
                    .drv_name = (d),    \
                    .item = (k),        \
                    .desc = (e),        \
                    .name = (n),        \
                    .value = (v),       \
                    .type = OPT_##t     \
            }

#define LZ_HOST_IP     "host_ip"
#define LZ_HOST_PORT   "host_port"
#define LZ_DBNAME      "dbname"
#define LZ_USER_NAME   "user_name"
#define LZ_USER_PASSWD "user_passwd"
#define LZ_CHARSET     "charset"
#define LZ_HOST_DSN    "host_dsn"

char *opt_upper(char *str);
void  opt_driver_name(const char *, char *, char *);


