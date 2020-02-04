#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

void main(int argc, char *argv[]){
	struct dirent *direntp;
	DIR *dirp;
	struct stat statbuf;
	
	if (argc != 2){
		fprintf(stderr, "Usage: %s directory_name\n", argv[0]);
		return;
	}
	dirp = opendir(argv[1]);
	while ((direntp = readdir(dirp)) != NULL){
		if(direntp->d_type==DT_UNKNOWN){
		stat(direntp->d_name, &statbuf);
		printf("%s Type: %u Stat: %d\n ", direntp->d_name, direntp->d_type, &statbuf.st_uid);
		}
	}
}
