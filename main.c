#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char *argv[]){
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;
	
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
	printf("argv[1] is %s", argv[1]);
	while ((direntp = readdir(dirp)) != NULL){
		if(direntp->d_type==DT_UNKNOWN){
		stat(direntp->d_name, &statbuf);
		printf("%s Type: %u Stat: %d\n ", direntp->d_name, direntp->d_type, &statbuf.st_uid);
		}
	}
}
