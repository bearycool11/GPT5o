#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Test function to run the PMLL solver and MiniSat solver, and compare results
void run_tests(int num_variables, int num_clauses) {
    char filename[256];
    sprintf(filename, "3sat_instance_%d_%d.cnf", num_variables, num_clauses);

    // Step 1: Generate random SAT instance
    generate_3sat_instance(num_variables, num_clauses, filename);

    // Open a log file to write the performance data
    FILE *log_file = fopen("performance_log.txt", "w");
    if (!log_file) {
        perror("Failed to open log file");
        return;
    }

    // Step 2: Run the PMLL algorithm (SAT Solver)
    clock_t start_time = clock();
    int **formula = load_cnf(filename);  // Load the generated CNF formula (to be implemented)
    bool pml_result = PMLL_SAT_Solver(formula, num_clauses, num_variables, log_file);
    clock_t end_time = clock();
    double pml_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("PMLL Algorithm: %s (Time: %f seconds)\n", pml_result ? "Satisfiable" : "Unsatisfiable", pml_time);

    // Step 3: Run MiniSat solver and measure time
    start_time = clock();
    run_miniSat(filename);  // Run MiniSat solver (see your MiniSat integration code)
    end_time = clock();
    double miniSat_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("MiniSat: Time: %f seconds\n", miniSat_time);

    // Close the log file
    fclose(log_file);

    // Free resources
    free_formula(formula);  // Implement memory cleanup
}

int main() {
    int num_variables = 50;
    int num_clauses = 150;
    run_tests(num_variables, num_clauses);
    return 0;
}
