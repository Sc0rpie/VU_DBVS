#include "messages.h"

const char* getMessage(MessageType type)
{
    switch (type)
    {
        case MSG_NO_USERS:
            return "Vartotoju nera.\n";
        case MSG_NO_CATEGORIES:
            return "Kategoriju nera.\n";
        case MSG_NO_PRODUCTS:
            return "Produktu nera.\n";
        case MSG_NO_ORDERS:
            return "Uzsakymu nera.\n";
        case MSG_NO_ORDERDETAILS:
            return "Uzsakymu informacijos nera.\n";
        default:
            return "NaN";
    }
}