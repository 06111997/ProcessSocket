/* fscanf example */
#include <stdio.h>
#include "lib.h"
#include <sys/stat.h>
#include <fcntl.h>
void convert_line(char*user, char*data,const char* line){
  sscanf(line,"%10s",user);
  strcpy(data,line+11);
}
int main ()
{
  char user[10];
  char data[1024];
  char *line=NULL;
  float f;
  int a;
  FILE * pFile;

  pFile = fopen ("luong.txt","w+");
  
  while (getline(&line,&a,pFile)!=-1)
  {   printf("luong\n");
      
       printf("%s",line);
       memset(user,0,10);
       memset(data,0,1024);
      convert_line(user,data,line);
      printf("%s:%s",user,data);
  }
  fseek(pFile, 200, SEEK_END);
  for(int i=0;i<10;i++)
  {
      fprintf(pFile,"%d : luong: pham\n",i);
  }
 
  
  
  fclose (pFile);
  
  return 0;
}