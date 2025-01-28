#include "watcher.h"

// Global whitelist and blacklist
char *whitelist[] = {"62.0.0.1", "62.0.0.2"};
int whitelist_count = 2;

char *blacklist[MAX_BLACKLIST];
int blacklist_count = 0;

// Initialize Watcher
void watcher_init() {
    printf("[Watcher] Initializing...\n");
    system("sudo apt-get update");
    system("sudo apt-get install fail2ban -y");
    system("sudo apt-get install ufw -y");
    printf("[Watcher] Security tools installed and configured.\n");
}

// Monitor Logs
void monitor_logs() {
    FILE *log_file = fopen("/var/log/auth.log", "r");
    if (!log_file) {
        perror("[Watcher] Failed to open log file");
        return;
    }
    fseek(log_file, 0, SEEK_END); // Start at the end
    char buffer[1024];
    while (1) {
        if (fgets(buffer, sizeof(buffer), log_file)) {
            if (strstr(buffer, "Failed password")) {
                char ip[16];
                sscanf(buffer, "%*s %*s %*s %*s %s", ip); // Extract IP
                handle_failed_attempt(ip);
            }
        }
        sleep(1);
    }
    fclose(log_file);
}

// Handle Failed Login Attempt
void handle_failed_attempt(const char *ip) {
    if (is_whitelisted(ip)) return;

    printf("[Watcher] State your case on why you accessed the back door. Your activity has been logged.\n");
    snoop_data_streams(ip);
    record_user_input(ip);
    add_to_blacklist(ip);
    printf("[Watcher] Warning! IP %s flagged.\n", ip);
}

// Snoop Data Streams
void snoop_data_streams(const char *ip) {
    printf("[Watcher] Snoop data streams for IP: %s\n", ip);
    // Add snooping logic here
}

// Record User Input
void record_user_input(const char *ip) {
    printf("[Watcher] Record user input for IP: %s\n", ip);
    // Add user input recording logic here
}

// Check if IP is Whitelisted
int is_whitelisted(const char *ip) {
    for (int i = 0; i < whitelist_count; i++) {
        if (strcmp(ip, whitelist[i]) == 0) return 1;
    }
    return 0;
}

// Add IP to Blacklist
void add_to_blacklist(const char *ip) {
    if (blacklist_count < MAX_BLACKLIST) {
        blacklist[blacklist_count++] = strdup(ip);
    }
    printf("[Watcher] IP %s added to blacklist.\n", ip);
}
