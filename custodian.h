#ifndef CUSTODIAN_H
#define CUSTODIAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io_socket.h"
#include "memory_silo.h"
#include "pml_logic_loop.h"

// Constants and Configuration
#define BITCOIN_WALLET "bc1qetkudft7hlsl3k7nhrg6zrkufpu6q3rdnx5ag5" // Example Bitcoin wallet address
#define BUFFER_SIZE 1024
#define LOG_FILE "custodian.log"

// Custodian Functions

/**
 * Logs a message to the custodian log file.
 * @param level The severity level (e.g., "INFO", "ERROR").
 * @param message The message to log.
 */
void log_message(const char* level, const char* message);

/**
 * Interacts with the LLaMA API by sending an input prompt and receiving a response.
 * @param input_prompt The input prompt to send to the LLaMA API.
 * @param llama_response The buffer to store the LLaMA API's response.
 */
void interact_with_llama_api(const char* input_prompt, char* llama_response);

/**
 * Interacts with the custom API by sending input data and receiving a response.
 * @param input_data The input data to send to the custom API.
 * @param custom_response The buffer to store the custom API's response.
 */
void interact_with_custom_api(const char* input_data, char* custom_response);

/**
 * Processes Bitcoin payments by sending the specified amount to the recipient wallet.
 * @param recipient The recipient's Bitcoin wallet address.
 * @param amount The amount of Bitcoin to send.
 */
void process_bitcoin_payment(const char* recipient, double amount);

/**
 * Synchronizes data with a memory silo over an IO socket.
 * @param io_socket Pointer to the IO socket instance.
 * @param silo Pointer to the memory silo instance.
 */
void sync_with_memory_silo(io_socket_t* io_socket, memory_silo_t* silo);

/**
 * Executes the main custodian process, including API interactions,
 * memory silo synchronization, and Bitcoin transactions.
 */
void run_custodian();

#endif // CUSTODIAN_H
