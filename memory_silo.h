#ifndef MEMORY_SILO_H
#define MEMORY_SILO_H

#include <stdlib.h>
#include <stddef.h>
#include "io_socket.h" // Include for socket-related operations
#include "knowledge.h" // Include for knowledge graph integration

// Memory Silo Structure
typedef struct {
    int id;             // Unique identifier for the memory silo
    int io_socket;      // Associated IO socket for the silo
    void* memory;       // Pointer to the silo's memory
    size_t size;        // Size of the memory silo
} memory_silo_t;

// Function Declarations

/**
 * Initializes a new memory silo associated with the given IO socket.
 * @param io_socket The IO socket to associate with the memory silo.
 */
void memory_silo_init(int io_socket);

/**
 * Retrieves a memory silo by its associated IO socket.
 * @param io_socket The IO socket to search for.
 * @return A pointer to the matching memory silo, or NULL if not found.
 */
memory_silo_t* get_memory_silo(int io_socket);

/**
 * Writes data to the specified memory silo.
 * @param io_socket The IO socket associated with the silo.
 * @param buffer The data to write to the memory silo.
 * @param length The length of the data to write.
 */
void memory_silo_write(int io_socket, void* buffer, size_t length);

/**
 * Reads data from the specified memory silo.
 * @param io_socket The IO socket associated with the silo.
 * @param buffer The buffer to store the read data.
 * @param length The length of the data to read.
 */
void memory_silo_read(int io_socket, void* buffer, size_t length);

/**
 * Cleans up all memory silos, freeing associated resources.
 */
void cleanup_memory_silos();

#endif // MEMORY_SILO_H
