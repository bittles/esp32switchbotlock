#ifndef __SWITCHBOT_LOCK_H__
#define __SWITCHBOT_LOCK_H__

#include "SwitchbotBaseDevice.h"

#define SWITCHBOT_LOCK_COMMAND_ON {0x57, 0x0f, 0x49, 0x01, 0x01};
#define SWITCHBOT_LOCK_COMMAND_OFF {0x57, 0x0f, 0x49, 0x01, 0x02};

class SwitchbotLock : public SwitchbotBaseDevice
{
public:
  SwitchbotLock(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                          std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                          std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
    m_uuidNotifyCharacteristics = std::string("cba20003-224d-11e6-9fb8-0002a5d5c51b");
    _callState = State_None;
  }

  virtual ~SwitchbotLock() {};

protected:
  String  _keyId;
  String  _encryptionKey;

  enum CallState {
    State_None,
    State_GetIvg_Lock,
    State_GetIvg_Unlock,
  } _callState;

  void _getIv();
  void _operateLock(const String &key, const uint8_t* iv, size_t ivLength);

public:
  bool setLock(boolean lock);
  void setKey(const String& keyId, const String& encryptionKey);

protected:
  virtual void onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length);
};

#endif // !__SWITCHBOT_LOCK_H__