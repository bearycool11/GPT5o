#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "memory_silo.h"
#include "io_socket.h"

// Unified Voice Structure
typedef struct {
    int id;
    int memory_silo_id;
    int io_socket_id;
} unified_voice_t;

// Unified Voice Functions
unified_voice_t* unified_voice_init(int memory_silo_id);
void unified_voice_read(int io_socket_id, void* buffer, int length);
void unified_voice_write(int io_socket_id, void* buffer, int length);
unified_voice_t* get_unified_voice(int io_socket_id);

// IO Socket Functions
void io_socket_init(int* io_socket_id);
void io_socket_read(int io_socket_id, void* buffer, int length);
void io_socket_write(int io_socket_id, void* buffer, int length);

// Diagnosis Window Functions
void diagnosis_window_init(int io_socket_id);
void diagnosis_window_update(int io_socket_id, void* buffer, int length);
void diagnosis_window_close(int io_socket_id);

// Unified Voice Initialization
unified_voice_t* unified_voice_init(int memory_silo_id) {
    unified_voice_t* unified_voice = malloc(sizeof(unified_voice_t));
    if (!unified_voice) {
        perror("Failed to allocate memory for unified voice");
        exit(EXIT_FAILURE);
    }
    
    unified_voice->id = 1;
    unified_voice->memory_silo_id = memory_silo_id;

    // Initialize the IO socket
    io_socket_init(&unified_voice->io_socket_id);

    return unified_voice;
}

void unified_voice_read(int io_socket_id, void* buffer, int length) {
    unified_voice_t* unified_voice = get_unified_voice(io_socket_id);
    memory_silo_read(unified_voice->memory_silo_id, buffer, length);
}

void unified_voice_write(int io_socket_id, void* buffer, int length) {
    unified_voice_t* unified_voice = get_unified_voice(io_socket_id);
    memory_silo_write(unified_voice->memory_silo_id, buffer, length);
}

// Get Unified Voice Function
unified_voice_t* get_unified_voice(int io_socket_id) {
    unified_voice_t* unified_voice = malloc(sizeof(unified_voice_t));
    if (!unified_voice) {
        perror("Failed to allocate memory for unified voice");
        exit(EXIT_FAILURE);
    }
    unified_voice->id = 1;
    unified_voice->memory_silo_id = 1;
    unified_voice->io_socket_id = io_socket_id;
    return unified_voice;
}

// IO Socket Initialization
void io_socket_init(int* io_socket_id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    *io_socket_id = sock;

    // Set up the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // Bind the socket to the server address
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(sock, 1) == -1) {
        perror("Error listening on socket");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

void io_socket_read(int io_socket_id, void* buffer, int length) {
    if (read(io_socket_id, buffer, length) == -1) {
        perror("Error reading from IO socket");
    }
}

void io_socket_write(int io_socket_id, void* buffer, int length) {
    if (write(io_socket_id, buffer, length) == -1) {
        perror("Error writing to IO socket");
    }
}

// Diagnosis Window Functions
void diagnosis_window_init(int io_socket_id) {
    // Initialize the diagnosis window
    // (Implementation detail)
}

void diagnosis_window_update(int io_socket_id, void* buffer, int length) {
    // Update the diagnosis window
    io_socket_read(io_socket_id, buffer, length);
    // (Implementation detail to update the window)
}

void diagnosis_window_close(int io_socket_id) {
    close(io_socket_id);
}

int main() {
    // Initialize the unified voice
    unified_voice_t* unified_voice = unified_voice_init(1);

    // Initialize the diagnosis window
    diagnosis_window_init(unified_voice->io_socket_id);

    // Main loop
    while (1) {
        void* buffer = malloc(1024);
        if (!buffer) {
            perror("Failed to allocate buffer");
            break;
        }

        io_socket_read(unified_voice->io_socket_id, buffer, 1024);
        diagnosis_window_update(unified_voice->io_socket_id, buffer, 1024);
        io_socket_write(unified_voice->io_socket_id, buffer, 1024);

        free(buffer);
    }

    // Close the diagnosis window
    diagnosis_window_close(unified_voice->io_socket_id);
    free(unified_voice);

    return 0;
}
