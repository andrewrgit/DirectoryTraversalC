//Author: Andrew Riebow
//Title: Directory Searcher
//Email: asrm8d@mail.umsl.edu
//Date completed: 02/10/2020

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>
#include "queue.h"

int main(int argc, char *argv[]){
	//Create buffers to hold our dirent and stat structs, and a directory stream for the open directory
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;

	//Create our queue and call our initialization method
	Queue *queue = malloc(sizeof(struct Queue));
	setupQueue(queue);
	
	//Create a pointer to our executable with the ./ stripped out for error messages
	char *execName = basename(argv[0]);

	//Set opterr to 1 so error messages are displayed when getopt runs into an issue
	opterr = 1;

	//Create a variable for each flag so we know if that option was entered. 0 = false, 1 = true
	int hFlag = 0;
	int LFlag = 0;
	int dFlag = 0;
	int gFlag = 0;
	int iFlag = 0;
	int pFlag = 0;
	int sFlag = 0;
	int tFlag = 0;
	int uFlag = 0;
	int lFlag = 0;
	
	//Create strings to hold data about the file that we can then append to our final output string
	char *LString;
	char *dString;
	char *gString;
	char *iString;
	char *tString;
	int option; //Variable to hold our option
	
	//use get opt to set flags depending on which options were selected
	while ((option = getopt (argc, argv, "hLdgipstul")) != -1){
		switch(option){
			case 'h':
				hFlag = 1;
				break;
			case 'L':
				LFlag = 1;
				break;
			case 'i':
				iFlag = 1;
				break;
			case 'd':
				dFlag = 1;
				break;
			case 'u':
				uFlag = 1;
				break;
			case 'g':
				gFlag = 1;
				break;
			case 's':
				sFlag = 1;
				break;
			case 'p':
				pFlag = 1;
				break;
			case 'l':
				lFlag = 1;
				break;
			case 't':
				tFlag = 1;
				break;

		}
	}
	
	//if h option is input, output a help message and exit. Or, if there is more than 1 argument left after the options have been stripped out
	if (hFlag == 1 || argc - optind > 1){
		char *bufName = malloc(strlen(execName) + 1);
		strcpy(bufName, execName);
		printf("Usage: %s [directory_name] [-options]\n"
		"Options:\n"
		"	-h : Print help message\n"
		"	-L : Follow symbolic links\n"
		"	-t : Print file type\n"
		"	-p : Print permissions\n"
		"	-i : Print number of links in inode table\n"
		"	-u : Print the UID\n"
		"	-g : Print the GID\n"
		"	-s : Print file size\n"
		"	-d : Show time of last modification\n"
		"	-l : One option to count for -tipugsd options", bufName);
		exit(0);
	}
	//use our custom enqueue method to add our argument to the queue
	enqueue(queue, argv[optind]);
	while(isEmpty(queue) != 1){//Repeat until the queue is empty
		char *path = (dequeue(queue))->fullFilePath; //Set a pointer to the filepath dequeued from the queue

		if((dirp = opendir(path)) != NULL){ //Open a directory stream using our filepath we dequeued
			while ((direntp = readdir(dirp)) != NULL){//While there are entries left in the directory to read
				char *fullPath = malloc(strlen(path) + strlen(direntp->d_name) + 2); //Allocate memory for a string to hold the path, name of the file, and 2 extra bytes for a / and '0
				strcpy(fullPath, path);//Create our new filepath by appending the correct strings to the front of it
				strcat(fullPath, "/");
				strcat(fullPath, direntp->d_name);

				int statSucceeded = 0;//Create a 'bool' to hold whether or not our stat calls were successful
				if(LFlag == 1){//If the -L option to follow symbolic links is selected, we call lstat instead of stat
					if(lstat(fullPath, &statbuf) == 0){//Get information about the file, following symbolic links
						statSucceeded = 1;
					}
					else{
						fprintf(stderr, "Failed to check file name: %s with fullPath %s\n", direntp->d_name, fullPath);
						char *bufName = malloc(strlen(execName) + 1);
						strcpy(bufName, execName);
						perror(strcat(bufName, ": Error: Call to lstat failed\n"));
						statSucceeded = 0;
					}
				}
				else{
					if(stat(fullPath, &statbuf) == 0){//Call the normal stat if -L option was not entered
						statSucceeded = 1;
					}
					else{
						fprintf(stderr, "Failed to check file name: %s with fullPath %s\n", direntp->d_name, fullPath);
						char *bufName = malloc(strlen(execName) + 1);
						strcpy(bufName, execName);
						perror(strcat(bufName, ": Error: Call to stat failed\n"));
						statSucceeded = 0;
					}
				}

				if(statSucceeded == 1){//If our stat call was successful

					char outputString[600];//Create a string with sufficient memory to hold our output
					outputString[0] = '\0';//Reset the string 

					//Last modification date
					if(dFlag == 1 || lFlag == 1){//We also will output this if -l option is selected which will output all extra information
						
						dString = ctime(&statbuf.st_mtime);
						dString[strlen(dString)-1] = '\0'; //Remove newline character from ctime
						strcat(outputString, dString);
					}
					
					//File type information
					if(tFlag == 1 || lFlag == 1){
						if(S_ISREG(statbuf.st_mode)){
							tString = " Regular File  ";
						}
						else if(S_ISDIR(statbuf.st_mode)){
							tString = " Directory     ";
						}
						else if(S_ISLNK(statbuf.st_mode)){
							tString = " Symbolic Link ";
						}
						strcat(outputString, tString);
					}

					//Print permission bits
					if(pFlag == 1 || lFlag == 1){
						char pString[10]; //Allocate 10 bytes for permission string plus null terminator
						
						//Use bitwise operator on bits of the st_mode and bits of the macros for permission bits
						//Use ternary operator to determine if the expression is true, then set the corresponding
						//index in the char array to the correct character. Otherwise set it to a -
						pString[0] = (statbuf.st_mode & S_IRUSR) ? 'r' : '-';
						pString[1] = (statbuf.st_mode & S_IWUSR) ? 'w' : '-';
						pString[2] = (statbuf.st_mode & S_IXUSR) ? 'x' : '-';
						pString[3] = (statbuf.st_mode & S_IRGRP) ? 'r' : '-';
						pString[4] = (statbuf.st_mode & S_IWGRP) ? 'w' : '-';
						pString[5] = (statbuf.st_mode & S_IXGRP) ? 'x' : '-';
						pString[6] = (statbuf.st_mode & S_IROTH) ? 'r' : '-';
						pString[7] = (statbuf.st_mode & S_IWOTH) ? 'w' : '-';
						pString[8] = (statbuf.st_mode & S_IXOTH) ? 'x' : '-';
						pString[9] = '\0';
						strcat(outputString, pString);
					}
				
					//Print number of links from the inode table
					if(iFlag == 1 || lFlag == 1){
						char numOfLinks[10];
						numOfLinks[0] = '\0';
						sprintf(numOfLinks, "%d", statbuf.st_nlink);
						strcat(outputString, " Num of links: ");
						strcat(outputString, numOfLinks);
						if(strlen(numOfLinks) == 2){
							strcat(outputString, " ");
						}else if(strlen(numOfLinks) == 1){
							strcat(outputString, "  ");
						}
					}

					//Print the UID
					if(uFlag == 1 || lFlag == 1){
						char UID[100];
						UID[0] = '\0';
						sprintf(UID, " UID: %u", statbuf.st_uid);
						strcat(outputString, UID);
					}
		
					//Print the GID
					if(gFlag == 1 || lFlag == 1){
						char GID[100];
						GID[0] = '\0';
						sprintf(GID, " GID: %u ", statbuf.st_gid);
						strcat(outputString, GID);
					}

					//Print the file size
					if(sFlag == 1 || lFlag == 1){
						char size[100];
						size[0] = '\0';
						if(statbuf.st_size >= 1024 && statbuf.st_size < 1048576){
							int fileSize = statbuf.st_size/1024;
							sprintf(size, " Size: %dK ", fileSize);
						}else if(statbuf.st_size >= 1048576 && statbuf.st_size < 1073741824){
							int fileSize = statbuf.st_size/1048576;
							sprintf(size, " Size: %dM ", fileSize);
						}else if(statbuf.st_size >= 1073741824){
							int fileSize = statbuf.st_size/1073741824;
							sprintf(size, " Size: %dG ", fileSize);
						}else{
						sprintf(size, " Size: %d ", statbuf.st_size);
						}
						strcat(outputString, size);

						//In this if statement, I append extra spaces to the end of the size file if it is shorter than normal, so the output stays formatted well
						if(strlen(size) <= 15 && strlen(size) > 8){
							int i;
							for (i = 0; i < (int)(13 - strlen(size)); i++){
								strcat(outputString, " ");
							}
						}
					}
					printf("%s %s\n", outputString, fullPath);//Output our final string for this file
					
					//If the file we are looking at is a directory, enqueue it
					if(S_ISDIR(statbuf.st_mode) == 1){
						if(strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0){
							enqueue(queue, fullPath);
						}
					}
				}
			}
			closedir(dirp);//Close the directory stream
		}
		else{
			fprintf(stderr, "Failed to open directory %s\n", path);
			char *bufName = malloc(strlen(execName) + 1);
			strcpy(bufName, execName);
			perror(strcat(bufName, ": Error: Call to opendir failed"));
		}
	}
	return 0;//Return 0 for a successful execution
}
