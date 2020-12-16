
typedef enum {
    XGCI = -2,
    ODBC

}API;

typedef struct{
    void  *val;
    int    rcode;
    int    size;
    int    length;
}column;


typedef struct{
    column  *pcol;    
    int      cap;    
}row;


enum optype {
    OPT_NULL,
    OPT_STRING,
    OPT_INT,
    OPT_BOOLEAN,
    OPT_FLOAT,
    OPT_END
};

enum xgci_opt {
    host_ip,
    host_port,
    dbname,
    user_name,
    user_passwd,
    charset,
    xugueof,
};

enum odbc_inx {
    odbcdsn,
    odbcuid,
    odbcpwd,
    odbceof,
};

typedef struct {
    void    *value;
    int      length;
}db_value_t;


typedef struct {
    const char  *drv_name;
    short        item;
    const char  *name;
    char        *value;
    enum optype  type;
}KV;

#define OPT(d, k, n, v, t) {            \
                    .drv_name = (d),    \
                    .item = (k),        \
                    .name = (n),        \
                    .value = (v),       \
                    .type = OPT_##t     \
            }

db_value_t *row_read_db_option(KV opt, db_value_t *pval);
char *row_strv_to_upper(char *str);


