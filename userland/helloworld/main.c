#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc,char **argv){
	int number;

	printf("Z-NIX booted, hello world! \n");
	printf("What is your favourite number? ");
	scanf("%s", &number);
	return 0;
}