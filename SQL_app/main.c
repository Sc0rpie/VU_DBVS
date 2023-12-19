#include <stdio.h>
#include <stdlib.h>

#include "menu.h"
#include "sql.h"

int main()
{
    PGconn *conn;

    conn = connectToDatabase();
    // clearScreen();
    displayMenu(conn);
    return 0;
}
