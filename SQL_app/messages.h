#ifndef MESSAGES_H_INCLUDED
#define MESSAGES_H_INCLUDED

typedef enum {
    MSG_NO_USERS,
    MSG_NO_CATEGORIES,
    MSG_NO_PRODUCTS,
    MSG_NO_ORDERS,
    MSG_NO_ORDERDETAILS
} MessageType;

const char* getMessage(MessageType type);

#endif