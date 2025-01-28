#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <curl/curl.h>

#define MAX_BLACKLIST 100

// Global whitelist and blacklist
char *whitelist[] = {"62.0.0.1", "62.0.0.2"};
int whitelist_count = 2;
char *blacklist[MAX_BLACKLIST];
int blacklist_count = 0;

// Function prototypes
void watcher_init();
void monitor_logs_dynamic();
void handle_dynamic_response(const char *ip, const char *log_entry);
void send_api_request(const char *url, const char *api_token, const char *payload);
void snoop_data_streams(const char *ip);
void log_activity(const char *type, const char *message);
int is_whitelisted(const char *ip);
int should_blacklist(const char *ip);
int recent_failed_attempts(const char *ip);
void add_to_blacklist(const char *ip);
void save_blacklist();
void ethical_review(const char *ip);

void watcher_init() {
    printf("[Watcher] Initializing...\n");
    system("sudo apt-get update");
    system("sudo apt-get install fail2ban -y");
    system("sudo apt-get install ufw -y");
    printf("[Watcher] Security tools installed and configured.\n");
    log_activity("SYSTEM", "Watcher initialized with fail2ban and UFW.");
}

void monitor_logs_dynamic() {
    FILE *log_file = fopen("/var/log/auth.log", "r");
    if (!log_file) {
        perror("[Watcher] Failed to open log file");
        log_activity("ERROR", "Failed to open auth log file.");
        return;
    }
    fseek(log_file, 0, SEEK_END);  // Start at the end
    char buffer[1024];
    while (1) {
        if (fgets(buffer, sizeof(buffer), log_file)) {
            if (strstr(buffer, "Failed password")) {
                char ip[16];
                sscanf(buffer, "%*s %*s %*s %*s %s", ip);  // Extract IP
                handle_dynamic_response(ip, buffer);
            }
        }
        sleep(1);
    }
    fclose(log_file);
}

void handle_dynamic_response(const char *ip, const char *log_entry) {
    if (is_whitelisted(ip)) return;

    // Log activity
    log_activity("THREAT", log_entry);

    // Prepare alert payload
    char alert_message[512];
    snprintf(alert_message, sizeof(alert_message), "Potential threat detected from IP: %s\nLog Entry: %s", ip, log_entry);

    // Example API endpoints
    const char *telegram_url = "https://api.telegram.org/botYOUR_TELEGRAM_BOT_TOKEN/sendMessage?chat_id=YOUR_CHAT_ID";
    const char *custom_api_url = "https://example.com/api/report";

    // Send alerts
    char payload[1024];
    snprintf(payload, sizeof(payload), "{\"ip\": \"%s\", \"log_entry\": \"%s\"}", ip, log_entry);
    send_api_request(telegram_url, NULL, alert_message);
    send_api_request(custom_api_url, "YOUR_CUSTOM_API_TOKEN", payload);

    // Dynamic analysis and actions
    snoop_data_streams(ip);
    if (should_blacklist(ip)) {
        add_to_blacklist(ip);
    } else {
        printf("[Watcher] Warning issued for IP: %s\n", ip);
    }
}

void send_api_request(const char *url, const char *api_token, const char *payload) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (curl) {
        if (api_token) {
            char auth_header[256];
            snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", api_token);
            headers = curl_slist_append(headers, auth_header);
        }
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "[Watcher] API request failed: %s\n", curl_easy_strerror(res));
            log_activity("ERROR", "API request failed.");
        } else {
            log_activity("INFO", "API request successful.");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void snoop_data_streams(const char *ip) {
    printf("[Watcher] Snoop data streams for IP: %s\n", ip);
    char command[256];
    snprintf(command, sizeof(command), "sudo tcpdump -i eth0 host %s -w capture.pcap", ip);
    system(command);  // Capture traffic from the specific IP
    log_activity("SNOOP", "Data streams snooped and logged.");
}

void log_activity(const char *type, const char *message) {
    FILE *log_file = fopen("/var/log/watcher_activity.log", "a");
    if (log_file) {
        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(log_file, "[%s] [%s] %s\n", timestamp, type, message);
        fclose(log_file);
    }
}

int is_whitelisted(const char *ip) {
    for (int i = 0; i < whitelist_count; i++) {
        if (strcmp(ip, whitelist[i]) == 0) return 1;
    }
    return 0;
}

int should_blacklist(const char *ip) {
    if (recent_failed_attempts(ip) > 3) {
        return 1;  // Blacklist IP if it exceeds threshold
    }
    return 0;  // Otherwise, warn and monitor
}

int recent_failed_attempts(const char *ip) {
    int count = 0;
    FILE *log_file = fopen("/var/log/failed_attempts.log", "r");
    if (log_file) {
        char line[256];
        while (fgets(line, sizeof(line), log_file)) {
            if (strstr(line, ip)) {
                count++;
            }
        }
        fclose(log_file);
    }
    return count;
}

void add_to_blacklist(const char *ip) {
    if (blacklist_count < MAX_BLACKLIST) {
        blacklist[blacklist_count++] = strdup(ip);
    }
    printf("[Watcher] IP %s added to blacklist.\n", ip);
    log_activity("BLACKLIST", ip);

    // Add IP to the firewall blacklist using ufw
    char command[256];
    snprintf(command, sizeof(command), "sudo ufw deny from %s", ip);
    system(command);

    // Add IP to fail2ban blacklist
    snprintf(command, sizeof(command), "sudo fail2ban-client set sshd banip %s", ip);
    system(command);

    // Save the blacklist to disk for persistence
    save_blacklist();
}

void save_blacklist() {
    FILE *file = fopen("/var/log/blacklist.txt", "w");
    for (int i = 0; i < blacklist_count; i++) {
        fprintf(file, "%s\n", blacklist[i]);
    }
    fclose(file);
    log_activity("SAVE", "Blacklist saved to disk.");
}

void ethical_review(const char *ip) {
    printf("[Watcher] Review flagged activity for IP: %s\n", ip);
    printf("1. Approve Blacklist\n2. Remove from Monitoring\n3. Warn Again\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            add_to_blacklist(ip);
            break;
        case 2:
            printf("[Watcher] IP %s removed from monitoring.\n", ip);
            log_activity("REVIEW", "IP removed from monitoring.");
            break;
        case 3:
            printf("[Watcher] Warning reissued for IP: %s\n", ip);
            log_activity("REVIEW", "Warning reissued.");
            break;
        default:
            printf("[Watcher] Invalid choice.\n");
    }
}
