#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to run the external MiniSat solver with performance logging
void run_miniSat(const char* filename, FILE *log_file) {
    // Start measuring the execution time of MiniSat
    clock_t start_time = clock();

    char command[256];
    sprintf(command, "minisat %s result.txt", filename);  // Assuming 'minisat' is in the system path
    int result = system(command);

    // End measuring the execution time of MiniSat
    clock_t end_time = clock();
    double miniSat_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Read the result from the result.txt file generated by MiniSat
    FILE *result_file = fopen("result.txt", "r");
    if (!result_file) {
        perror("Failed to open result file");
        return;
    }

    char result_str[20];
    fgets(result_str, sizeof(result_str), result_file);
    if (strstr(result_str, "SATISFIABLE") != NULL) {
        fprintf(log_file, "MiniSat: Satisfiable (Time: %.4f seconds)\n", miniSat_time);
        printf("MiniSat: Satisfiable\n");
    } else {
        fprintf(log_file, "MiniSat: Unsatisfiable (Time: %.4f seconds)\n", miniSat_time);
        printf("MiniSat: Unsatisfiable\n");
    }

    fclose(result_file);
}
