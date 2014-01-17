#define _GNU_SOURCE  // defines rawmemchr
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct name_data {
    char* utorid;
    char* names[10];
} name_data;

char* old_s;  // each time keep same pointer
int points = 0;
int student_n;
const char* class_find = "esc1t7";
name_data* master_storage[100000];  // value is arbitrary

char* strsplit(char* s, const char* delim) {
    char *piece;  // each time use new piece

    if (s == NULL)
        s = old_s;

    if (*s == '\0') {
        old_s = s;
        return NULL;
    }

    piece = s;
    s = strpbrk(piece, delim);  // finds location of next char in delim inside piece
    if (s == NULL)
        old_s = rawmemchr(piece, '\0');
    else {
        *s = '\0';
        old_s = s + 1;
    }
    return piece;
}


void initialize(FILE** infile) {
    char line[257];  // max of 256 char per line, 1 for null terminator
    char* buffer_1;  // p* used to hold temporary split strings
    char* buffer_2;  // used to deal with name separation
    int i = 0, j = 0;   // Counters, max j expected to be constant for each line

    for (; fgets(line, 256, *infile) != NULL; i++) {
        // only want to find students in class
        if (!strstr(line, class_find)) 
            continue;

        name_data* name = malloc(sizeof(name_data));
        master_storage[student_n] = name;
        student_n++;
        
        // printf("%s\n", line);
        char* new_line = strchr(line, '\n');
        if (new_line)  
            *new_line = '\0';  // removes newline at end
        buffer_1 = strsplit(line, " ,;:");  // splits string into tokens based on " ,;:/" char
        // if (NULL == buffer_1)
            // printf("buffer_1 is null\n");

        for (j = 0; buffer_1 != NULL; j++) {
            if (j == 0) {
                char* new_str = malloc(strlen(buffer_1)+1);  // temporary hoding string
                strcpy(new_str, buffer_1);
                name->utorid = new_str;
            }
            else if (j == 4) {
                char** lotsanames = name->names;
                buffer_2 = strtok(buffer_1, " ");
                for(int name_i = 0; buffer_2 != NULL; name_i++) {
                    char* a_name = malloc(strlen(buffer_2) + 1);
                    strcpy(a_name, buffer_2);
                    lotsanames[name_i] = a_name;
                    buffer_2 = strtok(NULL, " ");
                }
                break;
            }
            // printf("[Set %d, %d to %s]\n", i, j, buffer_1);
            buffer_1 = strsplit(NULL, ",;:");  // strsplit recycles memory, old overrides new
        }
    }
}

    /* Printing code for storing master_storage in file
    for(int i_recall = 0; i_recall < i; i_recall++) {
        for(int j_recall = 0; j_recall < 10; j_recall++) {
            // Maximum number of 10 fields per line
            printf("%d %d ", i_recall, j_recall);
            printf("%s\n", master_storage[i_recall][j_recall]);
        }
        printf("\n");
    }
    */

void play() {
    int student_n_temp = rand() % student_n;
    name_data* student = master_storage[student_n_temp];
    printf("Points: %d\nUtorid: %s\n", points, student->utorid);
}



int main(int argc, char** argv) {
    char* user_list = argv[1];
    FILE* infile = fopen(user_list, "r");
    initialize(&infile);

    while (1) {
        play();
    }



    return 0;
}

