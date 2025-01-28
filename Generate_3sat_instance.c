#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random 3-SAT instance
void generate_3sat_instance(int num_variables, int num_clauses, const char* filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    // Write the header for the 3-SAT CNF instance
    fprintf(file, "p cnf %d %d\n", num_variables, num_clauses);

    // Randomly generate clauses
    srand(time(0));  // Seed the random number generator

    for (int i = 0; i < num_clauses; ++i) {
        // Each clause consists of 3 literals, choose them randomly
        for (int j = 0; j < 3; ++j) {
            int var = rand() % num_variables + 1;  // Variable between 1 and num_variables
            if (rand() % 2 == 0) {
                var = -var;  // Randomly negate the variable
            }
            fprintf(file, "%d ", var);
        }
        fprintf(file, "0\n");  // End of clause
    }

    fclose(file);
    printf("Generated random 3-SAT instance: %s\n", filename);
}

int main() {
    int num_variables = 50;  // Number of variables
    int num_clauses = 150;   // Number of clauses
    const char* filename = "3sat_instance.cnf";  // Output file name
    generate_3sat_instance(num_variables, num_clauses, filename);  // Generate instance
    return 0;
}
