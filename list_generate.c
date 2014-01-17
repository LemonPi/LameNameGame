#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    char* user_list = argv[1];
    FILE* infile = fopen(user_list, "r");
    char line[257];
    char* work_str;
    char*** master_storage = malloc(100000);
    int i = 0, j = 0;   // Counters

    // Individual attributes
    char id[] = "initialized";
    char first_name[] = "Yew";
    char last_name[] = "Fail";
    int user_number = -666;

    for (; fgets(line, 256, infile) != NULL; i++) {
        printf("%s\n", line);
        char* new_line = strchr(line, '\n');
        if (new_line)
            *new_line = '\0';
        work_str = strtok(line, " ,;:/");
        master_storage[i] = malloc(256 * sizeof(char*));
        if (NULL == work_str)
            printf("work_str is null\n");

        for (j = 0; work_str != NULL; j++) {
            char* new_str = malloc(strlen(work_str)+1);
            strcpy(new_str, work_str);
            master_storage[i][j] = new_str;
            printf("[Set %d, %d to %s]\n", i, j, work_str);
            work_str = strtok(NULL, " ,;:/");
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


