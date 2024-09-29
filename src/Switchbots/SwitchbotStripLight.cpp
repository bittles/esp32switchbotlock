
#include "SwitchbotStripLight.h"

bool SwitchbotStripLight::setEnabled(boolean onOrOff)
{
  uint8_t writeDataOn[] = SWITCHBOT_STRIP_LIGHT_COMMAND_ON;
  uint8_t writeDataOff[] = SWITCHBOT_STRIP_LIGHT_COMMAND_OFF;

  uint8_t *pWriteData = onOrOff ? writeDataOn : writeDataOff;
  size_t writeLen = onOrOff ? (sizeof(writeDataOn) / sizeof(writeDataOn[0])) : (sizeof(writeDataOff) / sizeof(writeDataOff[0]));
  return sendValue(pWriteData, writeLen);
}

bool SwitchbotStripLight::setColor(uint32_t color) // 0xff0000 for r
{
  uint8_t writeData[] = {0x57, 0x0f, 0x49, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

  m_r = (uint8_t)((color >> 16) & 0xff);
  m_g = (uint8_t)((color >> 8) & 0xff);
  m_b = (uint8_t)((color >> 0) & 0xff);

  writeData[SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_EX_COMMAND] = 0x12;
  writeData[SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_LVL] = m_level;
  writeData[SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_R] = m_r;
  writeData[SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_G] = m_g;
  writeData[SWITCHBOT_STRIP_LIGHT_WRITE_OFFSET_B] = m_b;

  return sendValue(writeData, sizeof(writeData) / sizeof(writeData[0]));
}

void SwitchbotStripLight::onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length)
{
  Serial.println("onReceiveNotification: i got notification!");
}
