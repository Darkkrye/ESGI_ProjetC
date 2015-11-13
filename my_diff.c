#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_MAX 1000


int main(int argc, char** argv){
    
    if (3 > argc)
    {
        printf("Nombre de parametres insuffisant\n");
        return 1;
    }

    FILE* fich1 = NULL;
    FILE* fich2 = NULL;

    fich1 = fopen(argv[1], "r");
    fich2 = fopen(argv[2], "r");

    char ligneFich1[TAILLE_MAX] = "";
    char ligneFich2[TAILLE_MAX] = "";

    // char* ligneFich1 = NULL;
    // ligneFich1 = (char*)malloc(sizeof(char*));
    // char* ligneFich2 = NULL;
    // ligneFich2 = (char*)malloc(sizeof(char*));

    while(fgets(ligneFich1, TAILLE_MAX, fich1) != NULL) {
    	while(fgets(ligneFich2, TAILLE_MAX, fich2) != NULL) {
    		if(0 == strcmp(ligneFich1, ligneFich2)) {
    			printf("%s GOOD\n", ligneFich1);
    			printf("%s GOOD\n", ligneFich2);
    			break;
    		} else {
    			printf("%s BAD\n", ligneFich1);
    			printf("%s BAD\n", ligneFich2);
    		}
    	//     ligneFich2 = (char*)malloc(sizeof(char*));
    		printf("%s\n", ligneFich2);
    	}
	    // ligneFich1 = (char*)malloc(sizeof(char*));
	    printf("%s\n", ligneFich1);
    }

    return 0;
}