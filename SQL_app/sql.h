#ifndef SQL_H_INCLUDED
#define SQL_H_INCLUDED

#include <libpq-fe.h>
#include <stdbool.h>

// Connection functions
void exit_nicely(PGconn *conn);
PGconn *connectToDatabase();

// Display functions
void displayUserList(PGconn *conn);
void displayCategoryList(PGconn *conn);
void displayProductList(PGconn *conn);
void displayOrderList(PGconn *conn, int userID);

// Insert functions
void addNewCategory(PGconn *conn, char *categoryName);
void addNewProduct(PGconn *conn, int categoryID, char *productName, float price, int quantity);
void addNewOrder(PGconn *conn, int userID, int productID, int quantity);
void registerNewUser(PGconn *conn, char *name, char *surname);

// Update functions
void updateProductPrice(PGconn *conn, int productID, float newPrice);
void changeUserName(PGconn *conn, int userID, char *name, char *surname);

// Delete functions
void deleteUserData(PGconn *conn, int userID);

// Printing functions
void printQueryResults(PGconn *conn, const char *query, const char *noResultsMessage);
void printInsertResult(PGconn *conn, const char *query, const char *text);

// Helper functions
bool isValidCategory(PGconn *conn, int categoryID);
bool isValidProduct(PGconn *conn, int productID);
int loginCheck(PGconn *conn, char *name, char *surname);
char *getNameByID(PGconn *conn, int id);

#endif