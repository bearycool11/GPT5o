#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "io_socket.h"
#include "unified_voice.h"
#include "knowledge.h"
#include "memory_silo.h"
#include "logic_loop.h" // Updated from pml_logic_loop.h to logic_loop.h

// Constants for buffer sizes and retry limits
#define BUFFER_SIZE 2048
#define RETRY_LIMIT 5

// Logging utility function
#define LOG_FILE "io_socket.log"

void log_message(const char* level, const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        time_t now = time(NULL);
        char* timestamp = ctime(&now);
        timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline
        fprintf(log_file, "[%s] [%s] %s\n", level, timestamp, message);
        fclose(log_file);
    } else {
        fprintf(stderr, "Failed to open log file: %s\n", LOG_FILE);
    }
}

// Function to initialize an IO socket
int io_socket_init(io_socket_t *io_socket, const char *ip, int port) {
    if (!io_socket || !ip) {
        log_message("ERROR", "Invalid parameters for IO socket initialization");
        return -1;
    }

    int retries = 0;
    while (retries < RETRY_LIMIT) {
        io_socket->socket = socket(AF_INET, SOCK_STREAM, 0);
        if (io_socket->socket < 0) {
            log_message("ERROR", "Failed to create IO socket");
            perror("Error creating IO socket");
            retries++;
            sleep(1);  // Retry delay
            continue;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
            log_message("ERROR", "Invalid server IP address");
            perror("Invalid server IP address");
            close(io_socket->socket);
            retries++;
            continue;
        }

        if (connect(io_socket->socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            log_message("ERROR", "Failed to connect to server");
            perror("Error connecting to server");
            close(io_socket->socket);
            retries++;
        } else {
            log_message("INFO", "Successfully connected to server");
            return 0;
        }
    }

    log_message("FATAL", "Failed to establish connection after retries");
    return -1;
}

// Function to send data through the socket
int io_socket_send(int socket, const void *data, size_t length) {
    if (!data || length == 0) {
        log_message("ERROR", "Invalid parameters for sending data");
        return -1;
    }

    ssize_t bytes_sent = send(socket, data, length, 0);
    if (bytes_sent < 0) {
        log_message("ERROR", "Failed to send data");
        perror("Error sending data");
        return -1;
    }

    char log_entry[256];
    snprintf(log_entry, sizeof(log_entry), "Sent %ld bytes of data", bytes_sent);
    log_message("INFO", log_entry);

    return bytes_sent;
}

// Function to receive data from the socket
int io_socket_receive(int socket, void *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        log_message("ERROR", "Invalid parameters for receiving data");
        return -1;
    }

    ssize_t bytes_received = recv(socket, buffer, buffer_size - 1, 0);
    if (bytes_received < 0) {
        log_message("ERROR", "Failed to receive data");
        perror("Error receiving data");
        return -1;
    }

    ((char*)buffer)[bytes_received] = '\0';  // Null-terminate the data
    char log_entry[256];
    snprintf(log_entry, sizeof(log_entry), "Received %ld bytes of data: %s", bytes_received, (char*)buffer);
    log_message("INFO", log_entry);

    return bytes_received;
}

// Function to close the IO socket
void io_socket_close(io_socket_t *io_socket) {
    if (io_socket && io_socket->socket >= 0) {
        close(io_socket->socket);
        io_socket->socket = -1;
        log_message("INFO", "IO socket closed successfully");
    }
}

// Function to handle cross-talk communication with APIs
void cross_talk_with_apis(io_socket_t *io_socket, const char *message) {
    if (!io_socket || !message) {
        log_message("ERROR", "Invalid parameters for cross-talk");
        return;
    }

    // Send request to API
    printf("Sending message to API: %s\n", message);
    if (io_socket_send(io_socket->socket, message, strlen(message)) < 0) {
        log_message("ERROR", "Failed to send message to API");
        return;
    }

    // Receive response from API
    char buffer[BUFFER_SIZE];
    if (io_socket_receive(io_socket->socket, buffer, sizeof(buffer)) < 0) {
        log_message("ERROR", "Failed to receive response from API");
        return;
    }

    // Update knowledge graph with API response
    Graph *knowledge_graph = create_graph(1024);  // Assume a knowledge graph is used
    Node *node = create_node(buffer, 0);
    add_node(knowledge_graph, node);
    printf("Updated knowledge graph with response: %s\n", buffer);
}

// Function to integrate with PML Logic Loop and Memory Silo
void integrate_pml_memory(io_socket_t *io_socket, memory_silo_t *memory_silo) {
    if (!io_socket || !memory_silo) {
        log_message("ERROR", "Invalid parameters for PML Memory integration");
        return;
    }

    printf("Integrating PML logic loop and memory silo...\n");

    // Example: Send siloed data to the PML logic loop
    char siloed_data[BUFFER_SIZE] = "Example siloed data";
    io_socket_send(io_socket->socket, siloed_data, strlen(siloed_data));

    // Simulate processing within PML logic loop
    pml_logic_loop_process(io_socket);
    printf("Integration completed.\n");
}

// Main function for testing
int main() {
    // Initialize IO socket
    io_socket_t io_socket;
    if (io_socket_init(&io_socket, "127.0.0.1", 8080) < 0) {
        fprintf(stderr, "Failed to initialize IO socket\n");
        return EXIT_FAILURE;
    }

    // Initialize memory silo
    memory_silo_t *memory_silo = memory_silo_init(1);
    if (!memory_silo) {
        fprintf(stderr, "Failed to initialize memory silo\n");
        io_socket_close(&io_socket);
        return EXIT_FAILURE;
    }

    // Perform cross-talk with APIs
    cross_talk_with_apis(&io_socket, "Hello from IO Socket");

    // Integrate with PML logic loop and memory silo
    integrate_pml_memory(&io_socket, memory_silo);

    // Cleanup
    memory_silo_free(memory_silo);
    io_socket_close(&io_socket);

    return EXIT_SUCCESS;
}
