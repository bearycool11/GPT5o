#include "cross_talk.h"
#include "io_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For timestamps in logging

#define CHATGPT_SERVER_IP "127.0.0.1"
#define CHATGPT_SERVER_PORT 8080
#define LLAMA_SERVER_IP "127.0.0.1"
#define LLAMA_SERVER_PORT 9090
#define BUFFER_SIZE 1024
#define LOG_FILE "cross_talk.log"

// Logging utility
void log_message(const char* level, const char* message) {
    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "[%s] [%s] %s\n", level, ctime(&now), message);
        fclose(log_file);
    }
}

// Serialize data into JSON format
void serialize_data(void* raw_data, char* output_buffer, size_t buffer_size) {
    snprintf(output_buffer, buffer_size, "{\"prompt\": \"%s\", \"max_tokens\": 50}", (char*)raw_data);
    log_message("INFO", "Serialized data for ChatGPT.");
}

// Deserialize data from JSON format
void deserialize_data(const char* input_buffer, void* output_structure) {
    printf("Deserialized data: %s\n", input_buffer);
    char log_entry[256];
    snprintf(log_entry, sizeof(log_entry), "Deserialized data: %s", input_buffer);
    log_message("INFO", log_entry);
}

// Main cross-talk logic
void process_cross_talk() {
    log_message("INFO", "Starting cross-talk process.");
    
    // Step 1: Create sockets for ChatGPT and LLaMA
    int chatgpt_socket = create_io_socket();
    if (chatgpt_socket < 0) {
        log_message("FATAL", "Failed to establish connection to ChatGPT.");
        return;
    }

    int llama_socket = create_io_socket();
    if (llama_socket < 0) {
        log_message("FATAL", "Failed to establish connection to LLaMA.");
        io_socket_cleanup(chatgpt_socket);
        return;
    }

    char serialized_data[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    // Step 2: Send input data to ChatGPT
    char* input_data = "Hello, ChatGPT!";
    serialize_data(input_data, serialized_data, sizeof(serialized_data));
    log_message("INFO", "Sending serialized data to ChatGPT.");
    io_socket_write(chatgpt_socket, serialized_data, strlen(serialized_data));

    // Step 3: Receive response from ChatGPT
    log_message("INFO", "Waiting for response from ChatGPT.");
    io_socket_read(chatgpt_socket, buffer, sizeof(buffer));
    log_message("INFO", "Received response from ChatGPT.");

    // Step 4: Forward ChatGPT response to LLaMA
    log_message("INFO", "Forwarding ChatGPT response to LLaMA.");
    io_socket_write(llama_socket, buffer, strlen(buffer));

    // Step 5: Receive final response from LLaMA
    log_message("INFO", "Waiting for final response from LLaMA.");
    io_socket_read(llama_socket, buffer, sizeof(buffer));
    printf("Final response from LLaMA: %s\n", buffer);
    log_message("INFO", "Received final response from LLaMA.");

    // Step 6: Clean up sockets
    log_message("INFO", "Cleaning up sockets.");
    io_socket_cleanup(chatgpt_socket);
    io_socket_cleanup(llama_socket);

    log_message("INFO", "Cross-talk process completed.");
}
