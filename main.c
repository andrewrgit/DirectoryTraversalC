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

void main(int argc, char *argv[]){
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;

	Queue *queue = malloc(sizeof(struct Queue));
	setupQueue(queue);
	
	char *execName = basename(argv[0]);

	opterr = 1;
	int hFlag = 0;
	int LFlag = 0;
	int dFlag = 0;
	int gFlag = 0;
	int iFlag = 0;
	int pFlag = 0;
	int sFlag = 0;
	int tFlag = 0;
	int uFlag = 0;
	
	char *LString;
	char *dString;
	char *gString;
	char *iString;


	char *tString;
	int option;
	
	
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
	
			case 'p':
				pFlag = 1;
				break;

			case 't':
				tFlag = 1;
				break;

		}
	}
	
	if (hFlag == 1){
		printf("Usage: %s directory_name\n", argv[0]);
		exit(0);
	}
	
	if (argc - optind > 1){
		fprintf(stderr, " Usage: %s directory_name\n", argv[0]);
		return;
	}

	enqueue(queue, argv[optind]);
	while(isEmpty(queue) != 1){
		char *path = (dequeue(queue))->fullFilePath;
		//printf("Dequeued %s\n", path);
		if((dirp = opendir(path)) != NULL){
			while ((direntp = readdir(dirp)) != NULL){
				usleep(800);
				char *fullPath = malloc(strlen(path) + strlen(direntp->d_name) + 2);
				strcpy(fullPath, path);
				strcat(fullPath, "/");
				strcat(fullPath, direntp->d_name);
				//printf("Full Path is: %s\n", fullPath);
				if(stat(fullPath, &statbuf) == 0){

					char outputString[600];
					outputString[0] = '\0';
					if(dFlag == 1){
						
						dString = ctime(&statbuf.st_mtime);
						dString[strlen(dString)-1] = '\0'; //Remove newline character from ctime
						strcat(outputString, dString);
					}
					
					if(tFlag == 1){
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

					if(pFlag == 1){
						char pString[10]; //Allocate 10 bits for permission string plus null terminator
						
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
				
					if(iFlag == 1){
						char numOfLinks[10];
						numOfLinks[0] = '\0';
						sprintf(numOfLinks, "%d", statbuf.st_nlink);
						strcat(outputString, " Num of links: ");
						strcat(outputString, numOfLinks);
					}
					if(uFlag == 1){
						char UID[100];
						UID[0] = '\0';
						sprintf(UID, " UID: %u", statbuf.st_uid);
						strcat(outputString, UID);
					}
		
					if(gFlag == 1){
						char GID[100];
						GID[0] = '\0';
						sprintf(GID, " GID: %u ", statbuf.st_gid);
						strcat(outputString, GID);
					}
					printf("%s %s\n", outputString, fullPath);
					

					if(S_ISDIR(statbuf.st_mode) == 1){
						if(strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0){
							enqueue(queue, fullPath);
							//printf("Enqueued %s because direntp->d_name is %s\n", fullPath, direntp->d_name);
						}
					}
				}
				else{
					fprintf(stderr, "Failed to check file name: %s with fullPath %s\n", direntp->d_name, fullPath);
					perror(strcat(execName, ": Error: Call to stat failed\n"));
				}
			}
			closedir(dirp);
		}
		else{
			fprintf(stderr, "Failed to open directory %s\n", path);
			perror("Error: failed to open directory");
		}
	}
}
