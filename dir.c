#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
	struct dirent *entry;

	if (argc != 2) return -1;
	// switch current working directory to dir
	// given on command line
	chdir(argv[1]);

	// open the current working directory
	DIR *current = opendir(".");

	// read entries and output based on type
	while( (entry = readdir(current)) ) {
		if (entry->d_type == DT_DIR) {
			printf("%s is a directory\n", entry->d_name);
		} else if (entry->d_type == DT_REG) {
			printf("%s is a regular file\n", entry->d_name);
		} else {
			printf("%s is idk\n", entry->d_name);
		}
	}

	return 0;
}