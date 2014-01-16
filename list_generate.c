#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char* user_list = argv[1];
    FILE* infile = fopen(user_list, "r");
    char line[257];  // max of 256 char per line, 1 for null terminator
    char* work_str;  // p* used to hold temporary split strings
    char*** master_storage = malloc(100000);  // value is arbitrary
    int i = 0, j = 0;   // Counters, max j expected to be constant for each line

    // Individual attributes
    char id[] = "initialized";
    char first_name[] = "Yew";
    char last_name[] = "Fail";
    int user_number = -666;

    for (; fgets(line, 256, infile) != NULL; i++) {
        // printf("%s\n", line);
        char* new_line = strchr(line, '\n');
        if (new_line)  
            *new_line = '\0';  // removes newline at end
        work_str = strtok(line, " ,;:/");  // splits string into tokens based on " ,;:/" char
        master_storage[i] = malloc(256 * sizeof(char*));  // memory leaky but whatever
        // if (NULL == work_str)
            // printf("work_str is null\n");

        for (j = 0; work_str != NULL; j++) {
            char* new_str = malloc(strlen(work_str)+1);  // temporary hoding string
            strcpy(new_str, work_str);
            master_storage[i][j] = new_str;
            printf("[Set %d, %d to %s]\n", i, j, work_str);
            work_str = strtok(NULL, " ,;:/");  // strtok recycles memory, old overrides new
        }
    }

    for(int i_recall = 0; i_recall < i; i_recall++) {
        for(int j_recall = 0; j_recall < j; j_recall++) {
            printf("%d %d ", i_recall, j_recall);
            printf("%s\n", master_storage[i_recall][j_recall]);
        }
        printf("\n");
    }

    return 0;
}


