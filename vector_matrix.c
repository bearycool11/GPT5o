#include <stdio.h>
#include <stdlib.h>
#include "vector_matrix.h"
#include "knowledge.h" // Integration with Knowledge Graph system

// Function to Initialize a Vector Matrix
vector_matrix_t* init_vector_matrix(int rows, int cols) {
    vector_matrix_t* matrix = (vector_matrix_t*)malloc(sizeof(vector_matrix_t));
    if (!matrix) {
        perror("Memory allocation failed for vector matrix");
        exit(EXIT_FAILURE);
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = (int**)malloc(rows * sizeof(int*));
    if (!matrix->data) {
        perror("Memory allocation failed for matrix rows");
        free(matrix);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        matrix->data[i] = (int*)malloc(cols * sizeof(int));
        if (!matrix->data[i]) {
            perror("Memory allocation failed for matrix columns");
            for (int j = 0; j < i; j++) {
                free(matrix->data[j]);
            }
            free(matrix->data);
            free(matrix);
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

// Function to Free a Vector Matrix
void free_vector_matrix(vector_matrix_t* matrix) {
    if (!matrix) return;

    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}

// Function to Populate the Vector Matrix with Values
void populate_vector_matrix(vector_matrix_t* matrix, int* values) {
    if (!matrix || !values) {
        fprintf(stderr, "Invalid matrix or values for population\n");
        return;
    }

    int index = 0;
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            matrix->data[i][j] = values[index++];
        }
    }
}

// Function to Update the Knowledge Graph with Matrix Operations
void update_knowledge_graph(vector_matrix_t* matrix) {
    if (!matrix) {
        fprintf(stderr, "Invalid matrix for knowledge graph update\n");
        return;
    }

    char node_name[256];
    snprintf(node_name, sizeof(node_name), "Matrix_%dx%d", matrix->rows, matrix->cols);

    // Example: Aggregate matrix data for knowledge graph
    int sum = 0;
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            sum += matrix->data[i][j];
        }
    }

    // Create a knowledge graph node
    add_knowledge_node(node_name, "MatrixEntity");

    // Add a relationship indicating the sum of matrix elements
    char relationship[256];
    snprintf(relationship, sizeof(relationship), "Sum=%d", sum);
    add_knowledge_relationship("Root", node_name, relationship);

    printf("Knowledge graph updated with node: %s, relationship: %s\n", node_name, relationship);
}

// PMLL Logic Loop Integration for Processing
void pmll_vector_matrix_process(vector_matrix_t* matrix) {
    if (!matrix) {
        fprintf(stderr, "Invalid matrix for processing\n");
        return;
    }

    printf("Processing the vector matrix...\n");

    // Example: Matrix Multiplication (Square Matrix)
    if (matrix->rows == matrix->cols) {
        vector_matrix_t* result = init_vector_matrix(matrix->rows, matrix->cols);

        for (int i = 0; i < matrix->rows; i++) {
            for (int j = 0; j < matrix->cols; j++) {
                result->data[i][j] = 0;
                for (int k = 0; k < matrix->cols; k++) {
                    result->data[i][j] += matrix->data[i][k] * matrix->data[k][j];
                }
            }
        }

        printf("Resultant Matrix:\n");
        for (int i = 0; i < result->rows; i++) {
            for (int j = 0; j < result->cols; j++) {
                printf("%d ", result->data[i][j]);
            }
            printf("\n");
        }

        // Update the Knowledge Graph with the result
        update_knowledge_graph(result);

        free_vector_matrix(result);
    } else {
        printf("Non-square matrices are currently not supported for this operation.\n");
    }
}

// Main Function for Testing
int main() {
    // Example values for the matrix
    int rows = 3;
    int cols = 3;
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Initialize and populate the vector matrix
    vector_matrix_t* matrix = init_vector_matrix(rows, cols);
    populate_vector_matrix(matrix, values);

    printf("Input Matrix:\n");
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%d ", matrix->data[i][j]);
        }
        printf("\n");
    }

    // Process the matrix and integrate with the knowledge graph
    pmll_vector_matrix_process(matrix);

    // Free allocated resources
    free_vector_matrix(matrix);

    return 0;
}
