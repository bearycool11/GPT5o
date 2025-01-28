#ifndef IO_SOCKET_H
#define IO_SOCKET_H

#include <stddef.h> // For size_t

// Constants
#define SERVER_IP "127.0.0.1"       // Default server IP
#define SERVER_PORT 8080            // Default server port
#define BUFFER_SIZE 1024            // Default buffer size
#define RETRY_LIMIT 5               // Maximum number of connection retries

// Function Declarations

/**
 * Logs a message to the log file with a given level and message.
 * @param level The severity level (e.g., "INFO", "ERROR").
 * @param message The message to log.
 */
void log_message(const char* level, const char* message);

/**
 * Creates an IO socket and connects to the server.
 * Retries with exponential backoff if the connection fails.
 * @return The socket file descriptor on success, or -1 on failure.
 */
int create_io_socket();

/**
 * Sends data over the IO socket.
 * @param io_socket The socket file descriptor.
 * @param buffer Pointer to the data to send.
 * @param length The length of the data to send.
 */
void io_socket_write(int io_socket, const void* buffer, size_t length);

/**
 * Receives data from the IO socket.
 * @param io_socket The socket file descriptor.
 * @param buffer Pointer to the buffer where data will be stored.
 * @param buffer_size The size of the buffer.
 */
void io_socket_read(int io_socket, void* buffer, size_t buffer_size);

/**
 * Cleans up and closes the IO socket.
 * @param io_socket The socket file descriptor to close.
 */
void io_socket_cleanup(int io_socket);

#endif // IO_SOCKET_H
