#pragma once
#define USER_ENTER "UENTER"
#define USER_OK "UOK"
#define USER_ERROR "UERROR"
#define CLOSE "CLOSE"
#define MESS_CLIENT_TO_SERVER "MESS"
#define ERROR_SEND "ERROR SEND"
#define ERROR_RECV "ERROR RECEIVE"
#define LOG "LOG"
typedef struct Account
{
    char username[20];
    char pass[20];
} Account;

typedef struct Data
{
    char cmd[10];
    char user[10];
    char string[1024];
} Data;
typedef struct UserSocket
{
    int socket;
    Account account;

} UserSocket;

struct Node_UserSocket
{
    UserSocket user;
    struct Node_UserSocket *pre;
    struct Node_UserSocket *next;
};

