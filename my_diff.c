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
	
	
	
	
	
    /*j=0;	
    while(j < tabSize1)
    {
    	printf("%i : %s", j, tab1[j]);
    	j++;
    }
	
	printf("\n\n");
	
    j=0;
    while(j < tabSize2)
    {
    	printf("%i : %s", j, tab2[j]);
    	j++;
    }*/



    // COMAPARAISON DES FICHIERS
    int i = 0;
    j = 0;
	int j2 = 0;
    while(i < tabSize1 && j < tabSize2) {
		if(!strcmp(tab1[i], tab2[j])) {
			// printf("Ligne %i OK\n", i+1);
			i++;
    		j++;
		} else { //En gros j'essaye de voir si les 2 lignes ne sont pas identiques je continue les lignes du fichier 2 
			/*j2 = j; //et j'essaye de trouver si la ligne du fichier 2 == la ligne du fichier 1 qui est en attente  
			j2++; //et si c'est le cas et bin faut faire des trucs avec. J2 <- J ça permet après de pouvoir dire que dans le fichier
			while (j2 < tabSize2) { //ce sont les lignes de j2 à j (puisqu'on a continué j) qui ne sont pas dans le fichier 1.
				if(!strcmp(tab1[i], tab2[j2])) //après j'ai juste essayé des printf pour voir ce que j'avais
				{
					// printf("%i:%s - %i:%s", j, tab2[j], i, tab1[i]);
					if(i +1 == j2) {
						printf("%ic%i\n", i, j2);
						printf("< %s", tab1[i]);
						printf("---\n");
						printf("> %s", tab2[j2]);	
					}
					
					if (i +2 == j2) {
						printf("%ia%i,%i\n", i, j2+1, j);
						printf("> %s", tab2[j2]);
						printf("> %s", tab2[j2+1]);						
					}
				}
				j2++;
			}
			i++;j++;*/
			
			// printf("Ligne i:%i / Ligne j:%i pas OK\n", i+1, j+1);
			int saveJ = j;
			int saveI = i;
			int runThrough = 0;
			while (i < tabSize1 && j < tabSize2) {
				// printf("Ligne i:%i / Ligne j:%i\n", i+1, j+1);
				
				if (!strcmp(tab1[i], tab2[j])) {
					// printf("%s/%s", tab2[j], tab1[i]);
					printf("%ia%i,%i\n", i, saveJ+1, j);
					for (runThrough = saveJ; runThrough < j; runThrough++) {
						printf("> %s", tab2[runThrough]);
					}
					runThrough = 0;
					break;
				}
				else if (j == tabSize2-1 && strcmp(tab1[i], tab2[j])) {
					/*printf("%i - %i / %i - %i\n", i, j, saveI, saveJ);
					printf("%ic%i\n<%s---\n>%s", i+1, saveJ+1, tab1[i], tab2[saveJ]);*/
					
					while (i < tabSize1) {
						/*if(i+1 - saveI == j - saveJ && i < tabSize1-1 && j < tabSize2-1) {
							printf("%ic%i\n<%s---\n>%s", saveI+1, saveJ+1, tab1[saveI], tab2[saveJ]);
						} else if (i == tabSize1-1 && j == tabSize2-1) {
							printf("%i, %id%i,%i\n", saveI, i, saveJ+1, j);
						}*/
						
						if (i == tabSize1-1 && strcmp(tab1[i], tab2[j])) { //Test algo d
							printf("%id%i\n", saveI, i);
							for (runThrough = saveI; runThrough < i; runThrough++) {
								printf("< %s", tab1[runThrough]);
							}
							runThrough = 0;
							break;
						}
						
						i++;
					}
					
					i = saveI;
				}
				j++;
			}
			j = saveJ;
			i++;
			j++;
    	}

    }

// ANCIENNEMENT
    // char fileLine1[MAX_SIZE] = "";
    // char fileLine2[MAX_SIZE] = "";
    // while(fgets(fileLine1, MAX_SIZE, file1) != NULL) {
    // 	while(fgets(fileLine2, MAX_SIZE, file2) != NULL) {
    // 		if(0 == strcmp(fileLine1, fileLine2)) {
    // 			// printf("> %s", fileLine1);
    // 			break;
    // 		} else {
    // 			printf("> %s", fileLine2);
    // 		}
    // 	//     fileLine2 = (char*)malloc(sizeof(char*));
    // 		// printf("%s\n", fileLine2);
    // 	}
	   //  // fileLine1 = (char*)malloc(sizeof(char*));
	   //  // printf("%s\n", fileLine1);
    // }
// <--

    return 0;
}