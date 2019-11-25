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
#include<signal.h>
#include<ncurses.h>

#define PORT 1500
#define SERVER "10.72.0.10"

int main()
{
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
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Client Socket %d is created.\n", client);

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = inet_addr(SERVER);

  ret = connect(client, (struct sockaddr *)&server, sizeof(server));

  if (ret < 0)
  {
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Connected to Server.\n");
  while (0 == 0)
  {
    memset((void *)&data, 0, sizeof(Data));
    strcpy(data.cmd, USER_ENTER);
	printf("Enter user:");
    scanf("%s", data.user);
    

    ret = send(client, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_SEND, errno);
          close(client);
    }
    memset((void *)&data, 0, sizeof(Data));
    recv(client, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_RECV, errno);
          close(client);
      exit(-1);
    }

    if (strcmp(data.cmd, USER_OK));
      break;
  }
  system("clear");
  memset(me, 0, sizeof(me));
  strcpy(me, data.user);
      send_pid = fork();
  switch (send_pid)
  {
  case -1:
  {
    printf("Can't create process");
    exit(-1);
  }
  case 0:
  { 
	  attroff(A_BOLD);
    while (0 == 0)
    {
      memset((void *)&data, 0, sizeof(Data));
      strcpy(data.cmd, MESS_CLIENT_TO_SERVER);
      strcpy(data.user, me);
	  printf("\033[%d;%dHSend:", 50, 1);
     
      scanf("%s", data.string);
      ret = send(client, &data, sizeof(data), 0);
	   printf("%s : %s\n", data.user, data.string);
      if (-1 == ret)
      {
        printf("%s: %d\n", ERROR_SEND, errno);
            close(client);
            exit(-1);
      }
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
      exit(-1);
    }
    case 0:
    {
      while (0 == 0)
      {
        memset((void *)&data, 0, sizeof(Data));
        ret = recv(client, &data, sizeof(data), 0);
		printf("luong\n %d",ret);
        if (-1 == ret)
        {
          printf("%s: %d\n", ERROR_RECV, errno);
              close(client);
              exit(-1);
        }
        printf("%s : %s\n", data.user, data.string);
      }
    }
    default:
    {
      int status;
      pid_t pid1= wait(&status);
      pid_t pid2=(pid1==send_pid)?rcv_pid:send_pid;
      kill(pid2, SIGTERM);
      
    }
    }
  }
  }

  return 0;
}
