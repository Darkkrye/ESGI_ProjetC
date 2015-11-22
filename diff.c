#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

int my_diff(char* pArray1, char* pArray2);
int my_diff_brief(char* pArray, char* pArray2);
int my_diff_report_identical_files(char* pArray, char* pArray2);
void my_diff_help();
int my_diff_ignore_case(char* pArray1, char* pArray2);

int main(int argc, char** argv){
    
    if (1 == argc) {
		printf("diff: missing operand after 'diff'\ndiff: Try 'diff --help' for more information.\n");
		return 0;
	}
	
	if (3 > argc)
    {
		if(argv[1][0] == 45 && argv[1][1] == 45 && argv[1][2] == 104) {
			my_diff_help();
			return 0;
		} else {
			printf("Nombre de parametres insuffisant\n");
			return 1;
		}
    }
	
	if (argv[1][0] == 45 && argv[1][1] == 113) {
		my_diff_brief(argv[2], argv[3]);
	} else if (argv[1][0] == 45 && argv[1][1] == 115) {
		my_diff_report_identical_files(argv[2], argv[3]);
    // } else if (argv[1][0] == 45 && argv[1][1] == ) {

	} else if (argv[1][0] == 45 && argv[1][1] == 45 && argv[1][2] == 104) {
		my_diff_help();
	} else {
		my_diff_ignore_case(argv[1], argv[2]);
	}
}

int my_diff_ignore_case(char* pArray1, char* pArray2) {
	int j=0;
    
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

    if(file1 == NULL || file2 == NULL)
    	return 1;


    // INIT TAB1
    char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp1 = tab1;
    char** noCase1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough1 = (char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize1 = 0;
    int x;    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL) {
    	tmp1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        noCase1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
    	strcpy(noCase1[tabSize1], readThrough1);
        for(x = 0 ; strcmp(&noCase1[tabSize1][x], "\0") != 0 ; x++) {
            if(noCase1[tabSize1][x] >= 65 && noCase1[tabSize1][x] <= 90 /*&& option = "-i"*/) {
                tmp1[tabSize1][x] = noCase1[tabSize1][x] + 32;
            } else {
                tmp1[tabSize1][x] = noCase1[tabSize1][x];
            }
        }
    	tabSize1++;
    }
	
	// INIT TAB2
    char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp2 = tab2;
    char** noCase2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough2=(char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize2=0;
    j = 0;
    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
    	tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
    	noCase2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        strcpy(noCase2[tabSize2], readThrough2);
        for(x = 0 ; strcmp(&noCase2[tabSize2][x], "\0") != 0 ; x++) {
            if(noCase2[tabSize2][x] >= 65 && noCase2[tabSize2][x] <= 90 /*&& option = "-i"*/) {
                tmp2[tabSize2][x] = noCase2[tabSize2][x] + 32;
            } else {
                tmp2[tabSize2][x] = noCase2[tabSize2][x];
            }
        }
        tabSize2++;
    }

    // FILES COMPARISON
    int i = 0;
    j = 0;
	int j2 = 0, i2 = 0;
    int isLinesAdded = 0, isLinesDeleted = 0, isLinesReplaced = 0;

    while(i < tabSize1 && j < tabSize2) {
		if(!strcmp(tab1[i], tab2[j])) {
			i++;
    		j++;
		} else {
			j2 = j;
            isLinesAdded = 0;
			while (j < tabSize2) {

				if(!strcmp(tab1[i], tab2[j]))
				{
                    if(j == j2 + 1) {
                        printf("%da%d\n> %s", i, j, noCase2[j2]);
                        isLinesAdded = 1;
                    } else {
                        printf("%da%d,%d\n", i, j2+1, j);
                        isLinesAdded = 1;
                        for( ; j2 < j ; j2++) {
                            printf("> %s", noCase2[j2]);
                        }
                    }
                    if(isLinesAdded) {
                        j2++;
                        break;
                    }
				}
				j++;
			}

            if(!isLinesAdded) {
                j = j2;
                i2 = i;
                isLinesDeleted = 0;
                while(i < tabSize1) {
                    if(!strcmp(tab1[i], tab2[j])) {
                        if (i == i2 + 1) {
                            printf("%dd%d\n< %s", i, j, noCase1[i2]);
                            isLinesDeleted = 1;
                        } else {
                            printf("%d,%dd%d\n", i2+1, i, j);
                            isLinesDeleted = 1;
                            for( ; i2 < i ; i2++) {
                                printf("< %s", noCase1[i2]);
                            }
                        }

                        if(isLinesDeleted) {
                            i2++;
                            break;
                        }
                    }
                    i++;
                }

                if(!isLinesDeleted) {
                    i = i2;
                    isLinesReplaced = 0;
                    while(i < tabSize1) {
                        j = j2;
                        while(j < tabSize2) {
                            if(!strcmp(tab1[i], tab2[j])) {
                                if(i2 + 1 == i && j2 + 1 == j)
                                    printf("%dc%d\n", i, j);
                                else if (i2 + 1 == i && j2 + 1 != j)
                                    printf("%dc%d,%d\n", i2+1, j2+1, j);
                                else if(i2 + 1 != i && j2 + 1 == j)
                                    printf("%d,%dc%d\n", i2+1, i, j2+1);
                                else 
                                    printf("%d,%dc%d,%d\n", i2+1, i, j2+1, j);

                                for (; i2 < i; i2++) {
                                    printf("< %s", noCase1[i2]);
                                }
                                printf("---\n");
                                for(; j2 < j; j2++) {
                                    printf("> %s", noCase2[j2]);
                                }
                                isLinesReplaced = 1;
                                break;
                            }
                            j++;
                        }
                        if(isLinesReplaced)
                            break;
                        i++;
                    }
                }
            }
    	}


    }

    if(i >= tabSize1 && j < tabSize2) {
        if(j + 1 == tabSize2) {
            printf("%da%d\n> %s", i, j+1, noCase2[j]);
        } else {
            printf("%da%d,%d\n", i, j+1, tabSize2);
            for(; j < tabSize2 ; j++) {
                printf("> %s", noCase2[j]);
            }
        }
    } else if(i < tabSize1 && j >= tabSize2) {
        if(i + 1 == tabSize1) {
            printf("%dd%d\n< %s", i, j+1, noCase1[i]);
        } else {
            printf("%d,%da%d\n", i + 1, tabSize1, j);
            for(; i < tabSize1 ; i++) {
                printf("< %s", noCase1[i]);
            }
        }
    }

    free(tab1);
    free(tab2);

	return 0;
}

int my_diff(char* pArray1, char* pArray2) {
    int j=0;
    
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

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

    // FILES COMPARISON
    int i = 0;
    j = 0;
    int j2 = 0, i2 = 0;
    int isLinesAdded = 0, isLinesDeleted = 0, isLinesReplaced = 0;

    while(i < tabSize1 && j < tabSize2) {

        if(!strcmp(tab1[i], tab2[j])) {
            i++;
            j++;
        } else {
            j2 = j;
            isLinesAdded = 0;
            while (j < tabSize2) {

                if(!strcmp(tab1[i], tab2[j]))
                {
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
                    if(isLinesAdded) {
                        j2++;
                        break;
                    }
                }
                j++;
            }

            if(!isLinesAdded) {
                j = j2;
                i2 = i;
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
                            break;
                        }
                    }
                    i++;
                }

                if(!isLinesDeleted) {
                    i = i2;
                    isLinesReplaced = 0;
                    while(i < tabSize1) {
                        j = j2;
                        while(j < tabSize2) {
                            if(!strcmp(tab1[i], tab2[j])) {
                                if(i2 + 1 == i && j2 + 1 == j)
                                    printf("%dc%d\n", i, j);
                                else if (i2 + 1 == i && j2 + 1 != j)
                                    printf("%dc%d,%d\n", i2+1, j2+1, j);
                                else if(i2 + 1 != i && j2 + 1 == j)
                                    printf("%d,%dc%d\n", i2+1, i, j2+1);
                                else 
                                    printf("%d,%dc%d,%d\n", i2+1, i, j2+1, j);

                                for (; i2 < i; i2++) {
                                    printf("< %s", tab1[i2]);
                                }
                                printf("---\n");
                                for(; j2 < j; j2++) {
                                    printf("> %s", tab2[j2]);
                                }
                                isLinesReplaced = 1;
                                break;
                            }
                            j++;
                        }
                        if(isLinesReplaced)
                            break;
                        i++;
                    }
                }
            }
        }


    }

    if(i >= tabSize1 && j < tabSize2) {
        if(j + 1 == tabSize2) {
            printf("%da%d\n> %s", i, j+1, tab2[j]);
        } else {
            printf("%da%d,%d\n", i, j+1, tabSize2);
            for(; j < tabSize2 ; j++) {
                printf("> %s", tab2[j]);
            }
        }
    } else if(i < tabSize1 && j >= tabSize2) {
        if(i + 1 == tabSize1) {
            printf("%dd%d\n< %s", i, j+1, tab1[i]);
        } else {
            printf("%d,%da%d\n", i + 1, tabSize1, j);
            for(; i < tabSize1 ; i++) {
                printf("< %s", tab1[i]);
            }
        }
    }

    free(tab1);
    free(tab2);

    return 0;
}

int my_diff_brief(char* pArray1, char* pArray2) {
	int j=0;
    
    FILE * file1=fopen(pArray1,"r");
    FILE * file2=fopen(pArray2,"r");

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
	

    // FILES COMPARISON
    int i = 0, isDifferent = 0;
	j = 0;

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
		printf("Files %s and %s differ", pArray1, pArray2);
	}
	
	return 0;
}

int my_diff_report_identical_files(char* pArray1, char* pArray2) {
	int j=0;
    
    FILE * file1=fopen(pArray1,"r");
    FILE * file2=fopen(pArray2,"r");

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

    // FILES COMPARISON
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
			j2 = j;
            isLinesAdded = 0;
			while (j < tabSize2) {

				if(!strcmp(tab1[i], tab2[j]))
				{
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
                    if(isLinesAdded) {
                        j2++;
                        break;
                    }		
				}
				j++;
			}

            if(!isLinesAdded) {
                j = j2;
                i2 = i;
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
                            break;
                        }
                    }
                    i++;
                }

                if(!isLinesDeleted) {
                    i = i2;
                    isLinesReplaced = 0;
                    while(i < tabSize1) {
                        j = j2;
                        while(j < tabSize2) {
                            if(!strcmp(tab1[i], tab2[j])) {
                                if(i2 + 1 == i && j2 + 1 == j)
                                    printf("%dc%d\n", i, j);
                                else if (i2 + 1 == i && j2 + 1 != j)
                                    printf("%dc%d,%d\n", i2+1, j2+1, j);
                                else if(i2 + 1 != i && j2 + 1 == j)
                                    printf("%d,%dc%d\n", i2+1, i, j2+1);
                                else 
                                    printf("%d,%dc%d,%d\n", i2+1, i, j2+1, j);

                                for (; i2 < i; i2++) {
                                    printf("< %s", tab1[i2]);
                                }
                                printf("---\n");
                                for(; j2 < j; j2++) {
                                    printf("> %s", tab2[j2]);
                                }
                                isLinesReplaced = 1;
                                break;
                            }
                            j++;
                        }
                        if(isLinesReplaced)
                            break;
                        i++;
                    }
                }
            }
    	}


    }
	
	if (isDifferent == 0) {
		printf("Files %s and %s are identical", pArray1, pArray2);
	} else {
				if(i >= tabSize1 && j < tabSize2) {
			if(j + 1 == tabSize2) {
				printf("%da%d\n> %s", i, j+1, tab2[j]);
			} else {
				printf("%da%d,%d\n", i, j+1, tabSize2);
				for(; j < tabSize2 ; j++) {
					printf("> %s", tab2[j]);
				}
			}
		} else if(i < tabSize1 && j >= tabSize2) {
			if(i + 1 == tabSize1) {
				printf("%dd%d\n< %s", i, j+1, tab1[i]);
			} else {
				printf("%d,%da%d\n", i + 1, tabSize1, j);
				for(; i < tabSize1 ; i++) {
					printf("< %s", tab1[i]);
				}
			}
		}
		free(tab1);
		free(tab2);
	}
    return 0;
}

void my_diff_help() {
	printf("\n");
	printf("%0s %58s\n", "--normal", "output a normal diff (the default)");
	printf("%0s %50s\n", "-q, --brief", "report only when files differ");
	printf("%0s %38s\n", "-s, --report-identical-files", "report when two files are the same");
}