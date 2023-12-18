#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "sql.h"
#include "messages.h"

void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}

PGconn *connectToDatabase() 
{
    const char *conninfo;
    PGconn     *conn;

    conninfo = "dbname = biblio user = postgres password = root hostaddr = 127.0.0.1 port = 5432";
    conn = PQconnectdb(conninfo);

    if(PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Nepavyko prisijungti prie duombazes: %s", PQerrorMessage(conn));
        exit_nicely(conn);
    }

    return conn;
}

void displayUserList(PGconn *conn)
{
    printQueryResults(conn, "SELECT * FROM klientas;", getMessage(MSG_NO_USERS));
}

void displayCategoryList(PGconn *conn)
{
    printQueryResults(conn, "SELECT * FROM kategorija;", getMessage(MSG_NO_CATEGORIES));
}

void displayProductList(PGconn *conn)
{
    printQueryResults(conn, "SELECT * FROM produktas;", getMessage(MSG_NO_PRODUCTS));
}

void addNewCategory(PGconn *conn, char *categoryName)
{
    char query[100];
    sprintf(query, "INSERT INTO kategorija (pavadinimas) VALUES ('%s');", categoryName);
    printf("%s\n", query);
    printf("%s\n", categoryName);
    printInsertResult(conn, query, "Kategorija prideta!");
}

void addNewProduct(PGconn *conn, int categoryID, char *productName, float price, int quantity)
{
    char query[255];
    sprintf(query, "INSERT INTO produktas (kategorijos_nr, pavadinimas, kaina, kiekis) VALUES (%d, '%s', %f, %d);", categoryID, productName, price, quantity);
    printf("%s\n", query);
    printInsertResult(conn, query, "Produktas pridetas!");
}

void printQueryResults(PGconn *conn, const char *query, const char *noResultsMessage) 
{
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Neteisinga uzklausa: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int nrows = PQntuples(res);
    int ncols = PQnfields(res);
    int colWidths[ncols];

    if (nrows == 0) {
        printf("%s\n", noResultsMessage);  // Print the message if table is empty
        PQclear(res);
        return;
    }

    // Determine the maximum width for each column
    for (int col = 0; col < ncols; col++) {
        colWidths[col] = strlen(PQfname(res, col)); // Start with header's length
        for (int row = 0; row < PQntuples(res); row++) {
            int cellWidth = strlen(PQgetvalue(res, row, col));
            if (cellWidth > colWidths[col]) {
                colWidths[col] = cellWidth; // Adjust to the longest content
            }
        }
    }

    // Print column headers
    for (int col = 0; col < ncols; col++) {
        printf("%-*s ", colWidths[col], PQfname(res, col));
    }
    printf("\n");

    // Print rows
    for (int row = 0; row < PQntuples(res); row++) {
        for (int col = 0; col < ncols; col++) {
            printf("%-*s ", colWidths[col], PQgetvalue(res, row, col));
        }
        printf("\n");
    }

    PQclear(res);
}

void printInsertResult(PGconn *conn, const char *query, const char *text)
{
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Neteisinga uzklausa: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    printf("%s\n", text);

    PQclear(res);
}

bool isValidCategory(PGconn *conn, int categoryID)
{
    char query[100];
    sprintf(query, "SELECT EXISTS(SELECT 1 FROM kategorija WHERE kategorijos_id = %d);", categoryID);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    if (strcmp(PQgetvalue(res, 0, 0), "t") == 0) {
        return true;
    } else {
        printf("Tokios kategorijos nera. Bandykite dar karta. \n");
        return false;
    }
}