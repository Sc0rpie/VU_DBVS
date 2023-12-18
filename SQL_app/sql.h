#ifndef SQL_H_INCLUDED
#define SQL_H_INCLUDED

#include <libpq-fe.h>

// Connection functions
void exit_nicely(PGconn *conn);
PGconn *connectToDatabase();

// Display functions
void displayUserList(PGconn *conn);
void displayCategoryList(PGconn *conn);
void displayProductList(PGconn *conn);

// Insert functions
void addNewCategory(PGconn *conn, char *categoryName);
void addNewProduct(PGconn *conn, int categoryID, char *productName, float price, int quantity);

// Printing functions
void printQueryResults(PGconn *conn, const char *query, const char *noResultsMessage);
void printInsertResult(PGconn *conn, const char *query, const char *text);

// Helper functions
bool isValidCategory(PGconn *conn, int categoryID);

#endif