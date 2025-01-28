#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "logic_loop.h" // Updated include to match header
#include "cluster_manager.h"
#include "memory_silo.h"
#include "io_socket.h"
#include "cross_talk.h"

#define MAX_ITERATIONS 100
#define BUFFER_SIZE 1024
#define API_ENDPOINT_CHATGPT "http://127.0.0.1:8080/api/chatgpt"
#define API_ENDPOINT_LLAMA "http://127.0.0.1:8081/api/llama"

// Function to initialize the logic loop
int initialize_logic_loop(PMLL* pml) {
    if (!pml) {
        fprintf(stderr, "Error: PMLL instance is null\n");
        return -1;
    }

    printf("Initializing logic loop...\n");

    // Load metrics from persistent storage
    load_pml_metrics(pml);

    printf("Logic loop initialized successfully.\n");
    return 0;
}

// Main execution of the logic loop
void execute_logic_loop(PMLL* pml) {
    if (!pml) {
        fprintf(stderr, "Error: PMLL instance is null\n");
        return;
    }

    printf("Starting logic loop...\n");

    int iterations = 0;
    while (iterations < MAX_ITERATIONS) {
        printf("\nIteration %d:\n", iterations + 1);

        // Measure start time for performance metrics
        clock_t start_time = clock();

        // Step 1: Monitor system performance
        pml->monitor(pml);

        // Step 2: Consolidate memory across silos
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Pipe creation failed");
            continue; // Skip to next iteration
        }
        consolidate_memory(pml, pipefd);
        close(pipefd[0]);
        close(pipefd[1]);

        // Step 3: Evaluate the ML model's performance
        pml->evaluate(pml);

        // Step 4: Automate and prioritize next steps
        pml->automate(pml);

        // Step 5: Update consolidated memory graph
        char buffer[BUFFER_SIZE] = "Memory graph update data";
        update_consolidated_memory_graph(pml, buffer);

        // Step 6: Cross-talk with APIs
        char chatgpt_response[BUFFER_SIZE];
        char llama_response[BUFFER_SIZE];

        printf("Sending API request to ChatGPT...\n");
        if (send_api_request(API_ENDPOINT_CHATGPT, "Hello, ChatGPT!", chatgpt_response, sizeof(chatgpt_response)) == 0) {
            printf("Received response from ChatGPT: %s\n", chatgpt_response);

            printf("Forwarding ChatGPT response to LLaMA...\n");
            if (send_api_request(API_ENDPOINT_LLAMA, chatgpt_response, llama_response, sizeof(llama_response)) == 0) {
                printf("Received response from LLaMA: %s\n", llama_response);
            } else {
                fprintf(stderr, "Error: Failed to send request to LLaMA.\n");
            }
        } else {
            fprintf(stderr, "Error: Failed to send request to ChatGPT.\n");
        }

        // Step 7: Save metrics for persistence
        save_pml_metrics(pml);

        // Measure end time for performance metrics
        clock_t end_time = clock();
        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        printf("Iteration %d completed in %.2f seconds.\n", iterations + 1, elapsed_time);

        // Simulate work
        usleep(100000); // Sleep for 100ms

        iterations++;
    }

    printf("Logic loop completed after %d iterations.\n", iterations);
}

// Cleanup function for the logic loop
void cleanup_logic_loop(PMLL* pml) {
    if (!pml) {
        fprintf(stderr, "Error: PMLL instance is null\n");
        return;
    }

    printf("Cleaning up logic loop resources...\n");

    // Save final metrics to persistent storage
    save_pml_metrics(pml);

    printf("Logic loop cleanup complete.\n");
}

// Helper function to consolidate memory across silos
void consolidate_memory(PMLL* pml, int pipefd[2]) {
    if (!pml) {
        fprintf(stderr, "Error: PMLL instance is null\n");
        return;
    }

    printf("Consolidating memory across silos...\n");

    // Simulated memory consolidation process
    const char* data = "Consolidated memory data";
    if (write(pipefd[1], data, strlen(data)) == -1) {
        perror("Failed to write to pipe");
        return;
    }
    printf("Memory consolidation completed.\n");
}

// Helper function to update consolidated memory graph
void update_consolidated_memory_graph(PMLL* pml, const char* buffer) {
    if (!pml || !buffer) {
        fprintf(stderr, "Error: Invalid parameters for updating memory graph\n");
        return;
    }

    printf("Updating memory graph with: %s\n", buffer);

    // Simulate graph update process
    printf("Memory graph updated successfully.\n");
}
