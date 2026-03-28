// Cross-check with OpenSSL
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/evp.h>

static void print_hex(const char* label, const uint8_t* data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) printf("%02x", data[i]);
    printf("\n");
}

int main(void) {
    uint8_t key[32] = {
        0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
    };
    uint8_t pt[16] = {
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17,
        0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };
    uint8_t ct[16];
    
    printf("OpenSSL AES-256-ECB:\n");
    print_hex("Key", key, 32);
    print_hex("PT ", pt, 16);
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL);
    EVP_EncryptUpdate(ctx, ct, &len, pt, 16);
    EVP_CIPHER_CTX_free(ctx);
    
    print_hex("CT ", ct, 16);
    return 0;
}
