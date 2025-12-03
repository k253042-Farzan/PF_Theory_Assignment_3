#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define MAX_LINE 5000   // safe max temp input

// Insert a line at given index
void insertLine(char ***lines, int *count, int *capacity, int index, const char *text) {

    if (*count >= *capacity) {
        *capacity *= 2;
        char **temp = realloc(*lines, (*capacity) * sizeof(char*));
        if (!temp) {
            fprintf(stderr, "Memory allocation failed during insert.\n");
            exit(1);
        }
        *lines = temp;
    }

    memmove(&(*lines)[index + 1], &(*lines)[index], (*count - index) * sizeof(char*));

    (*lines)[index] = malloc(strlen(text) + 1);
    if (!(*lines)[index]) {
        fprintf(stderr, "Line malloc failed.\n");
        exit(1);
    }
    strcpy((*lines)[index], text);

    (*count)++;
}

// Delete a line at index
void deleteLine(char ***lines, int *count, int index) {

    free((*lines)[index]);

    memmove(&(*lines)[index], &(*lines)[index + 1], (*count - index - 1) * sizeof(char*));

    (*count)--;
}

// Print all stored lines
void printAllLines(char **lines, int count) {
    for (int i = 0; i < count; i++)
        printf("%d: %s", i, lines[i]);
}

// Free all allocated memory
void freeAll(char **lines, int count) {
    for (int i = 0; i < count; i++)
        free(lines[i]);
    free(lines);
}

// Reduce capacity to exact count
void shrinkToFit(char ***lines, int count) {
    char **temp = realloc(*lines, count * sizeof(char*));
    if (!temp) {
        fprintf(stderr, "shrinkToFit failed.\n");
        exit(1);
    }
    *lines = temp;
}

// Save buffer to file
void saveToFile(char **lines, int count, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Cannot open file.\n");
        return;
    }
    for (int i = 0; i < count; i++)
        fprintf(fp, "%s", lines[i]);
    fclose(fp);
}

// Load buffer from file
void loadFromFile(char ***lines, int *count, int *capacity, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file.\n");
        return;
    }

    *count = 0;
    char buffer[MAX_LINE];

    while (fgets(buffer, sizeof(buffer), fp)) {
        insertLine(lines, count, capacity, *count, buffer);
    }

    fclose(fp);
}

int main() {
    char **lines = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!lines) {
        fprintf(stderr, "Initial malloc failed.\n");
        return 1;
    }

    int count = 0, capacity = INITIAL_CAPACITY;
    char input[MAX_LINE];
    int choice, index;

    while (1) {
        printf("\n1.Insert  2.Delete  3.Print  4.Save  5.Load  6.Shrink  7.Exit\n");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            printf("Enter line index: ");
            scanf("%d", &index);
            getchar();

            printf("Enter text: ");
            fgets(input, sizeof(input), stdin);

            insertLine(&lines, &count, &capacity, index, input);
        }
        else if (choice == 2) {
            printf("Enter index to delete: ");
            scanf("%d", &index);
            getchar();
            deleteLine(&lines, &count, index);
        }
        else if (choice == 3) {
            printAllLines(lines, count);
        }
        else if (choice == 4) {
            saveToFile(lines, count, "output.txt");
        }
        else if (choice == 5) {
            loadFromFile(&lines, &count, &capacity, "output.txt");
        }
        else if (choice == 6) {
            shrinkToFit(&lines, count);
        }
        else if (choice == 7) {
            break;
        }
    }

    freeAll(lines, count);
    return 0;
}