#include <stdio.h>
#include <stdlib.h>

char* readFile(const char* file_name) {
	FILE* fp = fopen(file_name, "r");
	if (!fp) {
		perror(file_name);
		return NULL;
	}

	char* file_content;
	size_t file_size;

	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	file_content = malloc(file_size + 1);

	fread(file_content, file_size, 1, fp);
	file_content[file_size] = '\0';

	fclose(fp);
	return file_content;
}