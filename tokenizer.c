#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_SIZE 100

void tokenize(char *input) {
    char *token = strtok(input, " \t\n");  // Delimiters: space, tab, newline
    while (token != NULL) {
        printf("Token: %s\n", token);
        token = strtok(NULL, " \t\n");
    }
}

int main() {
    char input[MAX_TOKEN_SIZE];

    // Get input string
    printf("Enter a string to tokenize: ");
    fgets(input, MAX_TOKEN_SIZE, stdin);

    // Remove newline character
    input[strcspn(input, "\n")] = '\0';

    // Tokenize the input string
    tokenize(input);

    return 0;
}
