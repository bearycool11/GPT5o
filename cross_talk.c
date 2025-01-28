#include "cross_talk.h"
#include "io_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h> // For timestamps in logging
#include "unified_voice.h"

#define LLAMA_API_URL "https://api.llama.ai/v1/chat" // LLaMA API endpoint
#define LLAMA_API_KEY "LA-8c4003a74c5040b2b735866f22e754ed55c2ab712b0346b3bca0f1993362704a" // Replace with your LLaMA API Key
#define CUSTOM_API_URL "http://127.0.0.1:8080/api/v1" // Custom API endpoint
#define BUFFER_SIZE 1024
#define LOG_FILE "cross_talk.log"

// Ensure safe concatenation with strncat by checking space availability
#define SAFE_STRNCAT(dest, src, size) do { \
    if (strlen(dest) + strlen(src) + 1 <= size) { \
        strncat(dest, src, size - strlen(dest) - 1); \
    } \
} while(0)

// Logging utility
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

// CURL Write callback for capturing API responses
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    SAFE_STRNCAT((char*)userp, (char*)contents, BUFFER_SIZE);
    return realsize;
}

// Function to interact with the LLaMA API
void interact_with_llama_api(const char* input_prompt, char* llama_response) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        log_message("ERROR", "Failed to initialize cURL for LLaMA API.");
        return;
    }

    char post_fields[BUFFER_SIZE];
    snprintf(post_fields, sizeof(post_fields), "{\"prompt\": \"%s\", \"max_tokens\": 50}", input_prompt);

    char response_buffer[BUFFER_SIZE] = {0};

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer " LLAMA_API_KEY);

    curl_easy_setopt(curl, CURLOPT_URL, LLAMA_API_URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log_message("ERROR", curl_easy_strerror(res));
    } else {
        snprintf(llama_response, BUFFER_SIZE, "%s", response_buffer);
        log_message("INFO", "Successfully interacted with LLaMA API.");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

// Function to interact with the custom API
void interact_with_custom_api(const char* input_data, char* custom_response) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        log_message("ERROR", "Failed to initialize cURL for custom API.");
        return;
    }

    char post_fields[BUFFER_SIZE];
    snprintf(post_fields, sizeof(post_fields), "{\"data\": \"%s\"}", input_data);

    char response_buffer[BUFFER_SIZE] = {0};

    curl_easy_setopt(curl, CURLOPT_URL, CUSTOM_API_URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        log_message("ERROR", curl_easy_strerror(res));
    } else {
        snprintf(custom_response, BUFFER_SIZE, "%s", response_buffer);
        log_message("INFO", "Successfully interacted with custom API.");
    }

    curl_easy_cleanup(curl);
}

// Main cross-talk logic
void process_cross_talk() {
    log_message("INFO", "Starting cross-talk process.");

    char llama_response[BUFFER_SIZE] = {0};
    char custom_response[BUFFER_SIZE] = {0};

    // Step 1: Interact with LLaMA API
    const char* llama_input = "Hello, LLaMA!";
    log_message("INFO", "Sending input to LLaMA API.");
    interact_with_llama_api(llama_input, llama_response);
    log_message("INFO", "Received response from LLaMA API.");

    // Step 2: Forward response to custom API
    log_message("INFO", "Sending LLaMA API response to custom API.");
    interact_with_custom_api(llama_response, custom_response);
    log_message("INFO", "Received response from custom API.");

    // Step 3: Process and log results
    printf("Final response from Custom API:\n%s\n", custom_response);
    log_message("INFO", "Cross-talk process completed.");
}

// Main function to enable compilation
int main() {
    process_cross_talk();
    return 0;
}
