#ifndef __HEX_STRING_BYTE_H__
#define __HEX_STRING_BYTE_H__

#include <Arduino.h>

int hexStringToByteArray(const String &hexString, uint8_t *byteArray, size_t length);
String byteArrayToHexString(const uint8_t *array, size_t length);

#endif // !__HEX_STRING_BYTE_H__
