#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
int status;

typedef struct flags
{
	bool flag_r,
			flag_h,
			flag_v,
			flag_o,
			md5,
			sha1,
			sha256;
	char outputfile[255];
} flags;

flags info;

#define die(e)                  \
	do                            \
	{                             \
		fprintf(stderr, "%s\n", e); \
		exit(EXIT_FAILURE);         \
	} while (0);

long get_current_time_with_ms(void)
{
	long ms;	// Milliseconds
	time_t s; // Seconds
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	s = spec.tv_sec;
	ms = (spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
	if (ms > 999)
	{
		s++;
		ms = 0;
	}
	return ms;
}

void getName(char file_name[])
{
	int link[2];
	fflush(NULL);
	pid_t pid = fork();

	if (pipe(link) == -1)
		die("pipe");

	if ((pid = fork()) == -1)
		die("fork");

	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		char *cmd = "file";
		char *argvs[3];
		argvs[0] = "file";
		argvs[1] = "-F,";
		argvs[2] = file_name;
		argvs[3] = NULL;
		execvp(cmd, argvs);
		die("execlp");
	}
	else
	{
		close(link[1]);
		read(link[0], name, sizeof(name));
		//printf("%.*s",namebytes,name);
	}
}

void getInfo(char file_name[])
{
	long start, stop;
	start = get_current_time_with_ms();
	int link[2];
	pid_t pid = fork();

	if (pipe(link) == -1)
		die("pipe");

	if ((pid = fork()) == -1)
		die("fork");

	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		char *cmd = "stat";
		char *argvs[3];
		argvs[0] = "stat";
		argvs[1] = "--format= %s, %A, %w, %z";
		argvs[2] = file_name;
		argvs[3] = NULL;
		execvp(cmd, argvs);
		die("execlp");
	}
	else
	{
		char foo[4096];
		close(link[1]);
		read(link[0], infor, sizeof(infor));
		setbuf(stdout, NULL);
		//printf("%.*s",inforbytes,infor);

		if (info.flag_v)
		{
			stop = get_current_time_with_ms();
			long sub = stop - start;
			printf("%ld miliseconds\n", sub);
			printf("PID: %d \n", pid);
		}
	}
}

void getHash(char file_name[], char hash_command[])
{
	int link[2];
	pid_t pid = fork();

	if (pipe(link) == -1)
		perror("pipe error");

	if ((pid = fork()) == -1)
		perror("fork error");

	if (pid == 0)
	{
		dup2(link[1], STDOUT_FILENO);
		close(link[0]);
		close(link[1]);
		char *cmd = hash_command;
		char *argvs[3];
		argvs[0] = hash_command;
		argvs[1] = file_name;
		argvs[2] = NULL;
		execvp(cmd, argvs);
		die("execlp");
	}
	else
	{
		close(link[1]);
		read(link[0], hex, sizeof(hex));
	}
}

void printArraysHash(int time)
{
	bool changed = true;
	// for cicle to format the char arrays to match desired output
	for (size_t i = 0; i < 4096; i++)
	{
		if (name[i] == '\n')
		{
			name[i] = 0;
		}
		if (infor[i] == '\n')
		{
			infor[i] = 0;
		}
		if (infor[i] == '+')
		{
			infor[i] = 0;
		}
		if (infor[i - 1] > 47 && infor[i + 1] < 58 && infor[i] == ' ' && changed)
		{
			infor[i] = 'T';
			changed = false;
		}
		if (hex[i] == '\n')
		{
			hex[i] = 0;
		}
		if (hex[i] == ' ')
		{
			hex[i] = 0;
		}
	}

	printf("%s, %s, %s\n", name, infor, hex);
	// sleep is called to be certain that the output is fully printed before the program ends
	sleep(time);
}

void printArrays(int time)
{
	bool changed = true;
	// for cicle to format the char arrays to match desired output
	for (size_t i = 0; i < 4096; i++)
	{
		if (name[i] == '\n')
		{
			name[i] = 0;
		}
		if (infor[i] == '\n')
		{
			infor[i] = 0;
		}
		if (infor[i] == '+')
		{
			infor[i] = 0;
		}
		if (infor[i - 1] > 47 && infor[i + 1] < 58 && infor[i] == ' ' && changed)
		{
			infor[i] = 'T';
			changed = false;
		}
	}

	printf("%s, %s\n", name, infor);
	// sleep is called to be certain that the output is fully printed before the program ends
	sleep(time);
}

// Function to get the different informations of the file
void getFileInfo(char file_name[])
{
	// gets name and type of file
	getName(file_name);
	// gets size, access, creation date and modification date
	getInfo(file_name);
}

// Function to get the different informations of the file with hash from md5sum, sha256sum or sha1sum
void getFileInfoHash(char file_name[], char hash_command[])
{
	// gets name and type of file
	getName(file_name);
	// gets size, access, creation date and modification date
	getInfo(file_name);
	// gets hexadecimal value of either md5sum, sha256sum or sha1sum
	getHash(file_name, hash_command);
}

void main(int argc, char *argv[])
{
	//forensic -o sha1,sha256 a.txt
	char file_name[256];
	char hash_command[256];
	strncpy(file_name, argv[3], 256);
	if (argv[2] != NULL)
	{
		strncpy(hash_command, argv[2], 256);
		getFileInfoHash(file_name, hash_command);
		printArraysHash(1);
	}
	else
	{
		getFileInfo(file_name);
		printArrays(1);
	}
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
