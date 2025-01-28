#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory Silo Structure
typedef struct {
    int id;
    int io_socket;
    void* memory;
} memory_silo_t;

// Global memory silo list
memory_silo_t* memory_silos = NULL;  // This will hold memory silos
int memory_silo_count = 0;           // Track the number of memory silos

// Function to Initialize Memory Silo
void memory_silo_init(int io_socket) {
    // Allocate memory for a new memory silo
    memory_silo_t* new_silo = malloc(sizeof(memory_silo_t));
    if (new_silo == NULL) {
        perror("Failed to allocate memory for memory silo");
        exit(EXIT_FAILURE);
    }

    // Initialize the silo properties
    new_silo->id = memory_silo_count + 1;
    new_silo->io_socket = io_socket;
    new_silo->memory = malloc(1024);  // Allocate memory for the silo (1KB for example)
    if (new_silo->memory == NULL) {
        perror("Failed to allocate memory for silo data");
        free(new_silo);
        exit(EXIT_FAILURE);
    }

    // Add to global memory silos list
    memory_silos = realloc(memory_silos, sizeof(memory_silo_t) * (memory_silo_count + 1));
    if (memory_silos == NULL) {
        perror("Failed to resize memory silo list");
        exit(EXIT_FAILURE);
    }
    memory_silos[memory_silo_count] = *new_silo;
    memory_silo_count++;

    // Clean up the new_silo pointer as it's now stored in the list
    free(new_silo);
}

// Function to Read from Memory Silo
void memory_silo_read(int io_socket, void* buffer, int length) {
    memory_silo_t* silo = get_memory_silo(io_socket);
    if (silo == NULL) {
        fprintf(stderr, "Memory silo not found for IO socket %d\n", io_socket);
        return;
    }
    memcpy(buffer, silo->memory, length);
}

// Function to Write to Memory Silo
void memory_silo_write(int io_socket, void* buffer, int length) {
    memory_silo_t* silo = get_memory_silo(io_socket);
    if (silo == NULL) {
        fprintf(stderr, "Memory silo not found for IO socket %d\n", io_socket);
        return;
    }
    memcpy(silo->memory, buffer, length);
}

// Function to Get Memory Silo by IO Socket
memory_silo_t* get_memory_silo(int io_socket) {
    for (int i = 0; i < memory_silo_count; i++) {
        if (memory_silos[i].io_socket == io_socket) {
            return &memory_silos[i];
        }
    }
    return NULL;  // Return NULL if not found
}

// Function to Clean up Memory Silos
void cleanup_memory_silos() {
    for (int i = 0; i < memory_silo_count; i++) {
        free(memory_silos[i].memory);
    }
    free(memory_silos);
    memory_silos = NULL;
    memory_silo_count = 0;
}

int main() {
    // Example usage
    int io_socket1 = 1;
    int io_socket2 = 2;

    // Initialize memory silos
    memory_silo_init(io_socket1);
    memory_silo_init(io_socket2);

    // Write data to the first silo
    char data[] = "Hello, Memory Silo 1!";
    memory_silo_write(io_socket1, data, sizeof(data));

    // Read data from the first silo
    char buffer[1024];
    memory_silo_read(io_socket1, buffer, sizeof(data));
    printf("Data read from Memory Silo 1: %s\n", buffer);

    // Clean up allocated memory
    cleanup_memory_silos();

    return 0;
}
