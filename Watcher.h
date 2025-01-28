#ifndef WATCHER_H
#define WATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <curl/curl.h>

// Constants
#define MAX_BLACKLIST 1024

// Global whitelist and blacklist
extern char *whitelist[];
extern int whitelist_count;
extern char *blacklist[];
extern int blacklist_count;

// Function declarations

// Initialization
void watcher_init();

// Log Monitoring
void monitor_logs();
void monitor_logs_dynamic();

// Dynamic Response Handling
void handle_failed_attempt(const char *ip);
void handle_dynamic_response(const char *ip, const char *log_entry);

// API Communication
void send_api_request(const char *url, const char *api_token, const char *payload);

// Data Snooping and Input Recording
void snoop_data_streams(const char *ip);
void record_user_input(const char *ip);

// Logging
void log_activity(const char *type, const char *message);

// Whitelist and Blacklist Management
int is_whitelisted(const char *ip);
int should_blacklist(const char *ip);
int recent_failed_attempts(const char *ip);
void add_to_blacklist(const char *ip);
void save_blacklist();

// Ethical Review
void ethical_review(const char *ip);

#endif // WATCHER_H
