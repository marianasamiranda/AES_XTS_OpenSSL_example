# AES XTS OpenSSL

This repository provides an example of the Advanced Encryption Standard (AES) block cipher under the 
XTS operation mode.   
It assumes 4096 byte-sized blocks.

Based on https://gist.github.com/ants/862cb941057bdb8db00c72711d2b826c and https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

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

-------
## Dependencies 

The main dependency of this lib is `openssl` (1.1.1j version).
The installation of OpenSSL follows [these](https://rolandsdev.blog/how-to-install-openssl-1-1-1/) steps.

1. <b>Install missing packages and clone OpenSSL</b>
```shell
$ sudo apt-get -y install build-essential checkinstall git zlib1g-dev
$ git clone --depth 1 --branch OpenSSL_1_1_1j https://github.com/openssl/openssl.git
```

2. <b>Configure</b>
```shell
$ cd openssl; ./config zlib '-Wl,-rpath,$(LIBRPATH)'
```

3. <b>Build, test, and install</b>
```shell
$ make
$ make test
$ sudo make install
```

4. <b>Configure shared libs</b>
```shell
$ sudo ldconfig -v
```

5. <b>Validate OpenSSl version</b>
```shell
$ openssl version
# the output should be OpenSSL 1.1.1j  16 Feb 2021
```

## Getting started with AES XTS lib

1. <b>Library set up</b>
```shell
$ git clone https://github.com/rgmacedo/AES_XTS_OpenSSL_example.git
$ cd AES_XTS_OpenSSL_example; mkdir build; cd build;
$ cmake -DCMAKE_INSTALL_PREFIX:PATH=. ..
$ cmake --build .
```

2. <b>Including as external library</b>
```CMake
FetchContent_Declare(
    aesxts
    GIT_REPOSITORY https://github.com/rgmacedo/AES_XTS_OpenSSL_example.git
    GIT_TAG        origin/main
)
FetchContent_MakeAvailable(aesxts)
target_link_libraries(padll aesxts)
```
