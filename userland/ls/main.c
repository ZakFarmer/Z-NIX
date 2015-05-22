#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <os.h>
#include <file.h>
#include <filesystem.h>

int main(int argc, char **argv){
	//Scan for files.
	if (argc > 0){

		int i; //create a variable for the index.

		//create a container for the files.
		File[] f = getFilesInDir(argv[1]);

		//Show a message, stating the folder the ls is working.
		printf("Contents of: %s", argv[1]);

		//Loop through all files.
		for (i = 0; i < sizeof(f); i++){
			//Print out info about the file.
			printf("%s -- %s -- %s", f[i].getName(), f[i].getSize(), f[i].getType());
			//printf(f[i].getName() + " -- " + f[i].getSize() + " -- " + f[i].getType());
		}
	}
	else
	{
		printf("ls command, usage: ls (dir)");
	}
}
