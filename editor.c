#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN 256

typedef struct {
    char **lines;
    int count;
    int capacity;
} LineEditor;

void init_editor(LineEditor *ed) {
    ed->capacity = 10;
    ed->count = 0;
    ed->lines = malloc(sizeof(char *) * ed->capacity);
}

void free_editor(LineEditor *ed) {
    for (int i = 0; i < ed->count; i++) {
        free(ed->lines[i]);
    }

    free(ed->lines);
    ed->count = 0;
    ed->capacity = 0;
}

void insert_line(LineEditor *ed, int line_num, const char *text) {
    if (line_num < 1 || line_num > ed->count + 1) {
        printf("Error: Line number out of range (1 to %d allowed).\n",
               ed->count + 1);
        return;
    }

    if (ed->count >= ed->capacity) {
        ed->capacity *= 2;
        ed->lines = realloc(ed->lines,
                            sizeof(char *) * ed->capacity);
    }

    int index = line_num - 1;

    for (int i = ed->count; i > index; i--) {
        ed->lines[i] = ed->lines[i - 1];
    }

    ed->lines[index] = malloc(strlen(text) + 1);
    strcpy(ed->lines[index], text);

    ed->count++;

    printf("Line %d inserted successfully.\n", line_num);
}

void delete_line(LineEditor *ed, int line_num) {
    if (ed->count == 0) {
        printf("Error: Document is empty.\n");
        return;
    }

    if (line_num < 1 || line_num > ed->count) {
        printf("Error: Invalid line number %d.\n", line_num);
        return;
    }

    int index = line_num - 1;

    free(ed->lines[index]);

    for (int i = index; i < ed->count - 1; i++) {
        ed->lines[i] = ed->lines[i + 1];
    }

    ed->count--;

    printf("Line %d deleted.\n", line_num);
}

void display_document(const LineEditor *ed) {
    if (ed->count == 0) {
        printf("[ Document is empty ]\n");
        return;
    }

    printf("\n--- Document Start ---\n");

    for (int i = 0; i < ed->count; i++) {
        printf("%3d | %s\n", i + 1, ed->lines[i]);
    }

    printf("--- Document End (%d lines) ---\n\n", ed->count);
}

void save_file(const LineEditor *ed, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (!fp) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < ed->count; i++) {
        fprintf(fp, "%s\n", ed->lines[i]);
    }

    fclose(fp);

    printf("Successfully saved to '%s'.\n", filename);
}

void load_file(LineEditor *ed, const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        printf("Error opening file '%s'.\n", filename);
        return;
    }

    free_editor(ed);
    init_editor(ed);

    char buffer[MAX_LINE_LEN];

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        insert_line(ed, ed->count + 1, buffer);
    }

    fclose(fp);

    printf("Successfully loaded from '%s'.\n", filename);
}

void show_stats(const LineEditor *ed) {
    int words = 0;

    for (int i = 0; i < ed->count; i++) {
        int in_word = 0;

        for (int j = 0; ed->lines[i][j] != '\0'; j++) {
            if (!isspace((unsigned char)ed->lines[i][j])) {
                if (!in_word) {
                    in_word = 1;
                    words++;
                }
            } else {
                in_word = 0;
            }
        }
    }

    printf("Document Stats: %d Lines | %d Words\n",
           ed->count, words);
}

int main(void) {
    LineEditor ed;

    init_editor(&ed);

    char input[MAX_LINE_LEN];

    printf("=== Simple Line Editor in C ===\n");
    printf("Type 'HELP' for commands.\n\n");

    while (1) {
        printf("editor> ");

        if (!fgets(input, sizeof(input), stdin))
            break;

        input[strcspn(input, "\r\n")] = 0;

        if (strlen(input) == 0)
            continue;

        char cmd[20];
        int arg1 = 0;
        char text[MAX_LINE_LEN] = "";

        if (sscanf(input, "%19s", cmd) != 1)
            continue;

        if (strcasecmp(cmd, "INSERT") == 0) {

            if (sscanf(input, "%*s %d %[^\n]",
                       &arg1, text) == 2) {

                insert_line(&ed, arg1, text);

            } else {
                printf("Usage: INSERT <line_number> <text>\n");
            }

        } else if (strcasecmp(cmd, "DELETE") == 0) {

            if (sscanf(input, "%*s %d", &arg1) == 1) {
                delete_line(&ed, arg1);
            } else {
                printf("Usage: DELETE <line_number>\n");
            }

        } else if (strcasecmp(cmd, "DISPLAY") == 0) {

            display_document(&ed);

        } else if (strcasecmp(cmd, "SAVE") == 0) {

            if (sscanf(input, "%*s %255s", text) == 1) {
                save_file(&ed, text);
            } else {
                printf("Usage: SAVE <filename>\n");
            }

        } else if (strcasecmp(cmd, "LOAD") == 0) {

            if (sscanf(input, "%*s %255s", text) == 1) {
                load_file(&ed, text);
            } else {
                printf("Usage: LOAD <filename>\n");
            }

        } else if (strcasecmp(cmd, "STATS") == 0) {

            show_stats(&ed);

        } else if (strcasecmp(cmd, "HELP") == 0) {

            printf("Available Commands:\n");
            printf("  INSERT <line_num> <text> : Insert text at line_num\n");
            printf("  DELETE <line_num>        : Delete line at line_num\n");
            printf("  DISPLAY                  : View whole document\n");
            printf("  SAVE <filename>          : Save document to file\n");
            printf("  LOAD <filename>          : Load document from file\n");
            printf("  STATS                    : Show word/line count\n");
            printf("  EXIT                     : Quit editor\n");

        } else if (strcasecmp(cmd, "EXIT") == 0) {

            break;

        } else {

            printf("Unknown command. Type 'HELP' for options.\n");
        }
    }

    free_editor(&ed);

    printf("Goodbye!\n");

    return 0;
}