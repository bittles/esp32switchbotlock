#ifndef __SWITCHBOT_CONTACT_SENSOR_H__
#define __SWITCHBOT_CONTACT_SENSOR_H__

#include "SwitchbotBaseDevice.h"

#define SWITCHBOT_CONTACT_SENSOR_COMMAND_GET_STATUS {0x57, 0x00, 0x11};

#define SWITCHBOT_CONTACT_SENSOR_WRITE_OFFSET_EX_COMMAND (4)
#define SWITCHBOT_CONTACT_SENSOR_WRITE_OFFSET_LVL (5)
#define SWITCHBOT_CONTACT_SENSOR_WRITE_OFFSET_R (6)
#define SWITCHBOT_CONTACT_SENSOR_WRITE_OFFSET_G (7)
#define SWITCHBOT_CONTACT_SENSOR_WRITE_OFFSET_B (8)


#pragma pack(1)
typedef struct
{
  /* Byte0 */
  uint8_t deviceType : 7;
  uint8_t encrypt : 1;
  /* Byte1 */
  uint8_t reserve1 : 6;
  uint8_t pirState : 1;
  uint8_t scopeTested : 1;
  /* Byte2 */
  uint8_t battery : 7;
  uint8_t reserve2 : 1;
  /* Byte3 */
  uint8_t blight : 1;    /* 0: dark, 1: blight */
  uint8_t openState : 2; /* 0: door close, 1: door open, 2: timeout not close */
  uint8_t reserve3 : 3;
  uint8_t hiBitOfHalTime : 1;
  uint8_t hiBitOfPirTime : 1;
  /* Byte4, 5 */
  uint16_t ripTime;
  /* Byte6, 7 */
  uint16_t halTime;
  /* Byte8 */
  uint8_t countOfButton : 4;
  uint8_t countOfGoOut : 2;
  uint8_t countOfEntry : 2;
} ContactSensorBroadcastMessage;

class SwitchbotContactSensorBroadcastHandler {
public:
  virtual void onBroadcastMessage(ContactSensorBroadcastMessage* pMessage) = 0;
};

class SwitchbotContactSensor : public SwitchbotBaseDevice
{
public:
  SwitchbotContactSensor(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                             std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                             std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
    m_uuidNotifyCharacteristics = std::string("cba20003-224d-11e6-9fb8-0002a5d5c51b");
    m_pHandler = nullptr;
  }

  virtual ~SwitchbotContactSensor(){};

  SwitchbotContactSensorBroadcastHandler* m_pHandler;

public:
  bool getStatus();
  void addBroadcastHandler(SwitchbotContactSensorBroadcastHandler* pHandler) {
    m_pHandler = pHandler;
  } 

protected:
  virtual void onReceiveNotification(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length);
  virtual void onBroadcastScanRspMessage(uint8_t* pMessage, size_t length);
};

#endif // !__SWITCHBOT_CONTACT_SENSOR_H__