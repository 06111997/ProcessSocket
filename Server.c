#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include"lib.h"
#include<errno.h>
int main()
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  int result;
  int ret;
  fd_set readfds, testfds;
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(1500);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
  listen(server_sockfd, 5);
  FD_ZERO(&readfds);
  FD_SET(server_sockfd, &readfds);
  socklen_t addr_size;
  while (1){
       int newSocket = accept(server_sockfd, (struct sockaddr*)&client_address, &addr_size);
      while(1)
      {

           Data data;
        memset((void *)&data, 0, sizeof(Data));
        ret = recv(newSocket, &data, sizeof(data), 0);
         printf("rcv:%s :%s:%s",data.cmd,data.user,data.string);
         memset((void *)&data, 0, sizeof(Data));
        scanf("%s", data.cmd);
        scanf("%s", data.user);
        scanf("%s", data.string);
        ret = send(newSocket, &data, sizeof(data), 0);
        if (-1 == ret)
    {
      printf("%s: %d\n", ERROR_RECV, errno);
          close(newSocket);
          break;
     
    }
    else{
          printf("send to %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    }
      }


  }
//   {
//     char ch;
//     int fd;
//     int nread;
//     testfds = readfds;
//     printf("server waiting\n");
//     result = select(FD_SETSIZE, &testfds, (fd_set *)0,
//                     (fd_set *)0, (struct timeval *)0);
//     if (result < 1)
//     {
//       perror("server5");
//       exit(1);
//     }
//     for (fd = 0; fd < FD_SETSIZE; fd++)
//     {
//       if (FD_ISSET(fd, &testfds))
//       {
//         if (fd == server_sockfd)
//         {
//           client_len = sizeof(client_address);
//           client_sockfd = accept(server_sockfd,
//                                  (struct sockaddr *)&client_address, &client_len);
//           FD_SET(client_sockfd, &readfds);
//           printf("adding client on fd %d\n", client_sockfd);
//         }
//         else
//         {
//           ioctl(fd, FIONREAD, &nread);
//           if (nread == 0)
//           {
//             close(fd);
//             FD_CLR(fd, &readfds);
//             printf("removing client on fd %d\n", fd);
//           }
//           else
//           {
//             read(fd, &ch, 1);
//             sleep(5);
//             printf("serving client on fd %d\n", fd);
//             ch++;
//             write(fd, &ch, 1);
//           }
//         }
//       }
//     }
//   }
}