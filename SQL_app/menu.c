#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "menu.h"

int main()
{
    clearScreen();
    displayMenu();
    return 0;
}

void displayMenu()
{
    printf("1. Prisijungti kaip administratorius\n");
    printf("2. Prisijungti kaip vartotojas\n");
    printf("3. Registruotis kaip vartotojas\n");
    printf("4. Iseiti\n");

    printf("Pasirinkite: ");

    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        adminMenu();
        break;
    case 2:
        // userLogin();
        break;
    case 3:
        // registerUser();
        break;
    default:
        exit(0);
        break;
    }
}

void adminMenu() 
{
    clearScreen();

    printf("1. Perziureti vartotojus\n");
    printf("2. Prideti kategorija\n");
    printf("3. Prideti preke\n");
    printf("4. Perziureti preke(-es)\n");
    printf("5. Iseiti\n");

    printf("Pasirinkite: ");

    int choice;

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        clearScreen();
        displayUsers();
        break;
    case 2:
        clearScreen();
        // addCategory();
        break;
    case 3:
        clearScreen();
        // addProduct();
        break;
    case 4:
        clearScreen();
        // productsMenu();
        break;
    default:
        exit(0);
        break;
    }
}

void clearScreen()
{
    printf("\e[1;1H\e[2J");
}