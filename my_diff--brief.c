#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000


int main(int argc, char** argv){
    
    if (3 > argc)
    {
        printf("Too few parameters\n");
        return 1;
    }

    int j=0;
    
    FILE * file1=fopen(argv[1],"r");
    FILE * file2=fopen(argv[2],"r");

    if(file1 == NULL || file2 == NULL)
    	return 1;


    // INIT TAB1
    char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp1 = tab1;
    char* readThrough1 = (char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize1 = 0;
    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL) {
    	tmp1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
    	strcpy(tmp1[tabSize1], readThrough1);
    	tabSize1++;
    }
	
	// INIT TAB2
    char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp2 = tab2;
    char* readThrough2=(char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize2=0;
    j = 0;
    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
    	tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
    	strcpy(tmp2[tabSize2], readThrough2);
    	tabSize2++;
    }

    // COMAPARAISON DES FICHIERS
    int i = 0;
    j = 0;
	int j2 = 0, i2 = 0;
    int isLinesAdded = 0, isLinesDeleted = 0, isLinesReplaced = 0;
	int isDifferent = 0;

    while(i < tabSize1 && j < tabSize2) {

		if(!strcmp(tab1[i], tab2[j])) {
			i++;
    		j++;
		} else {
            isDifferent = 1;
			break;
    	}
    }
	
	if (isDifferent == 1) {
		printf("Files %s and %s differ", argv[1], argv[2]);
	}

    return 0;
}