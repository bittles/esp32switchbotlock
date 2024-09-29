
#ifndef __SWITCHBOT_BASE_DEVICE_H__
#define __SWITCHBOT_BASE_DEVICE_H__

#include <Arduino.h>
#include <NimBLEDevice.h>

#define SWITCHBOT_ENABLE_DEBUG_LOG

#define SWITCHOBT_MAX_DEVICES (8)

class SwitchbotBaseDevice : public NimBLEClientCallbacks
{
public:
  boolean m_connected = false;
  boolean m_found = false;
  NimBLEClient *m_pClient = nullptr;

  NimBLEAddress m_address;
  NimBLEUUID m_uuidService;
  NimBLEUUID m_uuidCharacteristics;
  NimBLEUUID m_uuidNotifyCharacteristics;

  SwitchbotBaseDevice(NimBLEAddress addr, NimBLEUUID uuidService, NimBLEUUID uuidCharacteristics) : m_address(addr),
                                                                                           m_uuidService(uuidService),
                                                                                           m_uuidCharacteristics(uuidCharacteristics)
  {
    m_uuidNotifyCharacteristics = NimBLEUUID();
  }

  virtual ~SwitchbotBaseDevice(){};

  virtual void onConnect(NimBLEClient *pClient);
  virtual void onDisconnect(NimBLEClient *pClient);

  virtual void onReceiveNotification(NimBLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length){};
  virtual void onBroadcastScanRspMessage(uint8_t* pMessage, size_t length) {};
  virtual void onBroadcastAdvIndMessage(uint8_t* pMessage, size_t length) {};

  void foundByAdvertising() { m_found = true; }
  boolean isFound() { return m_found; }

protected:
  NimBLERemoteCharacteristic *getCharacteristics();
  boolean connect();
  boolean sendValue(uint8_t *data, size_t size, boolean response = true);
  boolean registerNotify();
};

#endif // !__SWITCHBOT_BASE_DEVICE_H__
