#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "lib/slre.h"

#ifndef GIT_CONFIG
#define GIT_CONFIG "/.git/config"
#endif

struct slre slre;
struct cap captures[4 + 1];

int find_git_config(char *fullpath, char const *path);
void open_github_website(char const *file);

int main(void)
{
    char current_directory[PATH_MAX + 1];
    char fullpath[PATH_MAX + strlen(GIT_CONFIG) + 1];

    getcwd(current_directory, PATH_MAX);

    if (find_git_config(fullpath, current_directory))
        open_github_website(fullpath);
    else
        printf("No valid git repository found for the current directory.\n");

    return 0;
}

int find_git_config(char *fullpath, char const *path)
{
    strcpy(fullpath, path);
    strcat(fullpath, GIT_CONFIG);

    return (access(fullpath, F_OK) == 0);
}

void open_github_website(char const *file)
{
    FILE *fp;
    size_t size, result;
    char *buffer, *url;
	
    if ((fp = fopen(file, "r")) == NULL) {
        printf("Could not open the git configuration file..\n");
        exit(1);
    } else {
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        rewind(fp);
		
		/* reads the configuration file */
        buffer = (char *)malloc(size * sizeof(char));
        result = fread(buffer, 1, size, fp);
        if (result != size) {
            printf("An error occurred while trying to read the git configuration file.\n");
            exit(1);
        }
        fclose(fp);
		
		/* search by the github's url */
        if (!slre_compile(&slre, "github.com.(.+).git")) {
            printf("An error occurred while compiling the regular expression: %s\n", slre.err_str);
        } else if(!slre_match(&slre, buffer, strlen(buffer), captures)) {
            printf("Was not found a github repository in the git configuration file.\n");
        } else {
            sprintf(buffer, "%.*s", captures[1].len, captures[1].ptr);
            url = (char *)malloc((strlen(buffer) + 26) * sizeof(char));
            strcpy(url, "open https://github.com/");
            strcat(url, buffer);
            system(url);
            free(url);
        }
        free(buffer);
    }
}