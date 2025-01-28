#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "io_socket.h"
#include "memory_silo.h"
#include "knowledge.h"
#include "pml_logic_loop.h"
#include "utf8_tokenizer.h"

// Define a unified voice structure
typedef struct {
    io_socket_t io_socket;
    memory_silo_t *memory_silo;
    pml_logic_loop_t *pml_logic_loop;
    Graph *knowledge_graph;
} unified_voice_t;

// Function to initialize the unified voice system
unified_voice_t* init_unified_voice(const char *ip, int port, int silo_id) {
    unified_voice_t *uv = malloc(sizeof(unified_voice_t));
    if (!uv) {
        perror("Failed to allocate unified voice instance");
        exit(EXIT_FAILURE);
    }

    // Initialize IO socket
    if (io_socket_init(&uv->io_socket, ip, port) < 0) {
        fprintf(stderr, "Failed to initialize IO socket\n");
        free(uv);
        return NULL;
    }

    // Initialize memory silo
    uv->memory_silo = memory_silo_init(silo_id);
    if (!uv->memory_silo) {
        fprintf(stderr, "Failed to initialize memory silo\n");
        io_socket_cleanup(&uv->io_socket);
        free(uv);
        return NULL;
    }

    // Initialize PML logic loop
    uv->pml_logic_loop = pml_logic_loop_init(silo_id, uv->io_socket.socket);
    if (!uv->pml_logic_loop) {
        fprintf(stderr, "Failed to initialize PML logic loop\n");
        memory_silo_free(uv->memory_silo);
        io_socket_cleanup(&uv->io_socket);
        free(uv);
        return NULL;
    }

    // Initialize knowledge graph
    uv->knowledge_graph = create_graph(1024);
    if (!uv->knowledge_graph) {
        fprintf(stderr, "Failed to initialize knowledge graph\n");
        pml_logic_loop_cleanup(uv->pml_logic_loop);
        memory_silo_free(uv->memory_silo);
        io_socket_cleanup(&uv->io_socket);
        free(uv);
        return NULL;
    }

    return uv;
}

// Function to process UTF-8 input and update knowledge graph
void process_utf8_and_update_knowledge(unified_voice_t *uv, const char *input) {
    if (!uv || !input) {
        fprintf(stderr, "Invalid parameters for process_utf8_and_update_knowledge\n");
        return;
    }

    printf("Processing input: \"%s\"\n", input);

    const char *delimiters = " \t\n";
    const char *start = input;
    const char *current = input;

    while (*current) {
        if (strchr(delimiters, *current) != NULL) {
            if (current > start) {
                // Extract token
                size_t token_len = current - start;
                char *token = strndup(start, token_len);

                // Add token to the knowledge graph
                Node *node = create_node(token, 0);
                add_node(uv->knowledge_graph, node);
                printf("Added token to knowledge graph: %s\n", token);

                free(token);
            }
            current++;
            start = current;
        } else {
            if (is_utf8_start_byte((unsigned char)*current)) {
                current++;
                while (*current && (*current & 0xC0) == 0x80) {
                    current++;
                }
            } else {
                current++;
            }
        }
    }

    if (current > start) {
        // Add last token
        size_t token_len = current - start;
        char *token = strndup(start, token_len);

        Node *node = create_node(token, 0);
        add_node(uv->knowledge_graph, node);
        printf("Added token to knowledge graph: %s\n", token);

        free(token);
    }
}

// Function to handle cross-talk communication
void cross_talk(unified_voice_t *uv, const char *message) {
    if (!uv || !message) {
        fprintf(stderr, "Invalid parameters for cross-talk\n");
        return;
    }

    printf("Sending message to cross-talk socket: %s\n", message);
    io_socket_write(uv->io_socket.socket, message, strlen(message));

    char buffer[1024];
    io_socket_read(uv->io_socket.socket, buffer, sizeof(buffer));
    printf("Received response: %s\n", buffer);

    // Update the knowledge graph with the response
    process_utf8_and_update_knowledge(uv, buffer);
}

// Function to run the unified voice system
void run_unified_voice(unified_voice_t *uv) {
    if (!uv) {
        fprintf(stderr, "Unified voice instance is NULL\n");
        return;
    }

    const char *test_input = "This is a test input for cross-talk";
    process_utf8_and_update_knowledge(uv, test_input);

    cross_talk(uv, "Hello from Unified Voice");

    // Simulate PML logic loop execution
    printf("Executing PML logic loop...\n");
    pml_logic_loop_process(&uv->io_socket);

    printf("Unified voice processing completed.\n");
}

// Cleanup function for the unified voice system
void cleanup_unified_voice(unified_voice_t *uv) {
    if (!uv) return;

    pml_logic_loop_cleanup(uv->pml_logic_loop);
    memory_silo_free(uv->memory_silo);
    destroy_graph(uv->knowledge_graph);
    io_socket_cleanup(&uv->io_socket);
    free(uv);
}

// Main function
int main() {
    unified_voice_t *uv = init_unified_voice("127.0.0.1", 8080, 1);
    if (!uv) {
        fprintf(stderr, "Failed to initialize unified voice system\n");
        return EXIT_FAILURE;
    }

    run_unified_voice(uv);
    cleanup_unified_voice(uv);

    return EXIT_SUCCESS;
}
