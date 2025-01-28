#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

// Backtracking solver using the PMLL algorithm
bool PMLL_SAT_Solver(int **formula, int num_clauses, int num_variables) {
    int *assignment = (int *)calloc(num_variables, sizeof(int));  // All variables are initially unassigned (0)

    // Recursive backtracking
    for (int i = 0; i < num_variables; ++i) {
        assignment[i] = 1;  // Try true (1)
        if (evaluate_formula(formula, assignment, num_clauses, num_variables)) {
            free(assignment);
            return true;
        }
        assignment[i] = 0;  // Try false (0)
        if (evaluate_formula(formula, assignment, num_clauses, num_variables)) {
            free(assignment);
            return true;
        }
    }

    free(assignment);
    return false;  // If no assignment satisfies the formula, return unsatisfiable
}
