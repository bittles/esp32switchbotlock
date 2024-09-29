#ifndef __SWITCHBOT_COLOR_BUBBLE_H__
#define __SWITCHBOT_COLOR_BUBBLE_H__

#include "SwitchbotBaseDevice.h"

#define SWITCHBOT_COLOR_BUBBLE_COMMAND_ON {0x57, 0x0f, 0x47, 0x01, 0x01};
#define SWITCHBOT_COLOR_BUBBLE_COMMAND_OFF {0x57, 0x0f, 0x47, 0x01, 0x02};

#define SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_EX_COMMAND (4)
#define SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_LVL (5)
#define SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_R (6)
#define SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_G (7)
#define SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_B (8)

class SwitchbotColorBubble : public SwitchbotBaseDevice
{
public:
  uint8_t m_r = 0xff;
  uint8_t m_g = 0xff;
  uint8_t m_b = 0xff;
  uint8_t m_level = 100;
  uint16_t m_cw = 0;

public:
  SwitchbotColorBubble(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                              std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                              std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
  }

  virtual ~SwitchbotColorBubble(){};

public:
  bool setEnabled(boolean onOrOff)
  {
    uint8_t writeDataOn[] = SWITCHBOT_COLOR_BUBBLE_COMMAND_ON;
    uint8_t writeDataOff[] = SWITCHBOT_COLOR_BUBBLE_COMMAND_OFF;

    uint8_t *pWriteData = onOrOff ? writeDataOn : writeDataOff;
    size_t writeLen = onOrOff ? (sizeof(writeDataOn) / sizeof(writeDataOn[0])) : (sizeof(writeDataOff) / sizeof(writeDataOff[0]));
    return sendValue(pWriteData, writeLen);
  }

  bool setColor(uint32_t color) // 0xff0000 for r
  {
    uint8_t writeData[] = {0x57, 0x0f, 0x47, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

    m_r = (uint8_t)((color >> 16) & 0xff);
    m_g = (uint8_t)((color >> 8) & 0xff);
    m_b = (uint8_t)((color >> 0) & 0xff);

    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_EX_COMMAND] = 0x12;
    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_LVL] = m_level;
    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_R] = m_r;
    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_G] = m_g;
    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_B] = m_b;

    return sendValue(writeData, sizeof(writeData) / sizeof(writeData[0]));
  }

  bool setLevel(uint8_t level)
  {
    uint8_t writeData[] = {0x57, 0x0f, 0x47, 0x01, 0x00, 0x00};

    m_level = level;

    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_EX_COMMAND] = 0x14;
    writeData[SWITCHBOT_COLOR_BUBBLE_WRITE_OFFSET_LVL] = m_level;

    return sendValue(writeData, sizeof(writeData) / sizeof(writeData[0]));
  }
};

#endif // !__SWITCHBOT_COLOR_BUBBLE_H__