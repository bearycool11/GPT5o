#ifndef JSON_H
#define JSON_H

#include <stddef.h> // For size_t

// Forward declaration for the cJSON structure
typedef struct cJSON cJSON;

/**
 * Parses a JSON string and returns a JSON object.
 * @param json_str The JSON string to parse.
 * @return A pointer to the parsed JSON object, or NULL on failure.
 *         The caller is responsible for freeing the returned object with `json_delete`.
 */
cJSON* json_parse(const char* json_str);

/**
 * Converts a JSON object to a formatted string.
 * @param json_obj The JSON object to stringify.
 * @return A newly allocated string representation of the JSON object.
 *         The caller is responsible for freeing the string using `free()`.
 */
char* json_stringify(const cJSON* json_obj);

/**
 * Retrieves a string value from a JSON object.
 * @param json_obj The JSON object to query.
 * @param key The key to look up.
 * @return The string value associated with the key, or NULL if not found or not a string.
 */
const char* json_get_string(const cJSON* json_obj, const char* key);

/**
 * Adds or updates a key-value pair in a JSON object.
 * @param json_obj The JSON object to modify.
 * @param key The key to add or update.
 * @param value The string value to associate with the key.
 * @return 1 on success, 0 on failure.
 */
int json_set_string(cJSON* json_obj, const char* key, const char* value);

/**
 * Deletes a JSON object and frees its memory.
 * @param json_obj The JSON object to delete.
 */
void json_delete(cJSON* json_obj);

#endif // JSON_H
