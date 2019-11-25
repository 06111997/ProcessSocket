#pragma once
#define USER_ENTER "UENTER"
#define USER_OK "UOK"
#define USER_ERROR "UERROR"
#define MESS_CLIENT_TO_SERVER "MESS"
#define ERROR_SEND "ERROR SEND"
#define ERROR_RECV "ERROR RECEIVE"

typedef struct Data
{
  char cmd[10];
  char user[10];
  char string[1024];
} Data;
typedef struct UserSocket
{
    char user[10];
    int socket;
    struct sockaddr addr;

} UserSocket;
