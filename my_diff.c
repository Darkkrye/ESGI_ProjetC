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

  //   FILE* fich1 = NULL;
  //   FILE* fich2 = NULL;

  //   fich1 = fopen(argv[1], "r");
  //   fich2 = fopen(argv[2], "r");


  //   char** lignesFich1 = NULL;
  //   lignesFich1 = (char**)malloc(TAILLE_MAX*sizeof(char*));
  //   char** lignesFich2 = NULL;
  //   lignesFich2 = (char**)malloc(TAILLE_MAX*sizeof(char*));
  //   int i = 0;
  //   char ligneFich1[TAILLE_MAX] = "";
  //   printf("START\n");
  //   while(fgets(ligneFich1, TAILLE_MAX, fich1) != NULL) {
  //   	printf("TEST\n");
    	
  //   	lignesFich1[i] = ligneFich1;
		
		// int j;
  //   	for (j = 0; j <= i; ++j)
  //   	{
  //   		printf("%s\n", lignesFich1[j]);
  //   	}
  //   	i++;
  //   }
  //   // printf("%s\n", ligneFich1);
  //   printf("%s\n", lignesFich1[2]);
  //   printf("%s\n", lignesFich1[0]);



    int j=0;
    
    FILE * fich1=fopen(argv[1],"r");
    FILE * fich2=fopen(argv[2],"r");

    if(fich1 == NULL || fich2 == NULL)
    	return 1;


    // INIT TAB1
    char** tab1 = (char**)malloc((TAILLE_MAX)*sizeof(char*));
    char ** tmp1 = tab1;
    char* parcours1 = (char*)malloc((TAILLE_MAX)*sizeof(char));
    int tailleTab1 = 0;
    
    while(fgets(parcours1, TAILLE_MAX, fich1) != NULL)
    {
    	tmp1[tailleTab1] = (char*)malloc((strlen(parcours1))*sizeof(char));
    	strcpy(tmp1[tailleTab1], parcours1);
    	tailleTab1++;
    }

    j=0;

    while(j < tailleTab1)
    {
    	printf("%s", tab1[j]);
    	j++;
    }

    // INIT TAB2
    char** tab2 = (char**)malloc((TAILLE_MAX)*sizeof(char*));
    char ** tmp2 = tab2;
    char* parcours2=(char*)malloc((TAILLE_MAX)*sizeof(char));
    int tailleTab2=0;
    j = 0;
    while(fgets(parcours2, TAILLE_MAX, fich2) != NULL)
    {
    	tmp2[tailleTab2] = (char*)malloc((strlen(parcours2))*sizeof(char));
    	strcpy(tmp2[tailleTab2], parcours2);
    	tailleTab2++;
    }

    j=0;

    while(j < tailleTab2)
    {
    	printf("%s", tab2[j]);
    	j++;
    }



    // COMAPARAISON DES FICHIERS
    // int i = 0;
    // j = 0;
    // while(i <= tailleTab1 && j <= tailleTab2) {
    // 	if(strcmp(tab1[i], tab2[j])) {
    // 		i++;
    // 		j++;
    // 	} else {

    // 	}

    // }

// ANCIENNEMENT
    // char ligneFich1[TAILLE_MAX] = "";
    // char ligneFich2[TAILLE_MAX] = "";
    // while(fgets(ligneFich1, TAILLE_MAX, fich1) != NULL) {
    // 	while(fgets(ligneFich2, TAILLE_MAX, fich2) != NULL) {
    // 		if(0 == strcmp(ligneFich1, ligneFich2)) {
    // 			// printf("> %s", ligneFich1);
    // 			break;
    // 		} else {
    // 			printf("> %s", ligneFich2);
    // 		}
    // 	//     ligneFich2 = (char*)malloc(sizeof(char*));
    // 		// printf("%s\n", ligneFich2);
    // 	}
	   //  // ligneFich1 = (char*)malloc(sizeof(char*));
	   //  // printf("%s\n", ligneFich1);
    // }
// <--

    return 0;
}