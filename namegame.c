#define _GNU_SOURCE  // defines rawmemchr
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct name_data {
    char* utorid;
    char* names[10];
    int split_n;
} name_data;

// globals
char* class_find;
char* old_s;  // each time keep same pointer
int points = 0;
int student_n = 0;
name_data* master_storage[100000];  // value is arbitrary

// string splitting based off of strtok but returns '\0' on consecutive delimiter matches
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


void initialize(FILE** infile, char* class_find) {
    char line[257];  // max of 256 char per line, 1 for null terminator
    char* buffer_1;  // p* used to hold temporary split strings
    char* buffer_2;  // used to deal with name separation

    for (; fgets(line, 256, *infile) != NULL;) {
        // only want to find students in class
        if (!strstr(line, class_find))  // returns 0 on match
            continue;

        // only create a name_data object for each valid person
        name_data* name = malloc(sizeof(name_data));
        master_storage[student_n] = name;
        student_n++;
        
        // printf("%s\n", line);
        char* new_line = strchr(line, '\n');
        if (new_line)  
            *new_line = '\0';  // removes newline at end
        buffer_1 = strsplit(line, " ,;:");  // splits string into tokens based on " ,;:/" char
        name->split_n = 0;

        for (int field = 0; buffer_1 != NULL; field++) {
            if (field == 0) {
                char* new_str = malloc(strlen(buffer_1)+1);  // temporary hoding string
                strcpy(new_str, buffer_1);
                name->utorid = new_str;
            }
            else if (field == 4) {
                char** lotsanames = name->names;
                buffer_2 = strtok(buffer_1, " ");
                for(int name_i = 0; buffer_2 != NULL; name_i++) {
                    char* a_name = malloc(strlen(buffer_2) + 1);
                    strcpy(a_name, buffer_2);
                    lotsanames[name_i] = a_name;
                    buffer_2 = strtok(NULL, " ");
                    name->split_n++;
                }
                break;
            }
            buffer_1 = strsplit(NULL, ",;:");  // strsplit recycles memory, old overrides new
        }
    }
    printf("Population of filtered group: %d\n", student_n);
}

int play() {
    if (!student_n) {
        printf("Your user list doesn't contain any matching members");
        return 0;
    }
        
    int student_n_temp = rand() % student_n;
    name_data* student = master_storage[student_n_temp];

    char user_guess[80];
    char* user_guesses[80];
    // use temporary array to hold student names to avoid repeating entries
    char* buffer_guess;
    char* names_temp[80];
    memmove(names_temp, student->names, 80*sizeof(char*));
    printf("Points: %d\nUtorid: %s\n", points, student->utorid);
    printf("Can you remember their name: ");

    // scans for user guess
    fgets(user_guess, 100, stdin);
    strtok(user_guess, "\n");

    // splits user guess into first, last, middle, etc names
    buffer_guess = strtok(user_guess, " ");
    int guess_count = 0;
    for(; buffer_guess != NULL; guess_count++) {
        char* a_name = malloc(strlen(buffer_guess) + 1);
        strcpy(a_name, buffer_guess);
        user_guesses[guess_count] = a_name;
        buffer_guess = strtok(NULL, " ");
    }

    // compares user guesses and real names
    int matches = 0;
    for(int i = 0; i < guess_count; i++) {
        if (user_guesses[i][0] > 96)
            user_guesses[i][0] -= 32;    // if lower case, then change to upper case
        for(int j = 0; j < student->split_n; j++) {
            // printf("[%s] [%s]\n", user_guesses[i], names_temp[j]);
            if (!strcmp(names_temp[j], user_guesses[i])) {
                names_temp[j] = "";
                matches++;
            }
        }
    }
    // gives real name
    printf("Real name: ");
    // uses original array of names since temp one has removed elements
    for (int i = 0; i < master_storage[student_n_temp]->split_n; i++) 
        printf("%s ", master_storage[student_n_temp]->names[i]);
    printf("\n");

    if (!matches)
        return 0;
    points += 10 * matches;
    return 1;
}

const char* messages[] = {
    "Wow, I'm glad my name is simpler", // 0
    "You pass!", // 50
    "You have asian leveled up!", // 100
    "Wow, factor of safety of 1.5", // 150
    "Alas, your journey ended over 200 hero ...", // 200
    "Keep going, quarter to a thousand!", // 250
    "Memory spartan!", // 300
    "That's enough", // 350
    "You're probably addicted (420?)", // 400
    "You don't listen, do you?", // 450
    "You need professional help", // 500
    "You are the professional help", // 550
    "You've won the game :D", // 600
    "You sold your soul to the devil (666)" // 650
};

int main(int argc, char** argv) {
    char* user_list = argv[1];
    FILE* infile = fopen(user_list, "r");
    char class_find[80];

    printf("Group to filter (: for all, esc1t7 by default): ");
    fgets(class_find, 79, stdin);
    class_find[strlen(class_find) - 1] = '\0';
    if (!*class_find)
        strcpy(class_find, "esc1t7"); // esc1t7 is default group

    initialize(&infile, class_find);
    srand(time(NULL));  // seed random generator

    while (play())  // returns 0 if no matches and stops playing 
        printf("---------------------------------------------------------\n");

    // TODO will add shareable high score
    int power_level = points / 50;
    printf("Score: %d\n", points);
    if (power_level >= sizeof(messages)/sizeof(char*))
        power_level = sizeof(messages)/sizeof(char*) - 1;
    printf("%s\n", messages[power_level]);

    return 0;
}

