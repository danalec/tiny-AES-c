![CI](https://github.com/danalec/tiny-AES-c/actions/workflows/c-cpp.yml/badge.svg)

# Tiny AES in C

A small, simple, and portable AES encryption library written in pure C. No dependencies, no external libraries - just drop in the files and use it.

Perfect for embedded systems, microcontrollers, or any project where you need encryption without the complexity of full crypto libraries like OpenSSL.

## What is AES?

AES (Advanced Encryption Standard) is a widely-used symmetric encryption algorithm. It encrypts data in 16-byte blocks using a key. This library supports:

- **AES-128** - 16-byte key (default)
- **AES-192** - 24-byte key  
- **AES-256** - 32-byte key (most secure)

## Encryption Modes

| Mode | Description | Best For |
|------|-------------|----------|
| **ECB** | Simplest, encrypts each block independently | Not recommended for most uses (patterns visible) |
| **CBC** | Chains blocks together with an IV | General-purpose encryption |
| **CTR** | Turns block cipher into stream cipher | Streaming data, random access |

All three modes are enabled by default. You can disable any mode in `aes.h` to reduce code size.

## Quick Start

```c
#include "aes.h"

int main() {
    // Your 128-bit key (16 bytes)
    uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                       0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    
    // Data to encrypt (must be multiple of 16 bytes)
    uint8_t data[32] = "Hello, this is a secret!!";
    
    // Initialize
    struct AES_ctx ctx;
    AES_init_ctx(&ctx, key);
    
    // For CBC mode, you also need an IV (16 bytes, should be random/unique)
    uint8_t iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    AES_init_ctx_iv(&ctx, key, iv);
    
    // Encrypt
    AES_CBC_encrypt_buffer(&ctx, data, 32);
    
    // Decrypt
    AES_CBC_decrypt_buffer(&ctx, data, 32);
    
    return 0;
}
```

## API Reference

### Initialization

```c
// Basic init (for ECB)
void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);

// Init with IV (for CBC/CTR)
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);

// Change IV later
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);
```

### ECB Mode (16-byte blocks)

```c
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);
```

### CBC Mode (buffer of any length, must be multiple of 16)

```c
int AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length);
int AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length);
```

Returns 0 on success, -1 if length isn't a multiple of 16.

### CTR Mode (any length, same function for encrypt/decrypt)

```c
void AES_CTR_xcrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length);
```

## Important Notes

1. **Padding**: CBC and ECB require data to be multiples of 16 bytes. Use [PKCS7 padding](https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7) for arbitrary-length data.

2. **IV uniqueness**: Never reuse the same IV with the same key in CBC or CTR mode - this compromises security.

3. **ECB is weak**: ECB encrypts identical blocks identically, revealing patterns. Avoid for sensitive data.

4. **No error checking**: This is a minimal library. Validate your inputs.

## Compilation

```bash
# Default (AES-128, all modes)
gcc -c aes.c

# AES-256
gcc -DAES256=1 -c aes.c

# Only CTR mode (smallest)
gcc -DCBC=0 -DECB=0 -DCTR=1 -c aes.c
```

## Code Size

| Configuration | ARM (text) | Thumb (text) |
|---------------|------------|--------------|
| CTR only | ~1.1KB | ~900 bytes |
| All modes | ~2KB | ~1.5KB |

Uses less than 200 bytes of RAM.

## Platforms

Works on:
- 64-bit x86/x64
- 32-bit ARM
- 8-bit AVR (Arduino)
- Most embedded platforms

## Testing

```bash
# Build and run tests
make test

# Verify against NIST test vectors
gcc -DAES256=1 verify_aes.c -o verify_aes && ./verify_aes
```

## C++ Usage

C++ users should include `aes.hpp` instead of `aes.h`:

```cpp
#include "aes.hpp"
```

## License

Public domain - use freely for any purpose.

## Credits

Original implementation by [kokke](https://github.com/kokke/tiny-AES-c).  
Thanks to [all contributors](https://github.com/kokke/tiny-AES-c/graphs/contributors) for improvements and fixes.

Verified against [NIST SP 800-38A](http://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf) test vectors.