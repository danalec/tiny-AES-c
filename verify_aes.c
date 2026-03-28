// Verify AES-256-ECB using tiny-AES-c as reference
// Compile: gcc -o verify_aes verify_aes.c -I. -O2

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// Include tiny-AES-c directly (with AES256 enabled)
#define AES256 1
#include "aes.c"

static void print_hex(const char* label, const uint8_t* data, int len) {
    printf("%s: ", label);
    for (int i = 0; i < len; i++) printf("%02x", data[i]);
    printf("\n");
}

int main(void) {
    printf("=== AES-256-ECB Verification (tiny-AES-c) ===\n\n");
    
    // NIST test vectors
    // https://csrc.nist.gov/Projects/Cryptographic-Algorithm-Validation-Program/Block-Ciphers
    
    // Test 1: All zeros
    uint8_t key1[32] = {0};
    uint8_t pt1[16] = {0};
    uint8_t expected1[16] = {
        0xdc, 0x95, 0xc0, 0x78, 0xa2, 0x40, 0x89, 0x89,
        0xad, 0x48, 0xa2, 0x14, 0x92, 0x84, 0x20, 0x87
    };
    
    printf("Test 1: All zeros (NIST test vector)\n");
    print_hex("Key       ", key1, 32);
    print_hex("Plaintext ", pt1, 16);
    print_hex("Expected  ", expected1, 16);
    
    struct AES_ctx ctx1;
    AES_init_ctx(&ctx1, key1);
    AES_ECB_encrypt(&ctx1, pt1);
    
    print_hex("Ciphertext", pt1, 16);
    
    int pass1 = (memcmp(pt1, expected1, 16) == 0);
    printf("Result: %s\n\n", pass1 ? "PASS" : "FAIL");
    
    // Test 2: Sequential key/plaintext
    uint8_t key2[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    uint8_t pt2[16] = {
        0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
    };
    uint8_t expected2[16] = {
        0x8e, 0xa2, 0xb7, 0xca, 0x51, 0x67, 0x45, 0xbf,
        0xea, 0xfc, 0x49, 0x90, 0x4b, 0x49, 0x60, 0x89
    };
    
    printf("Test 2: Sequential key/plaintext\n");
    print_hex("Key       ", key2, 32);
    print_hex("Plaintext ", pt2, 16);
    print_hex("Expected  ", expected2, 16);
    
    struct AES_ctx ctx2;
    AES_init_ctx(&ctx2, key2);
    AES_ECB_encrypt(&ctx2, pt2);
    
    print_hex("Ciphertext", pt2, 16);
    
    int pass2 = (memcmp(pt2, expected2, 16) == 0);
    printf("Result: %s\n\n", pass2 ? "PASS" : "FAIL");
    
    // Test 3: IETF test vector
    uint8_t key3[32] = {
        0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
    };
    uint8_t pt3[16] = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a
    };
    uint8_t expected3[16] = {
        0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c,
        0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8
    };
    
    printf("Test 3: NIST SP 800-38A AES-256-ECB test vector\n");
    print_hex("Key       ", key3, 32);
    print_hex("Plaintext ", pt3, 16);
    print_hex("Expected  ", expected3, 16);
    
    struct AES_ctx ctx3;
    AES_init_ctx(&ctx3, key3);
    AES_ECB_encrypt(&ctx3, pt3);
    
    print_hex("Ciphertext", pt3, 16);
    
    int pass3 = (memcmp(pt3, expected3, 16) == 0);
    printf("Result: %s\n\n", pass3 ? "PASS" : "FAIL");
    
    // Test 4: IETF RFC 3962 test vector (AES-256-CTS, but ECB works for single block)
    uint8_t key4[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    uint8_t pt4[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    uint8_t expected4[16] = {
        0x5a, 0x6e, 0x04, 0x57, 0x08, 0xfb, 0x71, 0x96,
        0xf0, 0x2e, 0x55, 0x3d, 0x02, 0xc3, 0xa6, 0x92
    };
    
    printf("Test 4: IETF RFC test vector\n");
    print_hex("Key       ", key4, 32);
    print_hex("Plaintext ", pt4, 16);
    print_hex("Expected  ", expected4, 16);
    
    struct AES_ctx ctx4;
    AES_init_ctx(&ctx4, key4);
    AES_ECB_encrypt(&ctx4, pt4);
    
    print_hex("Ciphertext", pt4, 16);
    
    int pass4 = (memcmp(pt4, expected4, 16) == 0);
    printf("Result: %s\n\n", pass4 ? "PASS" : "FAIL");
    
    printf("=== Summary ===\n");
    printf("Test 1 (NIST zeros):   %s\n", pass1 ? "PASS" : "FAIL");
    printf("Test 2 (sequential):   %s\n", pass2 ? "PASS" : "FAIL");
    printf("Test 3 (NIST SP800):   %s\n", pass3 ? "PASS" : "FAIL");
    printf("Test 4 (IETF RFC):     %s\n", pass4 ? "PASS" : "FAIL");
    
    if (pass1 && pass2 && pass3 && pass4) {
        printf("\ntiny-AES-c verified correct! Use as reference.\n");
    } else {
        printf("\nSome tests failed!\n");
    }
    
    return (pass1 && pass2 && pass3 && pass4) ? 0 : 1;
}