#ifndef __XTS_ENCRYPTION_H__
#define __XTS_ENCRYPTION_H__

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>


int xts_init(int key_size);

int xts_encode(unsigned char* key, unsigned char* tweak, unsigned char* dest, const unsigned char* src, int size);

int xts_decode(unsigned char* key, unsigned char* tweak, unsigned char* dest, const unsigned char* src, int size);

int xts_clean();

void auth_handleErrors(void);

#endif
