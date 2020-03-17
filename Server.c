
/*edit branch 3*/
/*edit on branch3*//*branch4*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include "lib.h"

#include "lib.h"

#define MAX_CLIENT 100
#define MAX_ACCOUNT 100
#define PORT 3000
#define ACCOUNT_FILE "./account"

int len_account;
int count = 0;
int ret;
Data data;
Account account_list[MAX_ACCOUNT];
char flag[MAX_CLIENT];
struct Node_UserSocket *head = NULL;
struct Node_UserSocket *tail = NULL;
int open_account()
{
    printf("read file\n");
    memset(account_list, 0, sizeof(account_list));
    FILE *file_account = fopen(ACCOUNT_FILE, "r+");
    if (NULL == file_account)
    {
        printf("Error open %s", ACCOUNT_FILE);
        return -1;
    }
    char *line;
    int num_char;
    int i = 0;

    while (getline(&line, &num_char, file_account) != -1)
    {
        if (0 == num_char)
            continue;
        fscanf(file_account, "%s %s", account_list[i].username, account_list[i].pass);
        printf("%s:%s\n", account_list[i].username, account_list[i].pass);
        i++;
    }

    fclose(file_account);
    printf("end read file:%d\n", i);
    return i;
}
struct Node_UserSocket *add_usersocket(int socket, Account account)
{
    struct Node_UserSocket *new = malloc(sizeof(struct Node_UserSocket));
    if (new == NULL)
    {
        printf("Memory full!\n");
        return NULL;
    }
    memset(new,0,sizeof(struct Node_UserSocket));
    strcpy(new->user.account.pass,account.pass);
    strcpy(new->user.account.username,account.username);
    new->user.socket=socket;
  
    count++;
    new->next = NULL;

    new->pre = NULL;
  
    if (count == 1)
    {
        head = new;
        tail = new;
        return new;
    }
    tail->next = new;
  
    new->pre = tail;
  
    tail = new;
  
    return new;
}
int remove_usersocket(struct Node_UserSocket *user)
{

    if (count == 0||user==NULL)
    {
      
        return -1;
    }
    
    if (!(user->pre)||!(user->next) )
    { 
        if (NULL ==user->next)
        {
            tail = user->pre;
            
        }
        if(NULL==user->pre)
        {
        head=user->next;
        }
    }
    else
    {
            struct Node_UserSocket *pre = user->pre;
            struct Node_UserSocket *next = user->next;
            printf("remove SocketUser.%d\n", count);
            pre->next = next;
            next->pre = pre;
 
    }

    count--;

    free(user);
    user = NULL;
    return 0;
}
void convert_line(char *user, char *data, const char *line)
{
    sscanf(line, "%10s", user);
    strcpy(data, line + 11);
}
int compare_account(const void *in1, const void *in2)
{
    Account *account1 = (Account *)in1;
    Account *account2 = (Account *)in2;
    return strcmp(account1->username, account2->username) + strcmp(account1->pass, account2->pass);
}
int find_account(Account acc)
{
    printf("%s:%s:%d", acc.username, acc.pass, __LINE__);
    size_t num = 20;
  
    Account *ret = lfind((const void *)&acc, (const void *)account_list, &num, sizeof(Account), compare_account);
    printf("%s:%s\n", acc.username, acc.pass);
    if (ret == NULL)
    {
        return -1;
    }
  
    return 0;
}
int send_all(struct Node_UserSocket *current, Data data)
{
    int ret;
    struct Node_UserSocket *tmp;
    tmp = head;
    while (NULL != tmp)
    {
        ret = send(tmp->user.socket, &data, sizeof(data), 0);
        if ((tmp->user.socket == current->user.socket)&&((ret==-1)||(ret==0)))
        {
            return -1;
        }
        tmp=tmp->next;
    }
    return 0;
}
static void *thread_send_rcv(void *arg)
{
    int count_enter = 0;
    int socket = *(int *)arg;
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
            return NULL;
        }
        memset((void *)&data, 0, sizeof(Data));
        ret = recv(socket, &data, sizeof(data), 0);
        if (-1 == ret)
        {
            printf("%s: %d\n", ERROR_RECV, errno);
            close(socket);
            return NULL;
        }
        strcpy(tmp.username, (const char *)data.user);
        strcpy(tmp.pass, (const char *)data.string);
        printf("%s:%s", tmp.pass, tmp.username);
      
        ret = find_account(tmp);
      
        memset((void *)&data, 0, sizeof(Data));
        if (ret > -1)
        {
            printf("have account !\n");
            break;
        }
        count_enter++;
      

    } while (count_enter < 3);
    printf("send broadcast\n");
    if (count_enter == 3)
    {
        memset((void *)&data, 0, sizeof(Data));
        strcpy(data.cmd, CLOSE);
        ret = send(socket, &data, sizeof(data), 0);
        if (-1 == ret)
        {
            printf("%s: %d\n", ERROR_SEND, errno);
            close(socket);
            return NULL;
        }
        printf(" close(socket)\n");
        close(socket);
        return NULL;
    }
    memset((void *)&data, 0, sizeof(Data));
    strcpy(data.user, tmp.username);
    strcpy(data.cmd, USER_OK);
    ret = send(socket, &data, sizeof(data), 0);
    if (-1 == ret)
    {
        printf("%s: %d\n", ERROR_SEND, errno);
        close(socket);
        return NULL;
    }
    printf("begin create a node new!\n");
    struct Node_UserSocket *new = add_usersocket(socket, tmp);
    printf("a node new!\n");
    while (0 == 0)
    {
        memset((void *)&data, 0, sizeof(Data));
        ret = recv(socket, &data, sizeof(data), 0);
        if (-1 == ret)
        {
            printf("%s: %d\n", ERROR_RECV, errno);
            remove_usersocket(new);
            close(socket);
            return NULL;
        }
        ret = send_all(new, data);
        if (-1 == ret)
        {
            printf("%s: %d\n", ERROR_SEND, errno);
            remove_usersocket(new);
            close(socket);

            return NULL;
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
        printf("Error in connection.\n");
        exit(EXIT_FAILURE);
    }
    printf("Server Socket is created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        printf("Error in binding.\n");
        exit(EXIT_FAILURE);
    }
    printf("Bind to port %d\n", PORT);
    if (0 == listen(server, MAX_CLIENT))
    {
        printf("Listening....\n");
    }
    else
    {
        printf("wait!\n");
    }
    ret = open_account();
    if (0 == ret)
    {
        printf("Server don't ready.\n");
        exit(EXIT_FAILURE);
    }
    if (0 > ret)
    {
        exit(EXIT_FAILURE);
    }
    

    while (0 == 0)
    {
        if (MAX_CLIENT == count)
        {
            printf("Server full!\n");
            break;
        }
        pthread_t thread_client;
        client = accept(server, (struct sockaddr *)&client_addr, &addr_size);
        pthread_create(&thread_client, NULL, thread_send_rcv, (void *)&client);
    }
}
