#include "../inc/cwd.h"

char *getCompletePath(char completePath[], char path[]) {
	char cwd[100];
	getcwd(cwd, 100);

	sprintf(completePath, "%s%s", cwd, path);
	return completePath;
}
