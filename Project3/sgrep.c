/* sgrep.c - working solution
 * Author: Joshua Fawcett
 * Usage: ./sgrep [OPTIONS] STRING [FILE ...]
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#define UNUSED __attribute__((unused))

typedef struct FileTuple{
	FILE *file;
	char filename[256];
} fileTuple;

int findString(int argc, char *argv[]) { 
	/* find the index position of the
	* STRING argument if there is one*/

	int isflag = 0;
	int flagPosition = 0;
	int x;
	for (x = 0; x < argc; x++) {
		if (argv[x][0] == '-') {
			isflag = 1;
			flagPosition = x;
		}
		if ((isflag != 0) && (x > flagPosition)){
			return x;
		}
	}
	return 1;
}

int sgrep(int ifcase, int invert, int count, char *string, fileTuple files[], int file_count) {
	int N = 0;
	int matches;
	UNUSED char tomatch[1024];
	char prefix[256];
	char buf[1024];
	int nl = 0;
	char *p;
	char *lines[5000];
	
	int i;
	for (i = 0; files[i].file != NULL; i++){
		N = 0;
		nl = 0;
		if (file_count > 1) {
			strcpy(prefix, files[i].filename);
			strcat(prefix, ":");
		}			
		else {
			prefix[0] = '\0';
		}
		while (fgets(buf, 1028, files[i].file) != NULL) {
			p = (char *)malloc(strlen(buf) + 1);
			strcpy(p, buf);
			lines[nl++] = p;
		}
		int x;
		for (x = 0; x < nl; x++) {
			char tomatch[1028] = {"a"};

			if (ifcase) {
				strcpy(tomatch, lines[x]);
			}
			else {
				int y;
				for (y = 0; lines[x][y] != '\0'; y++){
					tomatch[y] = tolower(lines[x][y]);
				}
			}
			if (invert) {
				matches = (strstr(tomatch, string) == NULL);
			}
			else {
				matches = (strstr(tomatch, string) != NULL);
			}
			if (matches) {
				if (count) {
					N++;
				}
				else {
					printf("%s%s", prefix, lines[x]);
				}
			}
		}
		if (strcmp(files[i].filename, "STANDARD INPUT") != 0) {
			fclose(files[i].file);
		}
		if (count) {
			printf("%s%i\n", prefix, N);
		}
		for (x = 0; x < nl; x++){
			free((void *)lines[x]);
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	int ifcase = 1; // -i flag 
	int invert = 0; // -v flag
	int count = 0;  // -c flag
	int x = 0;
	char *string;

	int i = 1;
	if (argc == 1){
		printf("Usage: ./sgrep [OPTIONS] STRING [FILE ...]\n");
		return 0;
	}

	while (i < argc){
		if (argv[i][0] != '-'){
			i++;
			continue;
		}
		if (strstr(argv[i], "i") != NULL){
			ifcase = 0;
		}
		if (strstr(argv[i], "v") != NULL){
			invert = 1;
		}
		if (strstr(argv[i], "c") != NULL){
			count = 1;
		}
		i++;
		if (i >= argc){
			printf("Invoked without the required STRING parameter\n");
			return 0;
		}
	}

	int str_index = findString(argc, argv);
	if (ifcase){
		string = &argv[str_index][0];
	}
	else {
		string = &argv[str_index][0];
		for (x = 0; string[x] != '\0'; x++){
			string[x] = tolower(string[x]);
		}
	}

	fileTuple files[256];
	int j = 0;
	for (i = (str_index + 1); i < argc; i++){
		FILE *fd = fopen(argv[i], "r");
		if (fd != NULL){
			files[j].file = fd;
			strcpy(files[j].filename, argv[i]);
			j++;
		}
		else {
			printf("%s %s \n", "FILE DOESNT EXIST", argv[i]);
		}
	}
	if (j == 0) {
		files[j].file = stdin;
		strcpy(files[j].filename, "STANDARD INPUT");
	}
	sgrep(ifcase, invert, count, string, files, j);
}
