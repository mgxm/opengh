#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "lib/slre.h"

#ifndef GIT_CONFIG
#define GIT_CONFIG "/.git/config"
#endif 


struct slre        slre;
struct cap         captures[4 + 1];

int search_github_config(char *fullpath, const char *path);
void generate_project_url(char *file);

int main(int argc, char const *argv[])
{
	char currentDirectory[PATH_MAX+1];
	char fullpath[PATH_MAX+strlen(GIT_CONFIG)];

	getcwd(currentDirectory, PATH_MAX);

	if(search_github_config(fullpath, currentDirectory) == 0) {
		generate_project_url(fullpath);
	} else
		printf("%s\n", "The directory was not found");

	return 0;
}

/*
 * 0 - If Github config exists
 * 1 - If Github config doesn't exists
 */
int search_github_config(char *fullpath, const char *path)
{
	char tmp[(strlen(path)+strlen(GIT_CONFIG)+1)*sizeof(char)];

	strcat(tmp, path);
	strcat(tmp, GIT_CONFIG);

	if (access(tmp, F_OK) == 0)
	{
		strcpy(fullpath, tmp);
		return 0;
	}
	else
		return 1;
}

void generate_project_url(char *file)
{
	FILE *fp;
	long f_size;
	char *buffer;
	size_t result;

	
	if ((fp = fopen(file, "r")) == NULL) {
	    printf("Cannot open file.\n");
	    exit(1);
	} else {
		/* File size */
		fseek(fp, 0, SEEK_END);
		f_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		
		/* Copy the file into a string */
		buffer = malloc(f_size*sizeof(char));
		result = fread(buffer, 1, f_size, fp);
		if(result != f_size) {
			printf("%s\n", "Reading Error!");
			exit(1);
		}
		fclose(fp);
		
		/* Search for URL in config */
		if(!slre_compile(&slre, "(github.com.+).git")) {
			printf("Error compiling RE: %s\n", slre.err_str);
		} else if(!slre_match(&slre, buffer, strlen(buffer), captures)) {
			printf("Not match\n" );
		} else {
			printf("Match: %.*s\n", captures[1].len, captures[1].ptr);
		}
	}
}