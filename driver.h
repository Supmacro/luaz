

typedef struct{
    
    void   *pdb;
    short   Isdown;

    void   *connect;
    int     concap;

}db_conn_t;


typedef struct{

    void   *pdb;
    char   *sql;

    void   *connect;
    int     concap;

    short  Ispre;
    short  Isclose;

}db_stmt_t;



