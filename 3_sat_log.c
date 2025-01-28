#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to log results to a file
void log_results(const char *solver_name, const char *instance_name, bool result, double time_taken, int iterations) {
    FILE *log_file = fopen("sat_comparison_log.txt", "a");
    if (log_file == NULL) {
        perror("Failed to open log file");
        return;
    }

    fprintf(log_file, "Solver: %s, Instance: %s, Result: %s, Time: %f seconds, Iterations: %d\n",
            solver_name, instance_name, result ? "SATISFIABLE" : "UNSATISFIABLE", time_taken, iterations);

    fclose(log_file);
}

// Function to run tests for both PMLL and MiniSat solvers
void run_comparison_tests(int num_variables, int num_clauses) {
    char filename[256];
    sprintf(filename, "3sat_instance_%d_%d.cnf", num_variables, num_clauses);

    // Step 1: Generate random SAT instance
    generate_3sat_instance(num_variables, num_clauses, filename);

    // Step 2: Run the PMLL algorithm (SAT Solver)
    clock_t start_time = clock();
    int **formula = load_cnf(filename);  // Load the generated CNF formula (to be implemented)
    bool pml_result = PMLL_SAT_Solver(formula, num_clauses, num_variables);
    clock_t end_time = clock();
    double pml_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    int pml_iterations = get_pml_iterations();  // Assuming this function tracks the number of iterations
    log_results("PMLL", filename, pml_result, pml_time, pml_iterations);

    printf("PMLL Algorithm: %s (Time: %f seconds, Iterations: %d)\n", 
            pml_result ? "Satisfiable" : "Unsatisfiable", pml_time, pml_iterations);

    // Step 3: Run MiniSat solver and measure time
    start_time = clock();
    run_miniSat(filename);
    end_time = clock();
    double miniSat_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    log_results("MiniSat", filename, pml_result, miniSat_time, -1); // Iterations are not available from MiniSat directly

    printf("MiniSat: Time: %f seconds\n", miniSat_time);

    // Free resources
    free_formula(formula);  // Implement memory cleanup
}

int main() {
    // Set the number of variables and clauses for the test
    int num_variables = 50;
    int num_clauses = 150;

    // Run the comparison tests
    run_comparison_tests(num_variables, num_clauses);

    return 0;
}
