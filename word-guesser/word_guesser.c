#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

int get_line_count(char filename[])
{
    int c;
    long total_lines = 0;
    FILE *pF = fopen(filename, "r");

    if (pF == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    while ((c = fgetc(pF)) != EOF)
    {
        if (c == '\n')
        {
            total_lines++;
        }
    }

    return total_lines;
}

int get_random_number(int max)
{
    int random_number;
    srand(time(NULL));
    random_number = rand() % max;
    return random_number;
}

char *get_random_word(char filename[], int line_number)
{
    int current_line = 0, c = 0;
    FILE *pF = fopen(filename, "r");

    while ((c = fgetc(pF)) != EOF)
    {
        if (c == '\n')
        {
            if (++current_line == line_number)
            {
                int index = 0;
                char *random_word = NULL;
                random_word = malloc(sizeof(char) * 7);

                while ((c = fgetc(pF)) != EOF && c != '\n')
                {
                    if (index < 7)
                    {
                        random_word[index++] = c;
                    }
                }
                return random_word;
            }
        }
    }
}


int main(void)
{
    int total_lines_in_file;
    int random_line_number;
    char *chosen_word;
    char filename[20] = "all_words.txt";
    total_lines_in_file = get_line_count(filename);
    printf("Total lines in %s: %d\n", filename, total_lines_in_file);
    printf("Going to pick a random word from %s\n", filename);

    random_line_number = get_random_number(total_lines_in_file);
    printf("Random line number: %d\n", random_line_number);

    chosen_word = get_random_word(filename, random_line_number);
    for (int i = 0; i < strlen(chosen_word); i++) {
        chosen_word[i] = tolower(chosen_word[i]);
    }
 
    printf("Chosen word: %s\n", chosen_word);
}