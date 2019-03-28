
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct flags{
  bool flag_r,
       flag_h,
       flag_v,
       flag_o,
       md5,
       sha1,
       sha256;
  char outputfile[50];
}flags;

flags info;


int file_handler(char *file_path, int md5, int sha1, int sha256) //retieves the information of one file
{

    return 1;
}

int check_flags(int number_arguments, char *argv[])
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
        i++;
        if

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

}
