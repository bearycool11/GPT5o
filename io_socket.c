#include "io_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h> // For monitoring timestamps

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024
#define RETRY_LIMIT 5
#define LOG_FILE "io_socket.log"

// Logging utility
void log_message(const char* level, const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "[%s] [%s] %s\n", level, ctime(&now), message);
        fclose(log_file);
    }
}

// Retry mechanism for socket creation
int create_io_socket() {
    int io_socket = -1;
    int retries = 0;

    while (io_socket < 0 && retries < RETRY_LIMIT) {
        io_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (io_socket < 0) {
            log_message("ERROR", "Failed to create IO socket");
            perror("Error creating IO socket");
            retries++;
            sleep(1 << retries); // Exponential backoff
            continue;
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(SERVER_PORT);

        if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
            log_message("ERROR", "Invalid server IP address");
            perror("Invalid server IP address");
            close(io_socket);
            io_socket = -1;
            retries++;
            continue;
        }

        if (connect(io_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            log_message("ERROR", "Failed to connect to IO socket");
            perror("Error connecting to IO socket");
            close(io_socket);
            io_socket = -1;
            retries++;
        }
    }

    if (io_socket < 0) {
        log_message("FATAL", "Failed to establish connection after retries");
        fprintf(stderr, "Failed to connect after %d retries\n", RETRY_LIMIT);
        return -1;
    }

    log_message("INFO", "Successfully connected to IO socket");
    printf("Connected to IO socket at %s:%d\n", SERVER_IP, SERVER_PORT);
    return io_socket;
}

// Send data over the IO socket
void io_socket_write(int io_socket, void* buffer, size_t length) {
    if (io_socket < 0 || buffer == NULL || length == 0) {
        log_message("ERROR", "Invalid parameters for sending data");
        fprintf(stderr, "Invalid parameters for sending data\n");
        return;
    }

    ssize_t bytes_sent = send(io_socket, buffer, length, 0);
    if (bytes_sent < 0) {
        log_message("ERROR", "Failed to send data over IO socket");
        perror("Error sending data over IO socket");
    } else {
        char log_entry[256];
        snprintf(log_entry, sizeof(log_entry), "Sent %ld bytes of data", bytes_sent);
        log_message("INFO", log_entry);
    }
}

// Receive data from the IO socket
void io_socket_read(int io_socket, void* buffer, size_t buffer_size) {
    if (io_socket < 0 || buffer == NULL || buffer_size == 0) {
        log_message("ERROR", "Invalid parameters for receiving data");
        fprintf(stderr, "Invalid parameters for receiving data\n");
        return;
    }

    ssize_t bytes_received = recv(io_socket, buffer, buffer_size - 1, 0);
    if (bytes_received < 0) {
        log_message("ERROR", "Failed to receive data over IO socket");
        perror("Error receiving data over IO socket");
    } else {
        ((char*)buffer)[bytes_received] = '\0'; // Null-terminate the data
        char log_entry[256];
        snprintf(log_entry, sizeof(log_entry), "Received %ld bytes of data: %s", bytes_received, (char*)buffer);
        log_message("INFO", log_entry);
    }
}

// Cleanup resources
void io_socket_cleanup(int io_socket) {
    if (io_socket >= 0) {
        if (close(io_socket) < 0) {
            log_message("ERROR", "Error closing IO socket");
            perror("Error closing IO socket");
        } else {
            log_message("INFO", "IO socket closed successfully");
            printf("IO socket closed successfully\n");
        }
    }
}
