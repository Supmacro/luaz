

typedef struct{
    
    void   *pdb;
    short   Isdown;

}db_conn_t;


typedef struct{

    void   *pdb;
    char   *sql;

    short  Ispre;
    short  Isclose;

}db_stmt_t;



