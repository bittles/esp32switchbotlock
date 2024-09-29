
#include "SwitchbotApp.h"
#include "DeviceAddr.h"
#include "SensorActions.h"

struct IllumPatternData
{
  uint32_t illumLedPattern;
  uint32_t bubbleColor;
  boolean plugEnabled;
  uint32_t stripColor;
  uint32_t directStripColor;
};

void CSwitchbotApp::createDevices()
{
  // m_pColorBubble1 = new SwitchbotColorBubble(colorBubbleAddr);
  // m_pColorBubble2 = new SwitchbotColorBubble(colorBubbleAddr2);
  // m_pPlugMini = new SwitchbotPlugMini(plugMiniAddr);
  // m_pStripLight = new SwitchbotStripLight(stripLightAddr);
  // m_pContactSensor = new SwitchbotContactSensor(contactSensorAddr);
  // m_pContactSensor->addBroadcastHandler(new ContactSensorAction());
  // m_pMotionSensor = new SwitchbotMotionSensor(motionSensorAddr);
  // m_pMotionSensor->addBroadcastHandler(new MotionSensorAction());

  m_pSmartLock = new SwitchbotLock(lockAddr);
  m_pSmartLock->setKey(lockConfig_keyId, lockConfig_encryption_key);
  

  // devices[numOfDevice++] = m_pColorBubble1;
  // devices[numOfDevice++] = m_pColorBubble2;
  // devices[numOfDevice++] = m_pPlugMini;
  // devices[numOfDevice++] = m_pStripLight;
  // devices[numOfDevice++] = m_pContactSensor;
  // devices[numOfDevice++] = m_pMotionSensor;

  devices[numOfDevice++] = m_pSmartLock;
}

void CSwitchbotApp::onPressButton()
{
  static int index = 0;
  IllumPatternData array[] = {
      {ILLUM_LED_PATTERN_1, 0xff6600, false, 0xff6600, 0xff0000}, //  red
      {ILLUM_LED_PATTERN_2, 0xCC99FF, false, 0xCC99FF, 0x00ff00}, //  green
      {ILLUM_LED_PATTERN_3, 0xffff00, false, 0xffff00, 0x0000ff}, //  yellow?
      {ILLUM_LED_PATTERN_NONE, 0xffffff, false, 0x000000, 0x0},
  };

  Serial.println("CSwitchbotApp::onPressButton()");
  setIllumLedPattern(ILLUM_LED_PATTERN_PROGRESS);

  if (m_pColorBubble1)
  {
    m_pColorBubble1->setEnabled(true);
    m_pColorBubble1->setColor(array[index].bubbleColor);
  }

  if (m_pColorBubble2)
  {
    m_pColorBubble2->setEnabled(true);
    m_pColorBubble2->setColor(array[index].bubbleColor);
  }

  if (m_pPlugMini)
  {
    m_pPlugMini->setEnabled(array[index].plugEnabled);
  }

  if (m_pStripLight)
  {
    m_pStripLight->setEnabled(true);
    m_pStripLight->setColor(array[index].bubbleColor);
  }

  // if (m_pContactSensor)
  // {
  //   m_pContactSensor->getStatus();
  // }

  if (m_pSmartLock)
  {
    m_pSmartLock->setLock(true);
  }

  setIllumLedPattern(array[index++].illumLedPattern);
  index = index % (sizeof(array) / sizeof(array[0]));
}

void CSwitchbotApp::onLongPressButton()
{
  Serial.println("CSwitchbotApp::onLongPressButton()");

  if (m_pColorBubble1)
  {
    m_pColorBubble1->setEnabled(false);
  }
  if (m_pColorBubble2)
  {
    m_pColorBubble2->setEnabled(false);
  }
  if (m_pPlugMini)
  {
    m_pPlugMini->setEnabled(true);
  }
  if (m_pStripLight)
  {
    m_pStripLight->setEnabled(false);
  }
  if (m_pSmartLock)
  {
    m_pSmartLock->setLock(false);
  }
}
