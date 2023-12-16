#include <stdio.h>
#include <libpq-fe.h>

void exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int main() 
{
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;

    conninfo = "dbname = postgres user = postgres password = root hostaddr = 127.0.0.1 port = 5432";

    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    res = PQexec(conn, "SELECT * FROM users");

    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "SELECT query failed: %s", PQerrorMessage(conn));
        PQclear(res);
        exit_nicely(conn);
    }

    PQclear(res);
    PQfinish(conn);

    return 0;
}