
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_LENGTH 255

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
/*
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
    if (argv[i][0] != '-'){
      perror("Input error\nExpected use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
      exit(-1);
    }
    switch (argv[i][1]) {
      case 'r':
        info.flag_r = true;
        break;
      case 'h':
        info.flag_h = true;
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

}*/


bool forensicFolder(char fileDirectory[]) {
  return true;
}

/* Verifies the command structure for the -r */
bool attemptForensicFolder (int argc, char *argv[]) {
  const int MIN_LENGTH = 2;
  const int MAX_LENGTH = 3;
  const int FOLDER_INDEX = 3;
  if (argc >= MIN_LENGTH && argc <= MAX_LENGTH) {
    if (argc == MIN_LENGTH) {
      char defaultFolder[MAX_STRING_LENGTH];
      return forensicFolder(defaultFolder);
    }
    else if (argc == MAX_LENGTH) { return forensicFolder(argv[MAX_STRING_LENGTH]); }
  } else {
    perror("Invalid command structure");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 1 || argc > 7){
    perror("Input error\nExpected use: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>");
    exit(-1);
  }
  if (argc > 1){
    check_flags(argc-1, argv);
  }
}

setFlags (int argc, char *argv[]) {
  const int COMMAND = 1;
  const int COMMAND_SYMBOL = 0;
  const int COMMAND_NAME = 1;
  const int FLAG_ARGUMENT = 1;
  

  int argindex = 1;
  while (argindex < argc) {
    char commandSymbol = argv[argindex][COMMAND_SYMBOL];
    char commandName = argv[argindex][COMMAND_NAME];
    if (commandSymbol != '-') {
      perror('Command exited no command symbol found');
      exit(-1);
    }
    int indexJump = setFlag(commandName);
    if (indexJump == 2) {
      char nextArgument[MAX_STRING_LENGTH] = argv[argindex + FLAG_ARGUMENT];
      if (commandName == 'h') setFlagsH(nextArgument);
      if (commandName == 'o') setFlagsO(nextArgument);
    }
    argindex += indexJump;
  }
}

int setFlag (char commandName) {
  if (commandName == 'r') {
    info.flag_r = true;
    return 1;
  }
  else if (commandName == 'h') { 
    info.flag_h = true;
    return 2;
  }
  else if (commandName == 'v') info.flag_v = true;
  else if (commandName == 'o') info.flag_o = true;
  else if (commandName == 'm') info.md5 = true;
  else if (commandName == '1') info.sha1 = true;
  else if (commandName == '2') info.sha256 = true;
}
void setFlagsH (char * argument) {
  setFlag("md5");
}
void setFlagsO (char * argument) {

}
char getDefaultFolder() {
  // Do pwd
  char array = '/';
  return array;
}
