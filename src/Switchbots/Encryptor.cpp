
#include "Encryptor.h"
#include "HexStringByte.h"

Encryptor::Encryptor(const uint8_t *key, const uint8_t *iv)
{
  memcpy(_encryption_key, key, 16);
  memcpy(_iv, iv, 16);
  mbedtls_aes_init(&aes);
  mbedtls_aes_setkey_enc(&aes, _encryption_key, 128);
}

Encryptor::~Encryptor()
{
  mbedtls_aes_free(&aes);
}

String Encryptor::encrypt(const String &hexStr)
{
  // Convert hexadecimal strings to byte strings
  size_t length = hexStr.length() / 2;
  uint8_t *input = new uint8_t[length];
  hexStringToByteArray(hexStr, input, length);

  uint8_t *output = new uint8_t[length];
  uint8_t nonce_counter[16];
  uint8_t stream_block[16];
  size_t nc_off = 0;

  memcpy(nonce_counter, _iv, 16);

  mbedtls_aes_crypt_ctr(&aes, length, &nc_off, nonce_counter, stream_block,
                        input, output);

  // Convert the result to a hexadecimal string
  String result = byteArrayToHexString(output, length);

  delete[] input;
  delete[] output;
  return result;
}
