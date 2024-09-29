
#include "SensorActions.h"
// #include "WifiApp.h"

static void printPayload(uint8_t *pPayload, size_t payloadLength)
{
#define PAYLOAD_BUFF_MAX (128)

  static char buff[PAYLOAD_BUFF_MAX];
  memset(buff, 0, PAYLOAD_BUFF_MAX);

  Serial.print("payload (");
  Serial.print(payloadLength);
  Serial.println(") ");
  for (int i = 0; i < payloadLength && i < PAYLOAD_BUFF_MAX - 1; i++)
  {
    // Serial.print(String(pPayload[i], 16).c_str());
    snprintf(buff + i * 3, PAYLOAD_BUFF_MAX - i * 3, "%02x:", pPayload[i]);
  }
  Serial.println(buff);
}

MotionSensorAction::MotionSensorAction() : lightEnabled(false)
{
}

void MotionSensorAction::onBroadcastMessage(MotionSensorBroadcastMessage *pMessage)
{
  Serial.print(">>>  device type: ");
  Serial.print(String(pMessage->deviceType, 16).c_str());
  Serial.print(", lightIntensity: ");
  Serial.print(pMessage->lightIntensity);
  Serial.print(", sensingDistance: ");
  Serial.print(pMessage->sensingDistance);
  Serial.print(", (pirState) moving state: ");
  Serial.print(pMessage->pirState);
  Serial.print(", ledState: ");
  Serial.print(pMessage->ledState);
  Serial.println("");

  // printPayload((uint8_t*) pMessage, sizeof(MotionSensorBroadcastMessage));

  if (lightEnabled && (pMessage->pirState == 0))
  {
    Serial.println("light HUE_LIGHT_OUTSIDE off");
    // WifiApp.setHueLightState(HUE_LIGHT_OUTSIDE, false);
    lightEnabled = false;
  }
  else if (!lightEnabled && (pMessage->pirState == 1) && (pMessage->lightIntensity == 0x01))
  {
    Serial.println("light HUE_LIGHT_OUTSIDE on");
    // WifiApp.setHueLightState(HUE_LIGHT_OUTSIDE, true);
    lightEnabled = true;
  }
}

ContactSensorAction::ContactSensorAction() : lightEnabled(false)
{
}

void ContactSensorAction::onBroadcastMessage(ContactSensorBroadcastMessage *pMessage)
{
  Serial.print(">>> device type: ");
  Serial.print(String(pMessage->deviceType, 16).c_str());
  Serial.print(", open state: ");
  Serial.print(pMessage->openState);
  Serial.print(", moving state: ");
  Serial.print(pMessage->pirState);
  Serial.print(", blight: ");
  Serial.print(pMessage->blight);
  Serial.println("");

  if (lightEnabled && (pMessage->pirState == 0))
  {
    Serial.println("light HUE_LIGHT_ENTRANCE_DOOR off");
    // WifiApp.setHueLightState(HUE_LIGHT_ENTRANCE_DOOR, false);
    lightEnabled = false;
  }
  else if (!lightEnabled && (pMessage->pirState == 1) && (pMessage->blight == 0))
  {
    Serial.println("light HUE_LIGHT_ENTRANCE_DOOR on");
    // WifiApp.setHueLightState(HUE_LIGHT_ENTRANCE_DOOR, true);
    lightEnabled = true;
  }
}