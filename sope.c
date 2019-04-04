#include <stdio.h>
#include <sys/types.h>
#include<sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <stdint.h>

#define BUF_LENG = 255;


char name[4096];
char infor[4096];
char hex[4096];
int namebytes, inforbytes, hexbytes;
int status;

typedef struct flags{
  bool flag_r,
       flag_h,
       flag_v,
       flag_o,
       md5,
       sha1,
       sha256;
  char outputfile[255];
}flags;

flags info;


#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

long get_current_time_with_ms (void){
    long            ms; // Milliseconds
    time_t          s;  // Seconds
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    s  = spec.tv_sec;
    ms = (spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
    if (ms > 999) {
        s++;
        ms = 0;
    }
  return ms;
}

void getName(int argc, char *argv[]){
  int link[2];
  pid_t pid = fork();

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    char *cmd = "file";
    char *argvs[3];
    argvs[0] = "file";
    argvs[1] = argv[1];
    argvs[2] = NULL;  
    execvp(cmd,argvs);
    die("execlp");

  } else {
    close(link[1]);
    int namebytes = read(link[0], name , sizeof(name));
    //printf("%.*s",namebytes,name);

  }
}

void getInfo(int argc, char *argv[]) {
  long start, stop;
  start = get_current_time_with_ms();
  int link[2];
  pid_t pid = fork();

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    char *cmd = "stat";
    char *argvs[3];
    argvs[0] = "stat";
    argvs[1] = "--format= %s, %A, %w, %z";
    argvs[2] = argv[1]; 
    argvs[3] = NULL; 
    execvp(cmd,argvs);
    die("execlp");

  } else {
    char foo[4096];
    close(link[1]);
    int inforbytes = read(link[0], infor, sizeof(infor));
    //printf("%.*s",inforbytes,infor);

    
    if(info.flag_v){
      stop = get_current_time_with_ms();
      long sub = stop - start;
      printf("%ld miliseconds\n",sub);
      printf("PID: %d \n",pid);
    }
  }
} 

void getHex(int argc, char *argv[]) {
  int link[2];
  pid_t pid = fork();

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {
    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    char *cmd = "md5sum";
    char *argvs[3];
    argvs[0] = "md5sum";
    argvs[1] = argv[1];
    argvs[2] = NULL; 
    execvp(cmd,argvs);
    die("execlp");

  } else {
    close(link[1]);
    int hexbytes = read(link[0], hex, sizeof(hex));
    //printf("%.*s",hexbytes,hex);
  }
} 

void printArrays(int time){

  for(size_t i = 0; i < 256; i++)
  {
    if( name[i] == '\n' ){ name[i] = 0; }
    if( infor[i] == '\n' ) { infor[i] = 0; }
    if( hex[i] == '\n' ) { hex[i] = 0; }
  }
  printf("%s, %s, %s \n",name,infor,hex);
  /*sleep(1);
  printf("%s",infor);
  sleep(1);
  printf("%s",hex);*/
}

void main(int argc, char *argv[]){
  //forensic -o sha1,sha256 a.txt
  getName(argc,argv);
  getInfo(argc,argv);
  getHex(argc,argv);
  printArrays(1);
   

}

/*
int file_handler(char *file_path, int md5, int sha1, int sha256) //retieves the information of one file
{

    return 1;
}

void check_flags(int number_arguments, char *argv[])
{

  info.flag_r = false,
  info.flag_h = false,
  info.flag_v = false,
  info.flag_o = false,
  info.md5 = false,
  info.sha1 = false,
  info.sha256 = false;


  for (int i = 0; i < number_arguments; i++) {
    
    if (argv[1][0] != '-'){
      perror("Input error\nExpected use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
      exit(-1);
    }

    switch (argv[i][2]) {
      case 'r':
        info.flag_r = true;
        break;

      case 'h':
        info.flag_h = true;
        forensicH();
        i++;
        break;

      case 'v':
        info.flag_v = true;
        break;

      case 'o':
        info.flag_o = true;
        i++;
        strcpy(info.outputfile, argv[i]);
        break;

      default:
        perror("Input error\nExpected use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
        exit(-1);
        break;

    }

  }

}


int main(int argc, char *argv[])
{
   
    if (argc < 1 || argc > 7){
      perror("Input error\nExpected use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
      exit(-1);
    }

    if (argc > 1){
      check_flags(argc-1, argv);
    }

}*/



