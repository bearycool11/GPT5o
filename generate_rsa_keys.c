#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Generate RSA Keys
RSA* generate_rsa_keys() {
    RSA* rsa = RSA_new();
    BIGNUM* exponent = BN_new();

    if (!rsa || !exponent) {
        fprintf(stderr, "Failed to allocate RSA structure or BIGNUM.\n");
        if (rsa) RSA_free(rsa);
        if (exponent) BN_free(exponent);
        return NULL;
    }

    // Set the public exponent to 65537
    if (!BN_set_word(exponent, RSA_F4)) {
        fprintf(stderr, "Failed to set exponent: %s\n", ERR_error_string(ERR_get_error(), NULL));
        RSA_free(rsa);
        BN_free(exponent);
        return NULL;
    }

    // Generate the RSA key pair
    if (!RSA_generate_key_ex(rsa, 2048, exponent, NULL)) {
        fprintf(stderr, "Failed to generate RSA keys: %s\n", ERR_error_string(ERR_get_error(), NULL));
        RSA_free(rsa);
        BN_free(exponent);
        return NULL;
    }

    BN_free(exponent);
    return rsa;
}

// Save RSA Keys to Files
int save_rsa_keys(RSA* rsa, const char* private_key_file, const char* public_key_file) {
    if (!rsa) {
        fprintf(stderr, "RSA structure is NULL.\n");
        return 0; // Failure
    }

    // Save the private key
    FILE* private_file = fopen(private_key_file, "wb");
    if (!private_file) {
        perror("Failed to open private key file");
        return 0;
    }

    if (!PEM_write_RSAPrivateKey(private_file, rsa, NULL, NULL, 0, NULL, NULL)) {
        fprintf(stderr, "Failed to write private key: %s\n", ERR_error_string(ERR_get_error(), NULL));
        fclose(private_file);
        return 0;
    }
    fclose(private_file);

    // Save the public key
    FILE* public_file = fopen(public_key_file, "wb");
    if (!public_file) {
        perror("Failed to open public key file");
        return 0;
    }

    if (!PEM_write_RSA_PUBKEY(public_file, rsa)) {
        fprintf(stderr, "Failed to write public key: %s\n", ERR_error_string(ERR_get_error(), NULL));
        fclose(public_file);
        return 0;
    }
    fclose(public_file);

    return 1; // Success
}

// Encrypt Data with RSA Public Key
int rsa_encrypt(RSA* rsa, const unsigned char* data, size_t data_len, unsigned char* encrypted, size_t encrypted_len) {
    if (!rsa || !data || !encrypted) {
        fprintf(stderr, "Invalid parameters to rsa_encrypt.\n");
        return -1;
    }

    int result = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    if (result == -1) {
        fprintf(stderr, "RSA encryption failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
    }
    return result; // Returns the size of the encrypted data or -1 on failure
}

// Decrypt Data with RSA Private Key
int rsa_decrypt(RSA* rsa, const unsigned char* encrypted, size_t encrypted_len, unsigned char* decrypted, size_t decrypted_len) {
    if (!rsa || !encrypted || !decrypted) {
        fprintf(stderr, "Invalid parameters to rsa_decrypt.\n");
        return -1;
    }

    int result = RSA_private_decrypt(encrypted_len, encrypted, decrypted, rsa, RSA_PKCS1_OAEP_PADDING);
    if (result == -1) {
        fprintf(stderr, "RSA decryption failed: %s\n", ERR_error_string(ERR_get_error(), NULL));
    }
    return result; // Returns the size of the decrypted data or -1 on failure
}

// Free RSA Resources
void free_rsa(RSA* rsa) {
    if (rsa) {
        RSA_free(rsa);
    }
}

// Example Usage
int main() {
    // Generate RSA Keys
    RSA* rsa = generate_rsa_keys();
    if (!rsa) {
        fprintf(stderr, "Failed to generate RSA keys.\n");
        return EXIT_FAILURE;
    }

    // Save RSA Keys to Files
    if (!save_rsa_keys(rsa, "private_key.pem", "public_key.pem")) {
        fprintf(stderr, "Failed to save RSA keys.\n");
        free_rsa(rsa);
        return EXIT_FAILURE;
    }

    printf("RSA keys saved successfully.\n");

    // Encrypt Data
    const char* message = "This is a test message.";
    unsigned char encrypted[256];
    unsigned char decrypted[256];

    int encrypted_len = rsa_encrypt(rsa, (unsigned char*)message, strlen(message), encrypted, sizeof(encrypted));
    if (encrypted_len == -1) {
        free_rsa(rsa);
        return EXIT_FAILURE;
    }

    printf("Encrypted message length: %d\n", encrypted_len);

    // Decrypt Data
    int decrypted_len = rsa_decrypt(rsa, encrypted, encrypted_len, decrypted, sizeof(decrypted));
    if (decrypted_len == -1) {
        free_rsa(rsa);
        return EXIT_FAILURE;
    }

    decrypted[decrypted_len] = '\0'; // Null-terminate the decrypted string
    printf("Decrypted message: %s\n", decrypted);

    // Clean up
    free_rsa(rsa);
    return EXIT_SUCCESS;
}
