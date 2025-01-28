#include "json.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Parses a JSON string into a cJSON object.
 * @param json_str The input JSON string.
 * @return A pointer to the parsed cJSON object or NULL on error.
 */
cJSON* json_parse(const char* json_str) {
    if (!json_str) {
        fprintf(stderr, "Input JSON string is NULL.\n");
        return NULL;
    }

    cJSON* json_obj = cJSON_Parse(json_str);
    if (!json_obj) {
        fprintf(stderr, "JSON Parsing Error: %s\n", cJSON_GetErrorPtr());
    }
    return json_obj;
}

/**
 * Converts a cJSON object into a JSON string.
 * @param json_obj The input cJSON object.
 * @return A JSON string or NULL on error. Must be freed by the caller.
 */
char* json_stringify(const cJSON* json_obj) {
    if (!json_obj) {
        fprintf(stderr, "JSON Object is NULL. Cannot stringify.\n");
        return NULL;
    }

    char* json_str = cJSON_Print(json_obj);
    if (!json_str) {
        fprintf(stderr, "Error during JSON stringification.\n");
    }
    return json_str;
}

/**
 * Retrieves the value of a string from a cJSON object by key.
 * @param json_obj The cJSON object.
 * @param key The key to retrieve.
 * @return The string value or NULL if the key is not found or not a string.
 */
const char* json_get_string(const cJSON* json_obj, const char* key) {
    if (!json_obj || !key) {
        fprintf(stderr, "Invalid parameters to json_get_string.\n");
        return NULL;
    }

    cJSON* item = cJSON_GetObjectItemCaseSensitive(json_obj, key);
    if (cJSON_IsString(item) && (item->valuestring != NULL)) {
        return item->valuestring;
    }

    fprintf(stderr, "Key '%s' not found or not a string.\n", key);
    return NULL;
}

/**
 * Sets a string value for a key in a cJSON object.
 * @param json_obj The cJSON object.
 * @param key The key to set.
 * @param value The string value to set.
 * @return 1 on success, 0 on failure.
 */
int json_set_string(cJSON* json_obj, const char* key, const char* value) {
    if (!json_obj || !key || !value) {
        fprintf(stderr, "Invalid parameters to json_set_string.\n");
        return 0; // Indicate failure
    }

    cJSON* item = cJSON_GetObjectItemCaseSensitive(json_obj, key);
    if (item) {
        if (cJSON_IsString(item)) {
            cJSON_SetValuestring(item, value);
        } else {
            fprintf(stderr, "Key '%s' exists but is not a string. Overwriting.\n", key);
            cJSON_ReplaceItemInObject(json_obj, key, cJSON_CreateString(value));
        }
    } else {
        cJSON_AddStringToObject(json_obj, key, value);
    }

    return 1; // Indicate success
}

/**
 * Deletes a cJSON object.
 * @param json_obj The cJSON object to delete.
 */
void json_delete(cJSON* json_obj) {
    if (!json_obj) {
        fprintf(stderr, "Invalid JSON object for deletion.\n");
        return;
    }
    cJSON_Delete(json_obj);
}
