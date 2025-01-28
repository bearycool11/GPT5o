#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

// Constants
#define PASSWORD "secure_password"
#define ENCRYPTED_KEY_FILE "encrypted_key.dat"
#define STORED_HASH_FILE "stored_hash.dat"

// Function to generate a hash for the private key
void generate_hash(const char *private_key, unsigned char *hash) {
    SHA256((unsigned char *)private_key, strlen(private_key), hash);
}

// Encrypt the private key
int encrypt_private_key(const char *private_key, unsigned char **encrypted_key, int *encrypted_len) {
    unsigned char salt[8], key[32], iv[16];
    int len;

    // Generate random salt
    if (!RAND_bytes(salt, sizeof(salt))) {
        fprintf(stderr, "Failed to generate salt\n");
        return -1;
    }

    // Derive key and IV from password and salt
    if (!PKCS5_PBKDF2_HMAC_SHA1(PASSWORD, strlen(PASSWORD), salt, sizeof(salt), 10000, sizeof(key), key)) {
        fprintf(stderr, "Key derivation failed\n");
        return -1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Failed to create cipher context\n");
        return -1;
    }

    // Initialize encryption context
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    *encrypted_key = malloc(strlen(private_key) + AES_BLOCK_SIZE);
    if (!*encrypted_key) {
        fprintf(stderr, "Memory allocation failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    // Encrypt the private key
    EVP_EncryptUpdate(ctx, *encrypted_key, &len, (unsigned char *)private_key, strlen(private_key));
    *encrypted_len = len;

    EVP_EncryptFinal_ex(ctx, *encrypted_key + len, &len);
    *encrypted_len += len;

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

// Save encrypted key to file
void save_encrypted_key(const unsigned char *encrypted_key, int len, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for writing");
        return;
    }
    fwrite(encrypted_key, 1, len, file);
    fclose(file);
}

// Load encrypted key from file
unsigned char *load_encrypted_key(const char *filename, int *len) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file for reading");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *len = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(*len);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *len, file);
    fclose(file);
    return buffer;
}

// Decrypt the private key
int decrypt_private_key(const unsigned char *encrypted_key, int encrypted_len, char **decrypted_key) {
    unsigned char salt[8], key[32], iv[16];
    int len;

    // Derive key and IV from password
    if (!PKCS5_PBKDF2_HMAC_SHA1(PASSWORD, strlen(PASSWORD), salt, sizeof(salt), 10000, sizeof(key), key)) {
        fprintf(stderr, "Key derivation failed\n");
        return -1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Failed to create cipher context\n");
        return -1;
    }

    // Initialize decryption context
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    *decrypted_key = malloc(encrypted_len + 1); // Allocate memory for decrypted key
    if (!*decrypted_key) {
        fprintf(stderr, "Memory allocation failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    // Decrypt the private key
    EVP_DecryptUpdate(ctx, (unsigned char *)*decrypted_key, &len, encrypted_key, encrypted_len);
    int total_len = len;

    EVP_DecryptFinal_ex(ctx, (unsigned char *)*decrypted_key + len, &len);
    total_len += len;

    (*decrypted_key)[total_len] = '\0'; // Null-terminate the decrypted string

    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

// Validate the hash of the private key
int validate_hash(const char *private_key, const unsigned char *stored_hash) {
    unsigned char computed_hash[SHA256_DIGEST_LENGTH];
    generate_hash(private_key, computed_hash);
    return memcmp(computed_hash, stored_hash, SHA256_DIGEST_LENGTH) == 0;
}

// Save hash to file
void save_hash(const unsigned char *hash, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Unable to open file for writing");
        return;
    }
    fwrite(hash, 1, SHA256_DIGEST_LENGTH, file);
    fclose(file);
}

// Load hash from file
unsigned char *load_hash(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Unable to open file for reading");
        return NULL;
    }

    unsigned char *hash = malloc(SHA256_DIGEST_LENGTH);
    if (!hash) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(hash, 1, SHA256_DIGEST_LENGTH, file);
    fclose(file);
    return hash;
}

// Securely clear memory
void secure_clear(void *v, size_t n) {
    volatile unsigned char *p = v;
    while (n--) *p++ = 0;
}

// Main Function
int main() {
    const char *private_key = "your-private-key-string";

    // Step 1: Generate and store hash
    unsigned char hash[SHA256_DIGEST_LENGTH];
    generate_hash(private_key, hash);
    save_hash(hash, STORED_HASH_FILE);

    // Step 2: Encrypt and store private key
    unsigned char *encrypted_key = NULL;
    int encrypted_len;
    if (encrypt_private_key(private_key, &encrypted_key, &encrypted_len) == 0) {
        save_encrypted_key(encrypted_key, encrypted_len, ENCRYPTED_KEY_FILE);
        free(encrypted_key);
    }

    // Step 3: Load and decrypt private key
    int len;
    unsigned char *loaded_encrypted_key = load_encrypted_key(ENCRYPTED_KEY_FILE, &len);
    char *decrypted_key = NULL;
    if (decrypt_private_key(loaded_encrypted_key, len, &decrypted_key) == 0) {
        printf("Decrypted Key: %s\n", decrypted_key);
    }
    free(loaded_encrypted_key);

    // Step 4: Validate hash
    unsigned char *loaded_hash = load_hash(STORED_HASH_FILE);
    if (validate_hash(decrypted_key, loaded_hash)) {
        printf("Hash validated successfully!\n");
    } else {
        fprintf(stderr, "Hash validation failed.\n");
    }
    free(decrypted_key);
    free(loaded_hash);

    return 0;
}
