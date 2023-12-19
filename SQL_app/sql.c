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

void displayOrderList(PGconn *conn, int userID)
{
    char query[1024];
    sprintf(query, 
        "SELECT k.pavadinimas AS Kategorija, p.pavadinimas AS ProduktoPavadinimas, p.kaina AS Kaina, pi.kiekis AS Kiekis "
        "FROM pardavimo_informacija pi "
        "JOIN pardavimas pa ON pi.pardavimo_nr = pa.pardavimo_id "
        "JOIN produktas p ON pi.produkto_nr = p.produkto_id "
        "JOIN kategorija k ON p.kategorijos_nr = k.kategorijos_id "
        "WHERE pa.kliento_nr = %d;", 
        userID);

    printQueryResults(conn, query, getMessage(MSG_NO_ORDERS));
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

void addNewOrder(PGconn *conn, int userID, int productID, int quantity)
{
    printf("userID: %d\n", userID);
    printf("productID: %d\n", productID);
    printf("quantity: %d\n", quantity);
    PQexec(conn, "BEGIN;");

    // Fetching product price
    char fetchPriceQuery[256];
    sprintf(fetchPriceQuery, "SELECT kaina FROM produktas WHERE produkto_id = %d;", productID);
    PGresult *res = PQexec(conn, fetchPriceQuery);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Nepavyko gauti produkto kainos: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }

    float price = atof(PQgetvalue(res, 0, 0));
    float pajamos = price * quantity;
    PQclear(res);

    char insertOrderQuery[256];
    sprintf(insertOrderQuery, "INSERT INTO pardavimas (kliento_nr, pardavimo_data, pajamos) VALUES (%d, CURRENT_TIMESTAMP, %f) RETURNING pardavimo_id;", userID, pajamos);
    printf("%s\n", insertOrderQuery);
    res = PQexec(conn, insertOrderQuery);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Insert into pardavimas failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }

    printf("Pardavimas pridetas!\n");
    // Retrieve the new pardavimo_id
    int newPardavimoId = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);

    printf("Pardavimo ID: %d\n", newPardavimoId);
    // Insert a corresponding row into pardavimo_informacija
    char insertPardavimoInfoQuery[256];
    sprintf(insertPardavimoInfoQuery, "INSERT INTO pardavimo_informacija (pardavimo_nr, produkto_nr, kiekis) VALUES (%d, %d, %d);", newPardavimoId, productID, quantity);
    res = PQexec(conn, insertPardavimoInfoQuery);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Insert into pardavimo_informacija failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQexec(conn, "ROLLBACK;");
        return;
    }
    PQclear(res);

    // Commit the transaction
    PQexec(conn, "COMMIT;");

}

void updateProductPrice(PGconn *conn, int productID, float price)
{
    char updateQuery[256];
    sprintf(updateQuery, "UPDATE produktas SET kaina = %f WHERE produkto_id = %d;", price, productID);
    printInsertResult(conn, updateQuery, "Produkto kaina atnaujinta!");
}

void changeUserName(PGconn *conn, int userID, char *name, char *surname)
{
    char *fullName = malloc(strlen(name) + strlen(surname) + 1);
    strcpy(fullName, name);
    strcat(fullName, " ");
    strcat(fullName, surname);

    char updateQuery[256];
    sprintf(updateQuery, "UPDATE klientas SET vardas_pavarde = '%s' WHERE kliento_id = %d;", fullName, userID);
    printInsertResult(conn, updateQuery, "Vartotojo vardas atnaujintas!");
    free(fullName);
}

void registerNewUser(PGconn *conn, char *name, char *surname)
{
    char *fullName = malloc(strlen(name) + strlen(surname) + 1);
    strcpy(fullName, name);
    strcat(fullName, " ");
    strcat(fullName, surname);

    char query[256];
    sprintf(query, "INSERT INTO klientas (vardas_pavarde) VALUES ('%s');", fullName);
    printInsertResult(conn, query, "Vartotojas pridetas!");
    free(fullName);
}

void deleteUserData(PGconn *conn, int userID)
{
    char query[256];
    sprintf(query, "DELETE FROM klientas WHERE kliento_id = %d;", userID);
    printInsertResult(conn, query, "Vartotojas istrintas!");
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

bool isValidProduct(PGconn *conn, int productID)
{
    char query[100];
    sprintf(query, "SELECT EXISTS(SELECT 1 FROM produktas WHERE produkto_id = %d);", productID);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return false;
    }

    if (strcmp(PQgetvalue(res, 0, 0), "t") == 0) {
        return true;
    } else {
        printf("Tokio produkto nera. Bandykite dar karta. \n");
        return false;
    }
}

int loginCheck(PGconn *conn, char *name, char *surname)
{
    char *fullName = malloc(strlen(name) + strlen(surname) + 1);
    strcpy(fullName, name);
    strcat(fullName, " ");
    strcat(fullName, surname);

    char query[256];
    sprintf(query, "SELECT kliento_id FROM klientas WHERE lower(vardas_pavarde) = lower('%s');", fullName);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return -1;
    }

    if (PQntuples(res) == 0) {
        return -1;
    } else {
        return atoi(PQgetvalue(res, 0, 0));
    }
}

char *getNameByID(PGconn *conn, int id)
{
    char query[256];
    sprintf(query, "SELECT vardas_pavarde FROM klientas WHERE kliento_id = %d;", id);
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return NULL;
    }

    return PQgetvalue(res, 0, 0);
}
