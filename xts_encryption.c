#include "xts_encryption.h"

int KEY_SIZE;
int OPERATION_MODE;


void auth_handleErrors(void) {
      	// ERR_print_errors_fp(stderr);
    // abort();
}

const EVP_CIPHER* auth_get_cipher() {
    switch (KEY_SIZE) {
        case 32:
            return EVP_aes_128_xts();
        default:
            return EVP_aes_256_xts();
    }
}


int xts_init(int key_size) {
    // if (key == NULL) {
    //     // ERROR_MSG("(symmetric.c) - init's key argument is NULL");
    //     return 1;
    // }

    // ERR_load_crypto_strings();
    // OpenSSL_add_all_algorithms();
    // OPENSSL_config(NULL);

    KEY_SIZE = key_size;
    // KEY = (unsigned char*) key;
    // DEBUG_MSG("auth_encryption_init: operation mode = %d\n", OPERATION_MODE);

    return 0;
}

int xts_encode(unsigned char* key, unsigned char* tweak, unsigned char* dest, const unsigned char* src, int size) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) {
	    auth_handleErrors();
    }

    /* Initialise the encryption operation. */
    if(1 != EVP_EncryptInit_ex(ctx, auth_get_cipher(), NULL, key, tweak)) {
	    auth_handleErrors();
    }

    /* Provide the message to be encrypted, and obtain the encrypted output.
     * EVP_EncryptUpdate can be called multiple times if necessary
     */
    if(1 != EVP_EncryptUpdate(ctx, dest, &len, src, size))  {
            auth_handleErrors();
    } 
    ciphertext_len = len;

    /* Finalise the encryption. Normally ciphertext bytes may be written at
     * this stage, but this does not occur in GCM mode
     */
    if(1 != EVP_EncryptFinal_ex(ctx, dest + len, &len)) {
            auth_handleErrors();
    } 

    ciphertext_len += len;


    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int xts_decode(unsigned char* key, unsigned char* tweak, unsigned char* dest, const unsigned char* src, int size) {
    EVP_CIPHER_CTX *ctx;
    int len;
    int plaintext_len;
    int ret;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new())) auth_handleErrors();

    /* Initialise the decryption operation. */
    if(!EVP_DecryptInit_ex(ctx, auth_get_cipher(), NULL, key, tweak)) auth_handleErrors();
    
    /* Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary
     */
    if(!EVP_DecryptUpdate(ctx, dest, &len, src, size)) auth_handleErrors();
    plaintext_len = len;

    /* Finalise the decryption. A positive return value indicates success,
     * anything else is a failure - the plaintext is not trustworthy.
     */
    ret = EVP_DecryptFinal_ex(ctx, dest + len, &len);

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    if(ret > 0) {
        /* Success */
        plaintext_len += len;
        // DEBUG_MSG("Decode: success\n");
        return plaintext_len;
    } else {
        /* Verify failed */
       // DEBUG_MSG("Decode: fail\n");
        return -1;
    }
}

int xts_clean() {
    return 0;
}

