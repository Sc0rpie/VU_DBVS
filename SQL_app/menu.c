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
        userLogin(conn);
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

void userLogin(PGconn *conn)
{
    char *name;
    char *surname;
    int userID = -1;

    printf("Iveskite savo varda: ");
    name = getStringInput();

    printf("Iveskite savo pavarde: ");
    surname = getStringInput();

    while (userID == -1)
    {
        userID = loginCheck(conn, name, surname);
        if (userID == -1)
        {
            printf("Tokio vartotojo nera. Bandykite dar karta.\n");
            printf("Iveskite savo varda: ");
            name = getStringInput();

            printf("Iveskite savo pavarde: ");
            surname = getStringInput();
        }
    }
    waitForInput();
    userMenu(conn, userID);

}

void userMenu(PGconn *conn, int userID)
{
    clearScreen();
    char *fullName = getNameByID(conn, userID);

    printf("Sveiki, %s!\n", fullName);
    printf("1. Perziureti prekes\n");
    printf("2. Sukurti uzsakyma\n");
    printf("3. Perziureti padarytus uzsakymus\n");
    printf("4. Perziureti lojalumo tasku kieki\n");
    printf("5. Pakeisti varda\n");
    printf("6. Istrinti paskyra\n");
    printf("0. Iseiti\n");

    printf("Pasirinkite: ");
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            clearScreen();
            displayProductList(conn);
            waitForInput();
            userMenu(conn, userID);
            break;
        case 2:
            clearScreen();
            makeOrder(conn, userID);
            userMenu(conn, userID);
            break;
        case 3:
            displayOrders(conn, userID);
            // waitForInput();
            userMenu(conn, userID);
            // displayLoyaltyPoints(conn, userID);
            break;
        case 4:
            // displayLoyaltyPoints(conn, userID);
            break;
        case 5:
            changeName(conn, userID);
            break;
        case 6:
            deleteUser(conn, userID);
        default:
            exit(0);
            break;
    }
}

void makeOrder(PGconn *conn, int userID)
{
    printf("Pasirinkite produkta is saraso:\n");
    displayProductList(conn);

    int product;
    bool validProduct = false;
    while (!validProduct)
    {
        printf("Iveskite produkto numeri, kuri norite isigyti: ");
        scanf("%d", &product);
        validProduct = isValidProduct(conn, product);
    }

    printf("Iveskite produkto kieki: ");
    int quantity;
    scanf("%d", &quantity);

    addNewOrder(conn, userID, product, quantity);
    waitForInput();
    userMenu(conn, userID);
}

void changeName(PGconn *conn, int userID)
{
    clearScreen();
    char *name;
    char *surname;

    printf("Iveskite nauja varda: ");
    name = getStringInput();

    printf("Iveskite nauja pavarde: ");
    surname = getStringInput();

    changeUserName(conn, userID, name, surname);
    waitForInput();
    free(name);
    free(surname);
    userMenu(conn, userID);
}

void deleteUser(PGconn *conn, int userID)
{
    clearScreen();
    printf("Ar tikrai norite istrinti savo paskyra? (y/n)\n");
    char *choice = getStringInput();
    if (strcmp(choice, "y") == 0)
    {
        deleteUserData(conn, userID);
        waitForInput();
        displayMenu(conn);
    }
    else
    {
        userMenu(conn, userID);
    }
}

void displayOrders(PGconn *conn, int userID)
{
    clearScreen();
    displayOrderList(conn, userID);
    waitForInput();
    userMenu(conn, userID);
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