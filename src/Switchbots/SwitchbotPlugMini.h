#ifndef __SWITCHBOT_PLUG_MINI_H__
#define __SWITCHBOT_PLUG_MINI_H__

#include "SwitchbotBaseDevice.h"

#define SWITCHBOT_PLUG_COMMAND_ON {0x57, 0x0f, 0x50, 0x01, 0x01, 0x80};
#define SWITCHBOT_PLUG_COMMAND_OFF {0x57, 0x0f, 0x50, 0x01, 0x01, 0x00};

class SwitchbotPlugMini : public SwitchbotBaseDevice
{
public:
  SwitchbotPlugMini(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                           std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                           std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
  }

  virtual ~SwitchbotPlugMini(){};

public:
  bool setEnabled(boolean onOrOff)
  {
    uint8_t writeDataOn[] = SWITCHBOT_PLUG_COMMAND_ON;
    uint8_t writeDataOff[] = SWITCHBOT_PLUG_COMMAND_OFF;

    uint8_t *pWriteData = onOrOff ? writeDataOn : writeDataOff;
    size_t writeLen = onOrOff ? (sizeof(writeDataOn) / sizeof(writeDataOn[0])) : (sizeof(writeDataOff) / sizeof(writeDataOff[0]));
    return sendValue(pWriteData, writeLen);
  }
};

#endif // !__SWITCHBOT_PLUG_MINI_H__
