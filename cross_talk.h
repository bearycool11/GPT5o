#ifndef CROSS_TALK_H
#define CROSS_TALK_H

#include <stddef.h> // For size_t definitions

// Constants
#define LLAMA_API_URL "https://api.llama.ai/v1/chat"    // LLaMA API endpoint
#define LLAMA_API_KEY "LA-8c4003a74c5040b2b735866f22e754ed55c2ab712b0346b3bca0f1993362704a" // Replace with your LLaMA API key
#define CUSTOM_API_URL "http://127.0.0.1:8080/api/v1"  // Custom API endpoint
#define BUFFER_SIZE 1024                               // Buffer size for API responses
#define LOG_FILE "cross_talk.log"                      // Log file for cross-talk events

// Macros for safety
#define SAFE_STRNCAT(dest, src, size) do { \
    if (strlen(dest) + strlen(src) + 1 <= size) { \
        strncat(dest, src, size - strlen(dest) - 1); \
    } \
} while(0)

// Function Declarations

/**
 * Logs a message to the specified log file.
 * @param level The severity level (e.g., "INFO", "ERROR").
 * @param message The message to log.
 */
void log_message(const char* level, const char* message);

/**
 * Serialize data into JSON format.
 * @param raw_data The raw input data to serialize.
 * @param output_buffer The buffer where the serialized JSON string will be stored.
 * @param buffer_size The size of the output buffer.
 */
void serialize_data(const void* raw_data, char* output_buffer, size_t buffer_size);

/**
 * Deserialize data from JSON format.
 * @param input_buffer The JSON string to deserialize.
 * @param output_structure The structure where the deserialized data will be stored.
 */
void deserialize_data(const char* input_buffer, void* output_structure);

/**
 * Interacts with the LLaMA API by sending an input prompt and receiving a response.
 * @param input_prompt The input prompt to send to the LLaMA API.
 * @param llama_response The buffer where the LLaMA API's response will be stored.
 */
void interact_with_llama_api(const char* input_prompt, char* llama_response);

/**
 * Interacts with the custom API by sending input data and receiving a response.
 * @param input_data The input data to send to the custom API.
 * @param custom_response The buffer where the custom API's response will be stored.
 */
void interact_with_custom_api(const char* input_data, char* custom_response);

/**
 * Main cross-talk logic that facilitates communication between the LLaMA API
 * and the custom API.
 */
void process_cross_talk();

#endif // CROSS_TALK_H
