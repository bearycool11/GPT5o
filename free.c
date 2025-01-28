#include "json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_c_execute(const char* config_path) {
    if (!config_path) {
        fprintf(stderr, "No configuration path provided. Exiting...\n");
        return;
    }

    // Load JSON configuration
    printf("Reading configuration from %s...\n", config_path);
    json_object* config = load_json_from_file(config_path);

    if (!config) {
        fprintf(stderr, "Failed to load %s. Exiting...\n", config_path);
        return;
    }

    // Get the mode from the JSON
    const char* mode = json_get_string(config, "mode");
    if (!mode) {
        fprintf(stderr, "Mode not specified in the configuration file. Exiting...\n");
        json_delete(config);
        return;
    }

    printf("Free.c operating in mode: %s\n", mode);

    // Execute specific tasks based on the mode
    if (strcmp(mode, "cleanup") == 0) {
        printf("Performing cleanup tasks...\n");
        // Add cleanup code here
    } else if (strcmp(mode, "optimize") == 0) {
        printf("Optimizing resources...\n");
        // Add optimization code here
    } else {
        fprintf(stderr, "Unknown mode: %s. Exiting...\n", mode);
    }

    // Clean up the JSON object
    json_delete(config);
}

// Example usage
int main(int argc, char* argv[]) {
    const char* config_path = "free_config.json";
    if (argc > 1) {
        config_path = argv[1]; // Allow user to specify config path
    }
    free_c_execute(config_path);
    return 0;
}

