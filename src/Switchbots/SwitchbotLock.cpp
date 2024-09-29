
#include "SwitchbotLock.h"
#include "Encryptor.h"
#include "HexStringByte.h"

const char COMMAND_GET_CK_IV[] = "570f2103";
const char COMMAND_LOCK_INFO[] = "570f4f8101";
const char COMMAND_UNLOCK[] = "570f4e01011080";
const char COMMAND_UNLOCK_NO_UNLATCH[] = "570f4e010110a0";
const char COMMAND_LOCK[] = "570f4e01011000";

void SwitchbotLock::_getIv()
{

#define GET_IV_BUFFER_LENGTH (64)

  String key = COMMAND_GET_CK_IV;
  key += _keyId;

  // port typescript code below:
  //  const req = Buffer.from(key.substring(0, 2) + '000000' + key.substring(2), 'hex');
  String request = key.substring(0, 2) + "000000" + key.substring(2);

  // Serial.println("_getIV() request string:");
  // Serial.println(request.c_str());

  unsigned char buff[GET_IV_BUFFER_LENGTH];
  size_t l = hexStringToByteArray(request, buff, GET_IV_BUFFER_LENGTH);
  if (0 < l)
  {
    sendValue(buff, l, false);
  }
  else
  {
    Serial.print("can not convert command buffer on _getIv()");
  }
}

void SwitchbotLock::_operateLock(const String &key, const uint8_t *iv, size_t ivLength)
{
  uint8_t enc_key[16];
  if (hexStringToByteArray(_encryptionKey, enc_key, 16) < 0)
  {
    Serial.println("something wrong in encryptionKey string value!");
    return;
  }

  Encryptor encryptor(enc_key, iv);

  String x;
  String req;
  req += key.substring(0, 2);
  req += _keyId;
  req += byteArrayToHexString(iv, 2);
  req += encryptor.encrypt(key.substring(2));

  // Serial.println("_operateLock() request string:");
  // Serial.println(a.c_str());

#define GET_OPERATE_LOCK_BUFFER_LENGTH (64)
  unsigned char buff[GET_OPERATE_LOCK_BUFFER_LENGTH];

  size_t l = hexStringToByteArray(req, buff, GET_OPERATE_LOCK_BUFFER_LENGTH);
  if (0 < l)
  {
    sendValue(buff, l, false);
  }
  else
  {
    Serial.print("can not convert command buffer on _getIv()");
  }

  // 上のコードはtypescriptの以下のコードをポーティングしたもの↑
  //   const iv = await this._getIv();
  //   const req = Buffer.from(
  //     key.substring(0, 2) + this._key_id + Buffer.from(iv.subarray(0, 2)).toString('hex') + this._encrypt(key.substring(2))
  //     , 'hex');
  //   const bytes: unknown = await this._command(req);
}

bool SwitchbotLock::setLock(boolean lock)
{
  _callState = lock ? State_GetIvg_Lock : State_GetIvg_Unlock;
  _getIv();
  return true;
}

void SwitchbotLock::setKey(const String &keyId, const String &encryptionKey)
{
  _keyId = keyId;
  _encryptionKey = encryptionKey;
}

// ivは戻ってきた値の4バイト以降のデータ
// async _getIv(): Promise<Buffer> {
//   if (this._iv === null) {
//     const res = await this._operateLock(WoSmartLock.COMMAND_GET_CK_IV + this._key_id, false);
//     this._iv = res.subarray(4);
//   }
//   return this._iv;
// }
void SwitchbotLock::onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length)
{
  Serial.println("onReceiveNotification: i got notification!\n");

  switch (_callState)
  {
  case State_GetIvg_Lock:
  {
    _operateLock(COMMAND_LOCK, pData + 4, length - 4);
  }
  break;
  case State_GetIvg_Unlock:
  {
    _operateLock(COMMAND_UNLOCK, pData + 4, length - 4);
  }
  break;
  case State_None:
  {
    /** nothing to do here! */
  }
  break;
  }

  _callState = State_None;
}
