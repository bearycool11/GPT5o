#include "memory_silo.h"
#include "knowledge.h" // Include knowledge graph operations
#include "io_socket.h" // Include IO socket for API communication
#include "unified_voice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory Silo Structure
typedef struct {
    int id;
    int io_socket;
    void* memory;
    size_t size;
} memory_silo_t;

// Global memory silo list
memory_silo_t* memory_silos = NULL;
int memory_silo_count = 0;

// Function to Initialize Memory Silo
void memory_silo_init(int io_socket) {
    memory_silo_t* new_silo = malloc(sizeof(memory_silo_t));
    if (new_silo == NULL) {
        perror("Failed to allocate memory for memory silo");
        exit(EXIT_FAILURE);
    }

    // Initialize the silo
    new_silo->id = memory_silo_count + 1;
    new_silo->io_socket = io_socket;
    new_silo->size = 1024;  // Example size: 1KB
    new_silo->memory = malloc(new_silo->size);
    if (new_silo->memory == NULL) {
        perror("Failed to allocate memory for silo data");
        free(new_silo);
        exit(EXIT_FAILURE);
    }

    // Add to global list
    memory_silos = realloc(memory_silos, sizeof(memory_silo_t) * (memory_silo_count + 1));
    if (memory_silos == NULL) {
        perror("Failed to resize memory silo list");
        free(new_silo->memory);
        free(new_silo);
        exit(EXIT_FAILURE);
    }

    memory_silos[memory_silo_count] = *new_silo;
    memory_silo_count++;
    free(new_silo);  // Free the temporary allocation
}

// Function to Get Memory Silo
memory_silo_t* get_memory_silo(int io_socket) {
    for (int i = 0; i < memory_silo_count; i++) {
        if (memory_silos[i].io_socket == io_socket) {
            return &memory_silos[i];
        }
    }
    return NULL;
}

// Function to Write to Memory Silo
void memory_silo_write(int io_socket, void* buffer, size_t length) {
    memory_silo_t* silo = get_memory_silo(io_socket);
    if (silo == NULL) {
        fprintf(stderr, "Memory silo not found for IO socket %d\n", io_socket);
        return;
    }
    if (length > silo->size) {
        fprintf(stderr, "Error: Write length exceeds memory silo capacity\n");
        return;
    }
    memcpy(silo->memory, buffer, length);

    // Update the knowledge graph with written data
    Graph* knowledge_graph = get_global_knowledge_graph();
    if (knowledge_graph) {
        char* node_name = "Silo Update";
        Node* node = create_node(node_name, silo->id);
        add_node(knowledge_graph, node);
        printf("Knowledge graph updated with silo write operation.\n");
    }
}

// Function to Read from Memory Silo
void memory_silo_read(int io_socket, void* buffer, size_t length) {
    memory_silo_t* silo = get_memory_silo(io_socket);
    if (silo == NULL) {
        fprintf(stderr, "Memory silo not found for IO socket %d\n", io_socket);
        return;
    }
    if (length > silo->size) {
        fprintf(stderr, "Error: Read length exceeds memory silo capacity\n");
        return;
    }
    memcpy(buffer, silo->memory, length);

    // Log the read operation to the knowledge graph
    Graph* knowledge_graph = get_global_knowledge_graph();
    if (knowledge_graph) {
        char* node_name = "Silo Read";
        Node* node = create_node(node_name, silo->id);
        add_node(knowledge_graph, node);
        printf("Knowledge graph updated with silo read operation.\n");
    }
}

// Function to Sync Memory Silo with API via Socket
void sync_memory_silo_with_api(io_socket_t* api_socket, int silo_id) {
    memory_silo_t* silo = get_memory_silo(api_socket->socket);
    if (!silo) {
        fprintf(stderr, "No silo found for socket %d\n", api_socket->socket);
        return;
    }

    // Send silo data to the API
    printf("Syncing memory silo %d with API...\n", silo_id);
    if (io_socket_send(api_socket->socket, silo->memory, silo->size) < 0) {
        fprintf(stderr, "Failed to sync memory silo with API\n");
        return;
    }

    // Receive response from API and update knowledge graph
    char buffer[1024];
    if (io_socket_receive(api_socket->socket, buffer, sizeof(buffer)) > 0) {
        Graph* knowledge_graph = get_global_knowledge_graph();
        if (knowledge_graph) {
            Node* node = create_node(buffer, silo_id);
            add_node(knowledge_graph, node);
            printf("Knowledge graph updated with API response: %s\n", buffer);
        }
    }
}

// Function to Cleanup Memory Silos
void cleanup_memory_silos() {
    for (int i = 0; i < memory_silo_count; i++) {
        free(memory_silos[i].memory);
    }
    free(memory_silos);
    memory_silos = NULL;
    memory_silo_count = 0;
}

// Main function for testing
int main() {
    // Initialize IO Sockets for two APIs
    io_socket_t api_socket1, api_socket2;
    io_socket_init(&api_socket1, "127.0.0.1", 8081);  // API 1
    io_socket_init(&api_socket2, "127.0.0.1", 8082);  // API 2

    // Initialize Memory Silos for each API
    memory_silo_init(api_socket1.socket);
    memory_silo_init(api_socket2.socket);

    // Write data to API 1 Silo
    char data[] = "Hello, API 1 Memory Silo!";
    memory_silo_write(api_socket1.socket, data, sizeof(data));

    // Sync API 1 Silo with its API
    sync_memory_silo_with_api(&api_socket1, 1);

    // Write data to API 2 Silo
    char data2[] = "Hello, API 2 Memory Silo!";
    memory_silo_write(api_socket2.socket, data2, sizeof(data2));

    // Sync API 2 Silo with its API
    sync_memory_silo_with_api(&api_socket2, 2);

    // Cleanup
    cleanup_memory_silos();
    io_socket_close(&api_socket1);
    io_socket_close(&api_socket2);

    return 0;
}
