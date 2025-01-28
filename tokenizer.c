#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_INPUT_SIZE 100

// Function to check if a byte is the start of a UTF-8 character
bool is_utf8_start_byte(unsigned char byte) {
    return (byte & 0xC0) != 0x80; // Start byte does not have the pattern 10xxxxxx
}

// Function to tokenize a UTF-8 string
void utf8_tokenize(const char *input) {
    const char *delimiters = " \t\n"; // Define delimiters (space, tab, newline)
    const char *start = input;
    const char *current = input;

    printf("Tokenizing input: \"%s\"\n", input);

    while (*current) {
        // Check if the current character is a delimiter
        if (strchr(delimiters, *current) != NULL) {
            // Print the token if it's valid
            if (current > start) {
                printf("Token: ");
                fwrite(start, 1, current - start, stdout); // Print token without null-terminator
                printf("\n");
            }
            // Move past the delimiter and reset the token start
            current++;
            start = current;
        } else {
            // Handle UTF-8 character traversal
            if (is_utf8_start_byte((unsigned char)*current)) {
                current++;
                while (*current && (*current & 0xC0) == 0x80) {
                    current++; // Continue traversing multi-byte UTF-8 character
                }
            } else {
                current++;
            }
        }
    }

    // Print the last token if there is one
    if (current > start) {
        printf("Token: ");
        fwrite(start, 1, current - start, stdout);
        printf("\n");
    }
}

int main() {
    char input[MAX_INPUT_SIZE];

    printf("Enter a string to tokenize (supports UTF-8): ");
    if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return EXIT_FAILURE;
    }
    input[strcspn(input, "\n")] = '\0'; // Remove the newline character

    utf8_tokenize(input);

    return 0;
}
