#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 1000

int my_diff(char* pArray1, char* pArray2);
void compare(char** tab1, char** tab2, char** modifTab1, char** modifTab2, int tabSize1, int tabSize2);
int my_diff_brief(char* pArray, char* pArray2);
int my_diff_report_identical_files(char* pArray, char* pArray2);
void my_diff_help();
int my_diff_ignore_case(char* pArray1, char* pArray2);
int my_diff_ignore_space(char* pArray1, char* pArray2);

int main(int argc, char** argv){
    
    if (1 == argc) {
		printf("diff: missing operand after 'diff'\ndiff: Try 'diff --help' for more information.\n");
		return 0;
	}
	
	if (3 > argc)
    {
		if(strcmp(argv[1], "--help") == 0) {
			my_diff_help();
			return 0;
		} else {
			printf("diff: missing operand\ndiff: Try 'diff --help' for more information.\n");
			return 1;
		}
    }
	
	//Conditionnal call of function
	if (strcmp(argv[1], "-q") == 0  || strcmp(argv[1], "--brief") == 0) {
		my_diff_brief(argv[2], argv[3]);
	} else if (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--report-identical-files") == 0) {
		my_diff_report_identical_files(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--ignore-case") == 0) {
        my_diff_ignore_case(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-w") == 0 || strcmp(argv[1], "--ignore-all-space") == 0) {
        my_diff_ignore_space(argv[2], argv[3]);
	} else if (strcmp(argv[1], "--help") == 0) {
		my_diff_help();
    } else if (strcmp(argv[1], "--normal") == 0) {
        my_diff(argv[2], argv[3]);
	} else {
		my_diff(argv[1], argv[2]);
	}
}


/*
    ignore case differences in file contents
*/
int my_diff_ignore_case(char* pArray1, char* pArray2) {
    
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

    if(file1 == NULL || file2 == NULL)
    	return 1;


    // INIT TAB1
    char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp1 = tab1;
    // Init a copy of tab1 modified
    char** modifTab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough1 = (char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize1 = 0;
    int x;    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL) {
    	tmp1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        modifTab1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
    	strcpy(tmp1[tabSize1], readThrough1);
        for(x = 0 ; strcmp(&tmp1[tabSize1][x], "\0") != 0 ; x++) {
            // if each char is a lower case, add it to copy tab as a upper case
            if(tmp1[tabSize1][x] >= 65 && tmp1[tabSize1][x] <= 90 /*&& option = "-i"*/) {
                modifTab1[tabSize1][x] = tmp1[tabSize1][x] + 32;
            } else {
                modifTab1[tabSize1][x] = tmp1[tabSize1][x];
            }
        }
    	tabSize1++;
    }
	
	// INIT TAB2
    char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp2 = tab2;
    char** modifTab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough2=(char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize2=0;

    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
    	tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
    	modifTab2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        strcpy(tmp2[tabSize2], readThrough2);
        for(x = 0 ; strcmp(&tmp2[tabSize2][x], "\0") != 0 ; x++) {
            if(tmp2[tabSize2][x] >= 65 && tmp2[tabSize2][x] <= 90 /*&& option = "-i"*/) {
                modifTab2[tabSize2][x] = tmp2[tabSize2][x] + 32;
            } else {
                modifTab2[tabSize2][x] = tmp2[tabSize2][x];
            }
        }
        tabSize2++;
    }

    compare(tab1, tab2, modifTab1, modifTab2, tabSize1, tabSize2);

	return 0;
}


/*
    ignore all white space
*/
int my_diff_ignore_space(char* pArray1, char* pArray2) {
    int j=0;
    
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

    if(file1 == NULL || file2 == NULL)
        return 1;


    // INIT TAB1
    char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp1 = tab1;
    // Init a copy of tab1 modified
    char** modifTab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough1 = (char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize1 = 0;
    int x;    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL) {
        tmp1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        modifTab1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        strcpy(tmp1[tabSize1], readThrough1);
        for(x = 0 ; strcmp(&tmp1[tabSize1][x], "\0") != 0 ; x++) {
            // Add each char to the tab's copy if it's not a white space
            if(tmp1[tabSize1][x] != 32) {
                modifTab1[tabSize1][x] = tmp1[tabSize1][x];
            }
        }
        tabSize1++;
    }
    
    // INIT TAB2
    char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp2 = tab2;
    char** modifTab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough2=(char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize2=0;
    j = 0;
    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
        tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        modifTab2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        strcpy(tmp2[tabSize2], readThrough2);
        for(x = 0 ; strcmp(&tmp2[tabSize2][x], "\0") != 0 ; x++) {
            if(tmp2[tabSize2][x] != 32) {
                modifTab2[tabSize2][x] = tmp2[tabSize2][x];
            }
        }
        tabSize2++;
    }

    
    compare(tab1, tab2, modifTab1, modifTab2, tabSize1, tabSize2);
    return 0;
}


/*
    output a normal diff (the default)
*/
int my_diff(char* pArray1, char* pArray2) {
    
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

    if(file1 == NULL || file2 == NULL)
        return 1;


    int x;
    // INIT TAB1
    char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp1 = tab1;
    char** modifTab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough1 = (char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize1 = 0;
    
    while(fgets(readThrough1, MAX_SIZE, file1) != NULL) {
        tmp1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        modifTab1[tabSize1] = (char*)malloc((strlen(readThrough1))*sizeof(char));
        strcpy(tmp1[tabSize1], readThrough1);
        for(x = 0 ; strcmp(&tmp1[tabSize1][x], "\0") != 0 ; x++) {
            modifTab1[tabSize1][x] = tmp1[tabSize1][x];
        }
        tabSize1++;
    }
    
    // INIT TAB2
    char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char ** tmp2 = tab2;
    char** modifTab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
    char* readThrough2=(char*)malloc((MAX_SIZE)*sizeof(char));
    int tabSize2=0;

    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
        tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        modifTab2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        strcpy(tmp2[tabSize2], readThrough2);
        for(x = 0 ; strcmp(&tmp2[tabSize2][x], "\0") != 0 ; x++) {
            modifTab2[tabSize2][x] = tmp2[tabSize2][x];
        }
        tabSize2++;
    }

    compare(tab1, tab2, modifTab1, modifTab2, tabSize1, tabSize2);
    return 1;
}


/*
    Common comparison code
*/
void compare(char** tab1, char** tab2, char** modifTab1, char** modifTab2, int tabSize1, int tabSize2) {
    // FILES COMPARISON
    int i = 0;
    int j = 0;
    int j2 = 0, i2 = 0;
    int isLinesAdded = 0, isLinesDeleted = 0, isLinesReplaced = 0;

    while(i < tabSize1 && j < tabSize2) {

        if(!strcmp(modifTab1[i], modifTab2[j])) {
            i++;
            j++;
        } else {
            j2 = j;
            isLinesAdded = 0;
            while (j < tabSize2) {

                if(!strcmp(modifTab1[i], modifTab2[j]))
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
                    if(!strcmp(modifTab1[i], modifTab2[j])) {
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
                            if(!strcmp(modifTab1[i], modifTab2[j])) {
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
            printf("%dd%d\n< %s", i+1, j, tab1[i]);
        } else {
            printf("%d,%dd%d\n", i + 1, tabSize1, j);
            for(; i < tabSize1 ; i++) {
                printf("< %s", tab1[i]);
            }
        }
    }

    free(tab1);
    free(tab2);
    free(modifTab1);
    free(modifTab2);
}


/*
    report only when files differ
*/
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
	
	//Run through the files
    while(i < tabSize1 && j < tabSize2) {
		if(!strcmp(tab1[i], tab2[j])) {
			i++;
    		j++;
		} else {
            isDifferent = 1;
			break;
    	}
    }
	
	//if files have at least 1 line different
	if (isDifferent == 1) {
		printf("Files %s and %s differ\n", pArray1, pArray2);
	}
	
	return 0;
}


/*
    report when two files are the same
*/
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
		printf("Files %s and %s are identical\n", pArray1, pArray2);
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


/*
    display this help and exit
*/
void my_diff_help() {
	printf("\n");
	printf("%0s %54s\n", "    --normal", "output a normal diff (the default)");
	printf("%0s %50s\n", "-q, --brief", "report only when files differ");
	printf("%0s %38s\n", "-s, --report-identical-files", "report when two files are the same");
	printf("\n");
	printf("%0s %55s\n", "-i, --ignore-case", "ignore case differences in file contents");
	printf("%0s %32s\n", "-w, --ignore-all-space", "ignore all white space");
	printf("\n");
	printf("%0s %48s\n", "    --help", "display this help and exit");
}