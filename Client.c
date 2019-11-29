#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lib.h"
#include <errno.h>
#include <signal.h>
#include<ncurses.h>
#define PORT 1500
#define SERVER "127.0.0.1"

int main()
{
  int y_rcv=1;
  int client, ret;
  struct sockaddr_in server;
  char buffer[1024];
  char cmd[10];
  Data data;
  pid_t send_pid, rcv_pid;
  char me[10];
  

  client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0)
  {
    printf("Error in connection.\n");
    exit(EXIT_FAILURE);
  }
  printf("Client Socket %d is created.\n", client);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(SERVER);

  ret = connect(client, (struct sockaddr *)&server, sizeof(server));

  if (ret < 0)
  {
    printf("Error in connection.\n");
    exit(EXIT_FAILURE);
  }
  printf("Connected to Server.\n");
  while (0 == 0)
  {
    printf("luong\n");
    memset((void *)&data, 0, sizeof(Data));
    ret = recv(client, &data, sizeof(data), 0);printf("line %d\n",__LINE__);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_SEND, errno);
      close(client);
      exit(-1);
    }
    if (0== ret)
    {
      printf("Disconnect\n");
      close(client);
      exit(-1);
    }
    if (0 == strcmp(data.cmd, USER_ENTER))
    {

      memset((void *)&data, 0, sizeof(Data));
      strcpy(data.cmd, USER_ENTER);
      printf("Enter user :");
      gets(data.user);
      printf("Enter password :");
      gets(data.string);

      ret = send(client, &data, sizeof(data), 0);
      if (-1 == ret)
      {
        printf("%s: %d\n", ERROR_SEND, errno);
        close(client);
        exit(-1);
      }
      if (0== ret)
    {
      printf("Disconnect\n");
      close(client);
      exit(-1);
    }
    }
    else
    {
      if (0 == strcmp(data.cmd, USER_OK))
      {
        printf("Login Successful.\n");
        sleep(1);
        break;
      }
      if (0 == strcmp(data.cmd, USER_ERROR))
      {
        printf("Enter wrong 3 times.\n");

        exit(-1);
      }
    }
 
  }

  system("clear");printf("line %d\n",__LINE__);
  memset(me, 0, sizeof(me));
  strcpy(me, data.user);
  send_pid = fork();printf("line %d\n",__LINE__);
  switch (send_pid)
  {
  case -1:
  {
    printf("Can't create process");printf("line %d\n",__LINE__);
    exit(EXIT_FAILURE);
  }
  case 0:
  {

    while (0 == 0)
    {
      memset((void *)&data, 0, sizeof(Data));
      strcpy(data.cmd, MESS_CLIENT_TO_SERVER);
      strcpy(data.user, me);
      printf("Send:");printf("line %d\n",__LINE__);

      gets(data.string);
      ret = send(client, &data, sizeof(data), 0);
     
      if (-1 == ret)
      {
        printf("%s: %d\n", ERROR_SEND, errno);
        close(client);
        exit(EXIT_FAILURE);
      }
      if (0== ret)
    {
      printf("Disconnect\n");
      close(client);
      exit(-1);
    }
    system("clear");
    }
  }
  default:
  {
    rcv_pid = fork();
    switch (rcv_pid)
    {
    case -1:
    {
      printf("Can't create process");
      exit(EXIT_FAILURE);
    }
    case 0:
    {
      while (0 == 0)
      {
        memset((void *)&data, 0, sizeof(Data));
        ret = recv(client, &data, sizeof(data), 0);
        
        if (-1 == ret)
        {
          printf("%s: %d\n", ERROR_RECV, errno);
          close(client);
          exit(EXIT_FAILURE);
        }
        if (0== ret)
    {
      printf("Disconnect\n");
      close(client);
      exit(-1);
    }
        printf("%s : %s\n", data.user, data.string);
       
      }
    }
    default:
    {
      int status;
      pid_t pid1 = wait(&status);
      pid_t pid2 = (pid1 == send_pid) ? rcv_pid : send_pid;
      kill(pid2, SIGTERM);
      close(client);
    }
    }
  }
  }

  return 0;
}
