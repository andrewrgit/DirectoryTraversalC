#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "queue.h"

void main(int argc, char *argv[]){
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;

	Queue *queue = malloc(sizeof(struct Queue));
	setupQueue(queue);
	
	opterr = 1;
	int hFlag = 0;
	int LFlag = 0;
	int dFlag = 0;
	int gFlag = 0;
	
	char *LString;
	char *dString;
	char *gString;
	int option;
	
	
	while ((option = getopt (argc, argv, "hLdgipstul")) != -1){
		switch(option){
			case 'h':
				hFlag = 1;
				break;
			case 'L':
				LFlag = 1;
				break;
			case 'd':
				dFlag = 1;
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

					char outputString[64];
					outputString[0] = '\0';
					if(dFlag == 1){
						
						dString = ctime(&statbuf.st_mtime);
						dString[strlen(dString)-1] = '\0'; //Remove newline character from ctime
						strcat(outputString, dString);
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
					perror("Error: call to stat failed\n");
				}
			}
			closedir(dirp);
		}
		else{
			fprintf(stderr, "Failed to open directory %s\n", path);
			perror("Error: failed to open directory");
		}
	}




	exit(0);
	printf("argv[1] is %s\n", argv[1]);
	if (stat(argv[optind], &statbuf) == 0)
		printf("Is directory: %d\n", S_ISDIR(statbuf.st_mode));
	else{
		perror("Error: Unable to open directory");
	}
	while ((direntp = readdir(dirp)) != NULL){
		//if(direntp->d_type==DT_UNKNOWN){
		char *fullPath = malloc(strlen(argv[optind]) + strlen(direntp->d_name) + 1);
		strcpy(fullPath, argv[optind]);
		strcat(fullPath, "/");
		strcat(fullPath, direntp->d_name);
		//printf("Full Path is: %s\n", fullPath);
		if(stat(fullPath, &statbuf) == 0){
			printf("%s Type: %u IsDirectory: %d\n ", fullPath, direntp->d_type, S_ISDIR(statbuf.st_mode));
		}
		else{
			fprintf(stderr, "Failed to check file name: %s", direntp->d_name);
			perror("Error: call to stat failed\n");
		}
		//}
	}
}
