#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "lib.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <errno.h>
#include "lib.h"
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_CLIENT 100

char flag[MAX_CLIENT];
FILE* log;

int len_account;
int count;
Data data;
int ret;
Account *account;
struct Node_UserSocket *add_usersocket(int socket, Account account)
{
  struct Node_UserSocket *new = malloc(sizeof(struct Node_UserSocket));
  if (new == NULL)
  {
    print("memory full!\n");
    return NULL;
  }
  new->next = NULL;
  new->pre = NULL;
  if (count == 0)
    head = new;
  tail->next = new;
  new->pre = tail;
  tail = new;
  count++;
  return new;
}
int remove_usersocket(struct Node_UserSocket *user)
{
  if (count == 0)
    return -1;
  struct Node_UserSocket *pre = user->pre;
  struct Node_UserSocket *next = user->next;
  pre->next = next;
  next->pre = pre;
  count--;
  if (user == tail)
    tail = pre;
  if (user == head)
    head = next;
  free(user);
  user = NULL;
  return 0;
}
void convert_line(char*user, char*data,const char* line){
  sscanf(line,"%10s",user);
  strcpy(data,line+11);
}
int compare_account(const Account *account1, const Account *account2)
{
  return strcmp(account1->username, account2->username) + strcmp(account1->pass, account2->pass);
}
int find_account(const Account acc)
{

  Account *ret = lfind((void *)&acc, account, sizeof(Account), &len_account, compare_account);
  if (ret == NULL)
    return -1;
  return (ret - account) / sizeof(Account);
}
void send_all(Dâ){}
void *thread_send_rcv(void *arg)
{
  int i = 0;
  int socket = (int)arg;
  int ret;
  Data data;
  Account tmp;
  do
  {
    memset((void *)&data, 0, sizeof(Data));
    strcpy(data.cmd, USER_ENTER);
    ret = send(socket, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_SEND, errno);
      close(socket);
      return;
    }
    memset((void *)&data, 0, sizeof(Data));
    ret = recv(socket, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_RECV, errno);
      close(socket);
      return;
    }
    strcpy(tmp.username, data.user);
    strcpy(tmp.pass, data.cmd);
    ret = find_account(tmp);
    memset((void *)&data, 0, sizeof(Data));
    if (ret > -1)
    {
      break;
    }
    i++;

  } while (i < 3);
  if (i == 3)
  {
    memset((void *)&data, 0, sizeof(Data));
    strcpy(data.cmd, CLOSE);
    ret = send(socket, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_SEND, errno);
      close(socket);
      return;
    }
    close(socket);
    return;
  }
  struct Node_UserSocket *new = add_usersocket(socket, tmp);
  memset((void *)&data, 0, sizeof(Data));
  ret = recv(socket, &data, sizeof(data), 0);
  if (-1 == ret)
  {
    printf("%s: %d\n", ERROR_RECV, errno);
    remove_usersocket(new);
    close(socket);
    return;
  }
  /*
  char *line=NULL;
  int n;
  fseek(log,0, SEEK_END);
  while (getline(&line,&n,log)!=-1)
  {
    memset((void *)&data, 0, sizeof(Data));
    strcpy(data.cmd,LOG);
    convert_line(data.user,data.string,line);

  }
  */

  while(0==0)
  {
  memset((void *)&data, 0, sizeof(Data));
  ret = recv(socket, &data, sizeof(data), 0);
  if (-1 == ret)
  {
    printf("%s: %d\n", ERROR_RECV, errno);
    remove_usersocket(new);
    close(socket);
    remove_usersocket(new);
    return 0;
  }
  for(int i=0;i<count;i++)
  {
    ret = send(socket, &data, sizeof(data), 0);
    if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_SEND, errno);
      close(socket);
      return;
    }
  } 
  }
}


int main()
{
  int server, ret;
  struct sockaddr_in server_addr;
  int client;
  struct sockaddr_in client_addr;
  socklen_t addr_size;
  server = socket(AF_INET, SOCK_STREAM, 0);
  if (server < 0)
  {
    printf("[-]Error in connection.\n");
    exit(1);
  }
  printf("[+]Server Socket is created.\n");

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  ret = bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (ret < 0)
  {
    printf("[-]Error in binding.\n");
    exit(1);
  }
  printf("[+]Bind to port %d\n", PORT);

  if (0 == listen(server, 10))
  {
    printf("[+]Listening....\n");
  }
  else
  {
    printf("[-]Error in binding.\n");
  }

  memset(user, 0, sizeof(user));
  memset(flag, 0, sizeof(flag));
  while (1)
  {
    if (MAX_CLIENT == count)
    {
      printf("Server full!\n");
      break;
    }
    client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
  }
}