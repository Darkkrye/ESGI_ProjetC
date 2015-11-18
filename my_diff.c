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
    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL)
    {
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
    while(fgets(readThrough2, MAX_SIZE, file2) != NULL)
    {
    	tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
    	strcpy(tmp2[tabSize2], readThrough2);
    	tabSize2++;
    }

    // COMAPARAISON DES FICHIERS
    int i = 0;
    j = 0;
	int j2 = 0, i2 = 0;
    int isLinesAdded = 0, isLinesDeleted = 0, isLinesReplaced = 0;

    while(i < tabSize1 && j < tabSize2) {

		if(!strcmp(tab1[i], tab2[j])) {
            // printf("GOOD 2 -- i : %d - j : %d\n", i, j);
			i++;
    		j++;
		} else {
            // printf("i : %d, j : %d\n", i, j);
			j2 = j;
			j++;
            isLinesAdded = 0;
			while (j < tabSize2) {

				if(!strcmp(tab1[i], tab2[j]))
				{
					// Si le fichier 2 a des lignes en plus -->
                    if(j == j2 + 1) {
                        printf("%da%d\n> %s", i, j, tab2[j2]);
                        isLinesAdded = 1;
                    } else {
                        printf("%da%d,%d\n", i, j2+1, j);
                        isLinesAdded = 1;
                        for( ; j2 < j ; j2++) {
                            printf("> %s", tab2[j2]);
                        }
                    }
                    // <--
                    if(isLinesAdded) {
                        j2++;
                        // printf("GOOD --- i : %d - j : %d\n", i, j);
                        break;
                    }
                    // printf("%s - %s", tab1[i], tab2[j]);					
				}
				j++;
			}
            // printf("FIN WHILE - i : %d - j : %d %d\n", i, j2, j);

            if(!isLinesAdded) {
                j = j2;
                i2 = i;
                i++;
                isLinesDeleted = 0;
                while(i < tabSize1) {
                    if(!strcmp(tab1[i], tab2[j])) {
                        if (i == i2 + 1) {
                            printf("%dd%d\n< %s", i, j, tab1[i2]);
                            isLinesDeleted = 1;
                        } else {
                            printf("%d,%dd%d\n", i2+1, i, j);
                            isLinesDeleted = 1;
                            for( ; i2 < i ; i2++) {
                                printf("< %s", tab1[i2]);
                            }
                        }

                        if(isLinesDeleted) {
                            i2++;
                            // printf("GOOD --- i : %d - j : %d\n", i, j);
                            break;
                        }
                    }
                    i++;
                }
            }
    	}


    }
    // printf("\n");

    return 0;
}