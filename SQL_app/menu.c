#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"

void displayMenu(PGconn *conn)
{
    clearScreen();
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
        adminMenu(conn);
        break;
    case 2:
        // userLogin();
        break;
    case 3:
        registerUser(conn);
        break;
    default:
        exit(0);
        break;
    }
}

void adminMenu(PGconn *conn) 
{
    clearScreen();

    printf("1. Perziureti vartotojus\n");
    printf("2. Perziureti kategorijas\n");
    printf("3. Prideti kategorija\n");
    printf("4. Perziureti produktus\n");
    printf("5. Prideti produkta\n");
    printf("6. Atnaujinti produkto kaina\n");
    printf("0. Iseiti\n");

    printf("Pasirinkite: ");

    int choice;

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        clearScreen();
        displayUsers(conn);
        break;
    case 2:
        clearScreen();
        displayCategories(conn);
        waitForInput();
        adminMenu(conn);
        break;
    case 3:
        clearScreen();
        addCategory(conn);
        break;
    case 4:
        clearScreen();
        displayProducts(conn);
        break;
    case 5:
        clearScreen();
        addProduct(conn);
        break;
    case 6:
        clearScreen();
        updatePrice(conn);
    default:
        exit(0);
        break;
    }
}

void displayUsers(PGconn *conn) 
{
    displayUserList(conn);
    waitForInput();
    clearScreen();
    adminMenu(conn);
}

void displayCategories(PGconn *conn)
{
    displayCategoryList(conn);
}

void displayProducts(PGconn *conn)
{
    displayProductList(conn);
    waitForInput();
    clearScreen();
    adminMenu(conn);
}

void addCategory(PGconn *conn)
{
    char *name;

    printf("Iveskite kategorijos pavadinima: ");

    name = getStringInput();

    addNewCategory(conn, name);
    waitForInput();
    adminMenu(conn);
}

void addProduct(PGconn *conn)
{
    printf("Pasirinkite produkto kategorija is saraso:\n");
    displayCategoryList(conn);

    int category;
    bool validCategory = false;
    while (!validCategory)
    {
        printf("Iveskite kategorijos numeri, kuri noretumet panaudoti: ");
        scanf("%d", &category);
        validCategory = isValidCategory(conn, category);
    }
    // scanf("%d", &category);

    printf("Iveskite produkto pavadinima: ");
    char *name;
    name = getStringInput();

    printf("Iveskite produkto kaina: ");
    float price;
    scanf("%f", &price);

    printf("Iveskite produkto kieki: ");
    int quantity;
    scanf("%d", &quantity);

    addNewProduct(conn, category, name, price, quantity);
    waitForInput();
    adminMenu(conn);
}

void updatePrice(PGconn *conn)
{
    printf("Pasirinkite produkta is saraso:\n");
    displayProductList(conn);

    int product;
    bool validProduct = false;
    while (!validProduct)
    {
        printf("Iveskite produkto numeri, kurio kaina bus atnaujinta: ");
        scanf("%d", &product);
        validProduct = isValidProduct(conn, product);
    }

    printf("Iveskite nauja produkto kaina: ");
    float price;
    scanf("%f", &price);

    updateProductPrice(conn, product, price);
    waitForInput();
    adminMenu(conn);
}

void registerUser(PGconn *conn)
{
    char *name;
    char *surname;

    printf("Iveskite savo varda: ");
    name = getStringInput();

    printf("Iveskite savo pavarde: ");
    surname = getStringInput();

    registerNewUser(conn, name, surname);
    waitForInput();
    displayMenu(conn);
}



void clearScreen()
{
    printf("\e[1;1H\e[2J");
}

void waitForInput() 
{
    fflush(stdin); // clear input buffer (stdin)
    printf("Press enter to continue...");
    while (getchar() != '\n');  // Wait for Enter key press
}

char *getStringInput()
{
    char *input = malloc(sizeof(char) * 255);
    fflush(stdin); // clear input buffer (stdin)
    fgets(input, 255, stdin);
    if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n'))
        input[strlen (input) - 1] = '\0';
    return input;
}