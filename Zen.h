#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int IsEarlier(char *file1, char *file2)
{
	struct stat attr1, attr2;
	int r1 = stat(file1, &attr1);
	int r2 = stat(file2, &attr2);

	if(r1 == -1) return 0;
	if(r2 == -1) return 1;

	return attr1.st_mtim.tv_sec > attr2.st_mtim.tv_sec;
}

char *RemoveExt(char *name)
{
	char *new_name = calloc(256, 1);
	char *dot = strrchr(name, '.');
	if(!dot || dot == name) return name;

	size_t len = dot - name;

	for(size_t i = 0; i < len; i++)
		new_name[i] = name[i];

	return new_name;
}

char *GetExt(char *name)
{
	char *dot = strrchr(name, '.');
	if(!dot || dot == name) return "";
	return dot + 1;
}

void ForEach(char *dir_name, char *ext, void (*func)(char*, char*))
{
	DIR *dir = opendir(dir_name);

	struct dirent *ent = readdir(dir);

	while(ent != NULL) {
		if(ent->d_type == DT_REG) {
			if(strcmp("Build.c", ent->d_name) == 0)
				goto end;
			char *ext_1 = GetExt(ent->d_name);
			char *file_name = RemoveExt(ent->d_name);
			if(strcmp(ext, ext_1) == 0)
				func(dir_name, file_name);

		}
end:
		ent = readdir(dir);
	}

	closedir(dir);
}

void Compile(char *shell, ...)
{
	va_list ap;
	va_start(ap, shell);

	int len = vsnprintf(NULL, 0, shell, ap);

	va_end(ap);

	char *str = calloc(len + 1, 1);

	va_start(ap, shell);
	vsnprintf(str, len + 1, shell, ap);
	va_end(ap);

	printf("%s\n\n", str);
	int status = system(str);

	if(status == -1 || status == 127 || status == 1 || status == 256) {
		printf("\x1B[31mCan't compile file\x1B[0m\n");
		exit(1);
	}

	free(str);
}

void Execute(char *shell, int expected)
{
	printf("%s\n\n", shell);
	int status = system(shell);

	if( ( status == -1 || status == 127 ) && status != expected) {
		printf("\x1B[31mCan't execute \x1B[1m\"%s\"\x1B[0m\n", shell);
		exit(1);
	}
}

#define SetTarget(x, f) if(argc == 2) { if(strcmp(x, argv[1]) == 0) { f(); return 0; } }
