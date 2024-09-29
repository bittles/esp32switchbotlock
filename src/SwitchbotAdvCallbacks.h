#ifndef __SWITCHOBT_TOUCH_SENSOR_CALLBACKS_H__
#define __SWITCHOBT_TOUCH_SENSOR_CALLBACKS_H__

#include <Arduino.h>
#include "Switchbots/SwitchbotDevice.h"

class SwitchbotAdvCallbacks : public NimBLEAdvertisedDeviceCallbacks
{
public:
  SwitchbotAdvCallbacks(SwitchbotBaseDevice** _devices, uint32_t _numOfDevice) :
    devices(_devices), numOfDevice(_numOfDevice)
  {
  };

protected:
  SwitchbotBaseDevice** devices = nullptr;
  uint32_t numOfDevice = 0;

  virtual void onResult(NimBLEAdvertisedDevice* advertisedDevice);
  void handlePayload(uint8_t* pPayload, size_t payloadLength);
  void callDevice(SwitchbotBaseDevice* device, uint8_t* pPayload, size_t payloadLength);
};

#endif // !__SWITCHOBT_TOUCH_SENSOR_CALLBACKS_H__