/*zombie.c*/
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char *argv[])
{    
 pid_t pid;
 char *message;
 int n;
 int i;
/*create new process*/
 pid = fork();
 if(pid<0){
   printf("Error!\n");
   exit(-1);
 }


 /*parent process*/
 if(pid)
 {
    n=2;
    message="parent\n";
 }
 /*child process*/
 if(!pid)
 {
    n=1;
    message="child\n";
 }
 
 for(i=0;i<n;i++){
   printf("%s",message);
   sleep(3);
 }
 exit(0);
 }

