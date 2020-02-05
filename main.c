#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "queue.h"

void main(int argc, char *argv[]){
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;

	Queue *queue = malloc(sizeof(struct Queue));
	setupQueue(queue);
	printf("is queue empty: %d\n", isEmpty(queue));
	printf("Adding to queue\n");
	enqueue(queue, 4);
	enqueue(queue, 5);
	printf("is queue empty: %d\n", isEmpty(queue));
	
	node *node = dequeue(queue);
	printf("Dequeued value: %d\n", node->value);
	opterr = 1;
	int hFlag = 0;
	int LFlag = 0;
	int dFlag = 0;
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
	dirp = opendir(argv[optind]);
	printf("argv[1] is %s\n", argv[1]);
	if (stat(argv[optind], &statbuf) == 0)
		printf("Is directory: %d\n", S_ISDIR(statbuf.st_mode));
	while ((direntp = readdir(dirp)) != NULL){
		//if(direntp->d_type==DT_UNKNOWN){
		if(stat(direntp->d_name, &statbuf) == 0){
			printf("%s Type: %u IsDirectory: %d\n ", direntp->d_name, direntp->d_type, S_ISDIR(statbuf.st_mode));
		}
		else{
			fprintf(stderr, "Failed to check file name: %s", direntp->d_name);
			perror("Error: call to stat failed\n");
		}
		//}
	}
}
