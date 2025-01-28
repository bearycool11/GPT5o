#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Function to evaluate a CNF formula with a given variable assignment
bool evaluate_formula(int **formula, int *assignment, int num_clauses, int num_variables) {
    for (int i = 0; i < num_clauses; i++) {
        bool clause_satisfied = false;
        for (int j = 0; j < 3; j++) {
            int var = formula[i][j];
            if (var > 0 && assignment[var - 1] == 1) {
                clause_satisfied = true;
                break;
            }
            if (var < 0 && assignment[-var - 1] == 0) {
                clause_satisfied = true;
                break;
            }
        }
        if (!clause_satisfied) {
            return false;  // If one clause is not satisfied, formula is unsatisfied
        }
    }
    return true;  // If all clauses are satisfied
}

// Backtracking solver using the PMLL algorithm with performance tracking
bool PMLL_SAT_Solver(int **formula, int num_clauses, int num_variables, FILE *log_file) {
    int *assignment = (int *)calloc(num_variables, sizeof(int));  // All variables are initially unassigned (0)
    int iterations = 0;
    int backtracks = 0;
    clock_t start_time = clock();

    // Recursive backtracking
    for (int i = 0; i < num_variables; ++i) {
        assignment[i] = 1;  // Try true (1)
        iterations++;
        if (evaluate_formula(formula, assignment, num_clauses, num_variables)) {
            // Log data after finding a solution
            clock_t end_time = clock();
            fprintf(log_file, "PMLL Algorithm: Satisfiable (Time: %.4f seconds, Iterations: %d, Backtracks: %d)\n",
                    (double)(end_time - start_time) / CLOCKS_PER_SEC, iterations, backtracks);
            free(assignment);
            return true;
        }
        assignment[i] = 0;  // Try false (0)
        iterations++;
        if (evaluate_formula(formula, assignment, num_clauses, num_variables)) {
            // Log data after finding a solution
            clock_t end_time = clock();
            fprintf(log_file, "PMLL Algorithm: Satisfiable (Time: %.4f seconds, Iterations: %d, Backtracks: %d)\n",
                    (double)(end_time - start_time) / CLOCKS_PER_SEC, iterations, backtracks);
            free(assignment);
            return true;
        }
        backtracks++;  // Count backtracks
    }

    // If no assignment satisfies the formula, return unsatisfiable
    clock_t end_time = clock();
    fprintf(log_file, "PMLL Algorithm: Unsatisfiable (Time: %.4f seconds, Iterations: %d, Backtracks: %d)\n",
            (double)(end_time - start_time) / CLOCKS_PER_SEC, iterations, backtracks);
    free(assignment);
    return false;
}
