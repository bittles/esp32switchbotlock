
#include "HexStringByte.h"

int hexStringToByteArray(const String &hexString, uint8_t *byteArray, size_t length)
{
  size_t l = (hexString.length() / 2 < length) ? hexString.length() / 2 : length;
  for (size_t i = 0; i < l; i++)
  {
    sscanf(hexString.substring(2 * i, 2 * i + 2).c_str(), "%02hhx", &byteArray[i]);
  }
  return l;
}

String byteArrayToHexString(const uint8_t *array, size_t length)
{
  String hexString = "";
  for (size_t i = 0; i < length; i++)
  {
    char hex[3];
    sprintf(hex, "%02x", array[i]);
    hexString += hex;
  }
  return hexString;
}
