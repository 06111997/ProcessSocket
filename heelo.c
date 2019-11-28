/* fscanf example */
#include <stdio.h>
#include "lib.h"
#include <sys/stat.h>
#include <fcntl.h>
char user[20];
char pass[20];
FILE *file_account;
int open_account()
{
int i=0;
  do{
    memset(user, 0, 20);
    memset(pass, 0, 20);
    gets(user);
    gets(pass);
   fprintf(file_account, "%-19s %-19s\n", user,pass);
    i++;
  }while(i<20);
  return i;
}
int main ()
{
  file_account = fopen("./account", "r+");

  open_account();
  fclose(file_account);
  return 0;
}