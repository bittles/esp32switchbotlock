#ifndef __ENCRYPTOR_H__
#define __ENCRYPTOR_H__

#include "mbedtls/aes.h"
#include <Arduino.h>

class Encryptor
{
private:
  mbedtls_aes_context aes;
  uint8_t _encryption_key[16];
  uint8_t _iv[16];

public:
  Encryptor(const uint8_t *key, const uint8_t *iv);
  ~Encryptor();

  String encrypt(const String &hexStr);
};


#endif // !__ENCRYPTOR_H__
