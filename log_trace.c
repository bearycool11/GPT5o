#include "io_socket.h"
#include "logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <netinet/ip_icmp.h>

#define BUFFER_SIZE 1024
#define IP_BLOCK_LIST "blocked_ips.txt"

// Function to send a ping to the blocked IP
void ping_ip(const char* ip) {
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "ping -c 1 %s", ip);  // Sends one ping request
    system(command);  // Executes the ping command in the system shell
    log_message("INFO", "Pinged blocked IP address.");

    // Send the echo message after pinging
    printf("Careful, you naughty boy you: %s\n", ip);
    log_message("INFO", "Sent echo message to blocked IP.");
}

// Function to track failed login attempts for a given IP
int track_failed_attempts(const char* ip) {
    char line[BUFFER_SIZE];
    FILE* block_list = fopen(IP_BLOCK_LIST, "r");
    if (block_list) {
        while (fgets(line, sizeof(line), block_list)) {
            if (strstr(line, ip)) {
                fclose(block_list);
                return 1;  // IP is blocked
            }
        }
        fclose(block_list);
    }
    return 0;  // IP not blocked yet
}

// Log suspicious IP activity
void log_suspicious_activity(const char* ip) {
    log_message("WARNING", "Suspicious IP detected: ");
    log_message("WARNING", ip);
}

// Block an IP by adding it to the block list and pinging it
void block_ip(const char* ip) {
    FILE* block_list = fopen(IP_BLOCK_LIST, "a");
    if (block_list) {
        fprintf(block_list, "%s\n", ip);
        fclose(block_list);
    }

    // Log the blocked IP and ping it
    log_suspicious_activity(ip);
    ping_ip(ip);  // Send a ping and echo message
}

// Main function to check logs and perform system checks
void check_logs_and_perform_checks() {
    log_message("INFO", "Starting log check process.");

    // Step 1: Read log file and print to console
    printf("Reading log file: %s\n", LOG_FILE);
    read_log_file(LOG_FILE);

    // Step 2: Check for issues in logs via the system API
    log_message("INFO", "Checking system logs via custom API.");
    check_system_logs_for_issues();

    log_message("INFO", "Log check process completed.");
}

// Main function
int main() {
    // For now, we just trigger the log check and block the suspicious IP (if any)
    check_logs_and_perform_checks();
    return 0;
}
