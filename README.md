# AES_XTS_OpenSSL_example

Example for XTS-AES-256 with a block of data of 4096 bytes.
Based on https://gist.github.com/ants/862cb941057bdb8db00c72711d2b826c and
https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

--------

From https://www.kingston.com/en/solutions/data-security/xts-encryption: "XTS uses two AES keys. One key is used to perform the AES block encryption;
the other is used to encrypt what is known as a tweak."

The OpenSSL library for AES-XTS requires key of double-length. 
In particular, XTS-AES-128 (EVP_aes_128_xts) takes input of a 256-bit (32 bytes) key
to achieve AES 128-bit security, and XTS-AES-256 (EVP_aes_256_xts) takes input of a
512-bit (64 bytes) key to achieve AES 256-bit security. For example, the key 
being 512 bits, internally split into two 256 bit keys for each of the AES 256 ciphers.

--------
The tweak is 16 bytes and translates the position of the data in the storage.

From "Advanced Encryption Standard‐XTS implementation in
field programmable gate array hardware":
"(...) a storage
device that needs to be encrypted. Each data unit is given
a value that should be a non-negative integer, which is
called the tweak value. These tweak values start from an
arbitrary integer and are assigned consecutively. The tweak
value must be converted to a little-endian byte array, where
encryption of the tweak is done using the AES algorithm."


