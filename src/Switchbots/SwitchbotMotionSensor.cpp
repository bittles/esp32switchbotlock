
#include "SwitchbotMotionSensor.h"

#define SWITCHBOT_MOTION_SENSOR_SERVICE_UUID16  (0xfd3d)

void SwitchbotMotionSensor::onBroadcastScanRspMessage(uint8_t* pMessage, size_t length) {
  if (length < sizeof(MotionSensorBroadcastMessage) + 2) {
    Serial.print("onBroadcastMessage: length ");
    Serial.print(length);
    Serial.print(" is not enough for MotionSensorBroadcastMessage(");
    Serial.print(sizeof(MotionSensorBroadcastMessage));
    Serial.println(") + uuid16(2).");
    return ;
  }

  uint16_t uuid16 = pMessage[0] | (pMessage[1] << 8);
  if (SWITCHBOT_MOTION_SENSOR_SERVICE_UUID16 != uuid16) {
    Serial.print("ERR: invalid service uuid in SwitchbotMotionSensor: 0x");
    Serial.println(String(uuid16, 16).c_str());
    return;
  }

  if (m_pHandler) {
    m_pHandler->onBroadcastMessage((MotionSensorBroadcastMessage*)(pMessage + 2));
  }
}
