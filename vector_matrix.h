#ifndef VECTOR_MATRIX_H
#define VECTOR_MATRIX_H

#include <stddef.h> // For size_t

// Vector Matrix Structure
typedef struct {
    int rows;         // Number of rows
    int cols;         // Number of columns
    int** data;       // Pointer to the matrix data
} vector_matrix_t;

/**
 * Initializes a vector matrix with the specified dimensions.
 * @param rows Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 * @return Pointer to the initialized vector matrix.
 */
vector_matrix_t* init_vector_matrix(int rows, int cols);

/**
 * Frees the memory allocated for a vector matrix.
 * @param matrix Pointer to the vector matrix to be freed.
 */
void free_vector_matrix(vector_matrix_t* matrix);

/**
 * Populates a vector matrix with values from an array.
 * @param matrix Pointer to the vector matrix.
 * @param values Array of integers to populate the matrix.
 */
void populate_vector_matrix(vector_matrix_t* matrix, int* values);

/**
 * Updates the knowledge graph with information from the vector matrix.
 * @param matrix Pointer to the vector matrix.
 */
void update_knowledge_graph(vector_matrix_t* matrix);

/**
 * Processes the vector matrix and performs operations (e.g., matrix multiplication).
 * @param matrix Pointer to the vector matrix.
 */
void pmll_vector_matrix_process(vector_matrix_t* matrix);

#endif // VECTOR_MATRIX_H
