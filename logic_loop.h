#ifndef LOGIC_LOOP_H
#define LOGIC_LOOP_H

#include "pml.h" // Includes the PML structure and associated functions
#include <stddef.h> // For size_t

// Constants
#define MAX_ITERATIONS 1000  // Maximum iterations for the logic loop
#define BUFFER_SIZE 1024     // Buffer size for data handling

// Function Declarations

/**
 * Initializes the logic loop.
 * @param pml A pointer to the PMLL structure.
 * @return 0 on success, -1 on failure.
 */
int initialize_logic_loop(PMLL* pml);

/**
 * Executes the main logic loop.
 * @param pml A pointer to the PMLL structure.
 */
void execute_logic_loop(PMLL* pml);

/**
 * Cleans up resources used during the logic loop.
 * @param pml A pointer to the PMLL structure.
 */
void cleanup_logic_loop(PMLL* pml);

/**
 * Consolidates memory across silos using the provided pipe.
 * @param pml A pointer to the PMLL structure.
 * @param pipefd A file descriptor array for inter-process communication.
 */
void consolidate_memory(PMLL* pml, int pipefd[2]);

/**
 * Updates the consolidated memory graph with new data.
 * @param pml A pointer to the PMLL structure.
 * @param buffer The buffer containing memory graph update data.
 */
void update_consolidated_memory_graph(PMLL* pml, const char* buffer);

/**
 * Logs a message to the logic loop system.
 * @param level The severity level (e.g., "INFO", "ERROR").
 * @param message The message to log.
 */
void log_loop_message(const char* level, const char* message);

#endif // LOGIC_LOOP_H
