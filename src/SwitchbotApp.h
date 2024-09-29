#ifndef __SWITCHBOT_APP_H__
#define __SWITCHBOT_APP_H__

#include <Arduino.h>
#include "Switchbots/SwitchbotDevice.h"
#include "IlluminationDefs.h"

typedef uint32_t SBA_MESSAGE_ID;

#define SBA_MESSAGE_LED_PATTERN_BASE (100)
#define SBA_MESSAGE_PRESS_BUTTON (501)
#define SBA_MESSAGE_LONG_PRESS_BUTTON (502)

#define SBA_DEVICES_NUM SWITCHOBT_MAX_DEVICES

class CSwitchbotApp
{
protected:

  QueueHandle_t queueOnMain = nullptr;
  QueueHandle_t queueOnApp = nullptr;

  SwitchbotBaseDevice *devices[SBA_DEVICES_NUM];
  uint32_t numOfDevice = 0;



  void loop(); // on app task

  void doScan();
  void onScanStart();
  void onScanEnded();

  void setIllumLedPattern(uint32_t illumLedPatter);

  // ********************************************************
  // methods below are implemented in SwitchbotApp_Btn.cpp,
  // since it's related to specific devices...

  /** specific devices */
  SwitchbotColorBubble* m_pColorBubble1 = nullptr;
  SwitchbotColorBubble* m_pColorBubble2 = nullptr;
  SwitchbotPlugMini* m_pPlugMini = nullptr;
  SwitchbotStripLight* m_pStripLight = nullptr;
  SwitchbotContactSensor* m_pContactSensor = nullptr;
  SwitchbotMotionSensor* m_pMotionSensor = nullptr;
  SwitchbotLock* m_pSmartLock = nullptr;

  /** device related operations */
  void createDevices();
  void onPressButton();
  void onLongPressButton();

public:
  CSwitchbotApp(){};
  virtual ~CSwitchbotApp(){};

  void setup(); // on main task
  void task();
  void sendMessage(SBA_MESSAGE_ID messageId);
  boolean receiveMessage(SBA_MESSAGE_ID *pMessageId);
};

class SwitchbotAdvertisedDeviceCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
public:
  SwitchbotAdvertisedDeviceCallbacks(SwitchbotBaseDevice** _devices, uint32_t _numOfDevice) :
    devices(_devices), numOfDevice(_numOfDevice)
  {
  };

protected:
  SwitchbotBaseDevice** devices = nullptr;
  uint32_t numOfDevice = 0;

  virtual void onResult(NimBLEAdvertisedDevice* advertisedDevice);
  boolean isAllDevicesFound();
};

extern CSwitchbotApp SwitchbotApp;

#endif // !__SWITCHBOT_APP_H__
