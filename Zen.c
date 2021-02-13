#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int status = system("gcc ./Build.c -lm -o __ZEN_EXEC__");

	if(status == -1 || status == 127 || status == 1 || status == 256) {
		printf("\x1B[31mCan't compile Build.c\x1B[0m\n");
		return 1;
	}

	FILE *f = fopen("./__ZEN_EXEC__", "r");

	if(f == NULL) {
		printf("\x1B[31mCan't compile Build.c\x1B[0m\n");
		return 1;
	} else fclose(f);


	char *str1 = calloc(256, 1);


	if(argc == 2) {
		sprintf(str1, "./__ZEN_EXEC__ %s", argv[1]);
		system(str1);
	} else
		system("./__ZEN_EXEC__");
	system("rm ./__ZEN_EXEC__");
}
