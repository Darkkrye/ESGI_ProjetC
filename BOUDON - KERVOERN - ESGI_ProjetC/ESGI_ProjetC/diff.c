#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

#define MAX_SIZE 1000

int my_diff(char* pArray1, char* pArray2);
void compare(char** tab1, char** tab2, char** modifTab1, char** modifTab2, int tabSize1, int tabSize2);
int my_diff_brief(char* pArray, char* pArray2);
int my_diff_report_identical_files(char* pArray, char* pArray2);
void my_diff_help();
void my_diff_version();
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
        } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            my_diff_version();
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
    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        my_diff_version();
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
            if(tmp1[tabSize1][x] >= 65 && tmp1[tabSize1][x] <= 90) {
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
            if(tmp2[tabSize2][x] >= 65 && tmp2[tabSize2][x] <= 90) {
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
    const char c = ' ';
    FILE * file1=fopen(pArray1,"r+");
    FILE * file2=fopen(pArray2,"r+");

    if(file1 == NULL || file2 == NULL)
        return 1;


    int count = 0;
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
        count = 0;
        for(x = 0 ; strcmp(&tmp1[tabSize1][x], "\0") != 0 ; x++) {
            // Add each char to the tab's copy if it's not a white space
            if(tmp1[tabSize1][x] != c) {
                modifTab1[tabSize1][x - count] = tmp1[tabSize1][x];
            } else {
                count++;
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
    count = 0;
    while(fgets(readThrough2, MAX_SIZE, file2) != NULL) {
        tmp2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        modifTab2[tabSize2] = (char*)malloc((strlen(readThrough2))*sizeof(char));
        strcpy(tmp2[tabSize2], readThrough2);
        count = 0;
        for(x = 0 ; strcmp(&tmp2[tabSize2][x], "\0") != 0 ; x++) {
            if(tmp2[tabSize2][x] != c) {
                modifTab2[tabSize2][x - count] = tmp2[tabSize2][x];
            } else {
                count++;
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

    if(file1 == NULL || file2 == NULL) {
        printf("%s/%s: No such file or directory\n", pArray1, pArray2);
        return 1;
    }

    if(file1 == NULL && file2 == NULL) {
        // Init tab where we'll stock names of files
        char** tab1 = (char**)malloc((MAX_SIZE)*sizeof(char*));
        char ** tmp1 = tab1;
        char** tab2 = (char**)malloc((MAX_SIZE)*sizeof(char*));
        char ** tmp2 = tab2;
        int tabSize1 = 0, tabSize2 = 0;

        // Init tab 1
        struct dirent *lecture;
        DIR *rep;
        rep = opendir(pArray1);
        while ((lecture = readdir(rep))) {
            tmp1[tabSize1] = (char*)malloc((strlen(lecture->d_name))*sizeof(char));
            tmp1[tabSize1] = lecture->d_name;
            // printf("%s\n", lecture->d_name);
            tabSize1++;
        }
        closedir(rep);
        printf("");

        // Init tab 2
        struct dirent *lecture2;
        DIR *rep2;
        rep2 = opendir(pArray2);
        while ((lecture2 = readdir(rep2))) {
            tmp2[tabSize2] = (char*)malloc((strlen(lecture2->d_name))*sizeof(char));
            tmp2[tabSize2] = lecture2->d_name;
            tabSize2++;
        }
        closedir(rep2);
        
        int i, j, isOnly;

        // Check tab1
        for(i = 0 ; i < tabSize1 ; i++) {
            isOnly = 1;
            // Check tab2
            for(j = 0 ; j < tabSize2 ; j++) {
                // Compare the line of tab1 with each of tab2
                if(strcmp(tmp1[i], tmp2[j]) == 0) {
                    isOnly = 0;
                    break;
                }
            }
            // If no lines are equals, print 
            if(isOnly) {
                printf("Only in %s: %s\n", pArray1, tab1[i]);
                printf("");
            }
        }

        // Check tab2
        for(j = 0 ; j < tabSize2 ; j++) {
            isOnly = 1;
            // Check tab 1
            for(i = 0 ; i < tabSize1 ; i++) {
                // Compare the line of tab2 with each of tab1
                if(strcmp(tmp1[i], tmp2[j]) == 0) {
                    isOnly = 0;
                    break;
                }
            }
            // If no lines are equals, print
            if(isOnly == 1) {
                printf("Only in %s: %s\n", pArray2, tab2[j]);
                printf("");
            }

        }

        return 1;
    }


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

    // Loop : index of tab < nomber of lines of the file
    while(i < tabSize1 && j < tabSize2) {
        // If line from file A and line from file B are equal 
        if(!strcmp(modifTab1[i], modifTab2[j])) {
            i++;
            j++;
        } else {
            // if the lines differ
            j2 = j;
            isLinesAdded = 0;
            // Keep the line of file A
            // And continue to progress in file B
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

            // Keep the line of file B
            // And continue to progress in file A
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
                    // if both previous treatment failed,
                    // compare the line of file A with each of file B
                    // then get the next line and do it again.
                    while(i < tabSize1) {
                        j = j2;
                        while(j < tabSize2) {
                            if(!strcmp(modifTab1[i], modifTab2[j])) {
                                if(i2 + 1 == i && j2 + 1 == j) {
                                    printf("%dc%d\n", i, j);
                                }
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

    // When main loop if finish, at least 1 index > to nomber of lines of the file

    // case where tab1 go to the end and not tab2
    //  print each line of tab2 more
    if(i >= tabSize1 && j < tabSize2) {
        if(j + 1 == tabSize2) {
            printf("%da%d\n> %s", i, j+1, tab2[j]);
        } else {
            printf("%da%d,%d\n", i, j+1, tabSize2);
            for(; j < tabSize2 ; j++) {
                printf("> %s", tab2[j]);
            }
        }
    // case where tab2 go to the end and not tab1
    //  print each line of tab1 more
    } else if(i < tabSize1 && j >= tabSize2) {
        if(i + 1 == tabSize1) {
            printf("%dd%d\n< %s", i+1, j, tab1[i]);
        } else {
            printf("%d,%dd%d\n", i + 1, tabSize1, j);
            for(; i < tabSize1 ; i++) {
                printf("< %s", tab1[i]);
            }
        }
    // else compare both end of files
    } else {
        i2 = i;
        while(i < tabSize1) {
            j2 = j;
            while(j < tabSize2) {
                if(!strcmp(modifTab1[i], modifTab2[j])) {
                    if(i2 + 1 == i && j2 + 1 == j) {
                        printf("%dc%d\n", i, j);
                    }
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
            i++;
        }

    }

    // free tabs
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
				printf("%d,%dd%d\n", i + 1, tabSize1, j);
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
    printf("%0s %54s\n", "-v, --version", "output version information and exit");
	printf("%0s %48s\n", "    --help", "display this help and exit");
}

void my_diff_version() {
    printf("diff (GNU diffutils) 1.0 (2015)\n");
    printf("\n");
    printf("This program comes with NO WARRANTY, to the extent permitted by law.\n");
    printf("\n");
    printf("Written by Nicolas KERVOERN and Pierre BOUDON.\n");
}