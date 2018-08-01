#include <stdio.h>
#include <stdlib.h>

int main(){
	char ciaco[8387194];
	char* ciao = (char*) calloc(sizeof(char), 8387194);
	free(ciao);
	return 0;
}
