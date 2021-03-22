#include <aesxts/xts/xts.hpp>

#define KEY_SIZE 64 
#define TWEAK_SIZE  16

/*
Example for XTS-AES-256 with a block of data of 4096 bytes.
Based on https://gist.github.com/ants/862cb941057bdb8db00c72711d2b826c and
https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

From https://www.kingston.com/en/solutions/data-security/xts-encryption
"XTS uses two AES keys. One key is used to perform the AES block encryption;
the other is used to encrypt what is known as a tweak."

The OpenSSL library for AES-XTS, requires key of double-length. 
In particular, XTS-AES-128 (EVP_aes_128_xts) takes input of a 256-bit (32 bytes) key
to achieve AES 128-bit security, and XTS-AES-256 (EVP_aes_256_xts) takes input of a
512-bit (64 bytes) key to achieve AES 256-bit security. So, the key 
being 512 bits, internally split into two 256 bit keys for each of the AES 256 ciphers.


The tweak is 16 bytes and translates the position of the data in the storage.
From "Advanced Encryption Standard‐XTS implementation in
field programmable gate array hardware"
"(...) a storage
device that needs to be encrypted. Each data unit is given
a value that should be a non-negative integer, which is
called the tweak value. These tweak values start from an
arbitrary integer and are assigned consecutively. The tweak
value must be converted to a little-endian byte array, where
encryption of the tweak is done using the AES algorithm."

*/


int encode(unsigned char* key, uint8_t *tweak, uint8_t *dest, uint8_t* src, size_t src_size) {

    int res;

    res = xts_encode(key, tweak, dest, src, src_size);
    if (res <= 0) {
                printf("Trying again<T> Encode Error -> xts_encode return %d\n", res);
    }
    return res;
}

int decode(unsigned char* key, uint8_t *tweak, uint8_t *dest, uint8_t* src, size_t src_size) {

    int res;
    res = xts_decode(key, tweak, dest, src, src_size);
    if (res <= 0) {
                printf("Trying again<T> Decode Error -> xts_decode return %d\n", res);
    }
    return res;
}

int main (void)
{
  /* Set up the key and iv. Do I need to say to not hard code these in a
   * real application? :-)
   */

  /* A 512 bit key */
  unsigned char *key = (unsigned char *)"0123456789012345678901234567890123456789012345678901234567890123";

  /* A 128 bit tweak */
  unsigned char *tweak = (unsigned char *)"0123456789012345";

  /* Message to be encrypted */
  unsigned char plaintext[4096];
  int plaintext_len = 4096;

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */
  unsigned char ciphertext[4096];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[4096];

  int decryptedtext_len, ciphertext_len;

  memset(plaintext, 'x', 4096);

  plaintext[plaintext_len] = '\0';


  /* Show the decrypted text */
  printf("Plaintext text is:\n");
  printf("%s\n", plaintext);


  xts_init(KEY_SIZE);

  /* Encrypt the plaintext */
  ciphertext_len = encode(key, tweak, ciphertext, plaintext, plaintext_len);


  /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */
  decryptedtext_len = decode(key, tweak, decryptedtext, ciphertext, ciphertext_len);

  printf("%d\n", decryptedtext_len);
  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n");
  printf("%s\n", decryptedtext);

  return 0;
}
