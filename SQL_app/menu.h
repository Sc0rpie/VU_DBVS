#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "sql.h"

// Function prototypes
// Menus
void displayMenu(PGconn *conn);
void adminMenu(PGconn *conn);
void userLogin(PGconn *conn);
void registerUser(PGconn *conn);
void userMenu(PGconn *conn);

// Display functions
void displayUsers(PGconn *conn);
void displayCategories(PGconn *conn);
void displayProducts(PGconn *conn);

// Insert functions
void addCategory(PGconn *conn);
void addProduct(PGconn *conn);
void registerUser(PGconn *conn);

// Update functions
void updatePrice(PGconn *conn);

// Helper functions
void clearScreen();
void waitForInput();
char *getStringInput();

#endif