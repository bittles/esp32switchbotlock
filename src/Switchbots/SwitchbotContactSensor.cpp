
#include "SwitchbotContactSensor.h"

#define SWITCHBOT_CONTACT_SENSOR_SERVICE_UUID16 (0xfd3d)

bool SwitchbotContactSensor::getStatus() {
  uint8_t writeData[] = SWITCHBOT_CONTACT_SENSOR_COMMAND_GET_STATUS;

  size_t writeLen = (sizeof(writeData) / sizeof(writeData[0]));
  return sendValue(writeData, writeLen);
}

void SwitchbotContactSensor::onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length) {
  Serial.println("onReceiveNotification: contact sensor!");
}

void SwitchbotContactSensor::onBroadcastScanRspMessage(uint8_t* pMessage, size_t length) {
  if (length < sizeof(ContactSensorBroadcastMessage) + 2) {
    Serial.print("onBroadcastMessage: length ");
    Serial.print(length);
    Serial.print(" is not enough for ContactSensorBroadcastMessage(");
    Serial.print(sizeof(ContactSensorBroadcastMessage));
    Serial.println(") + uuid16(2).");
    return ;
  }

  uint16_t uuid16 = pMessage[0] | (pMessage[1] << 8);
  if (SWITCHBOT_CONTACT_SENSOR_SERVICE_UUID16 != uuid16) {
    Serial.print("ERR: invalid service uuid in SwitchbotContactSensor: 0x");
    Serial.println(String(uuid16, 16).c_str());
    return;
  }

  if (m_pHandler) {
    m_pHandler->onBroadcastMessage((ContactSensorBroadcastMessage*)(pMessage + 2));
  }
}
