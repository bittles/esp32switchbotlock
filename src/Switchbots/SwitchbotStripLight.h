#ifndef __SWITCHBOT_STRIP_LIGHT_H__
#define __SWITCHBOT_STRIP_LIGHT_H__

#include "SwitchbotBaseDevice.h"

#define SWITCHBOT_STRIP_LIGHT_COMMAND_ON {0x57, 0x0f, 0x49, 0x01, 0x01};
#define SWITCHBOT_STRIP_LIGHT_COMMAND_OFF {0x57, 0x0f, 0x49, 0x01, 0x02};

#define SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_EX_COMMAND (4)
#define SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_LVL (5)
#define SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_R (6)
#define SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_G (7)
#define SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_B (8)

class SwitchbotStripLight : public SwitchbotBaseDevice
{
public:
  uint8_t m_r = 0xff;
  uint8_t m_g = 0xff;
  uint8_t m_b = 0xff;
  uint8_t m_level = 100;

  SwitchbotStripLight(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                             std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                             std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
    m_uuidNotifyCharacteristics = std::string("cba20003-224d-11e6-9fb8-0002a5d5c51b");
  }

  virtual ~SwitchbotStripLight(){};

public:
  bool setEnabled(boolean onOrOff);
  bool setColor(uint32_t color);

protected:
  virtual void onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length);
};

#endif // !__SWITCHBOT_STRIP_LIGHT_H__