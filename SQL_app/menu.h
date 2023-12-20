#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "sql.h"

// Function prototypes
// Menus
void displayMenu(PGconn *conn);
void adminMenu(PGconn *conn);
void userLogin(PGconn *conn);
void registerUser(PGconn *conn);
void userMenu(PGconn *conn, int userID);

// Display functions
void displayUsers(PGconn *conn);
void displayCategories(PGconn *conn);
void displayProducts(PGconn *conn);
void displayOrders(PGconn *conn, int userID);
void displayLoyaltyPoints(PGconn *conn, int userID);

// Insert functions
void addCategory(PGconn *conn);
void addProduct(PGconn *conn);
void makeOrder(PGconn *conn, int userID);
void registerUser(PGconn *conn);

// Update functions
void updatePrice(PGconn *conn);
void changeName(PGconn *conn, int userID);

// Delete functions
void deleteUser(PGconn *conn, int userID);

// Helper functions
void clearScreen();
void waitForInput();
char *getStringInput();

#endif