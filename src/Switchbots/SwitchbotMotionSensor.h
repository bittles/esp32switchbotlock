#ifndef __SWITCHBOT_MOTION_SENSOR_H__
#define __SWITCHBOT_MOTION_SENSOR_H__

#include "SwitchbotBaseDevice.h"

#pragma pack(1)
typedef struct
{
  /* Byte0 */
  uint8_t deviceType : 7;
  uint8_t encrypt : 1;
  /* Byte1 */
  uint8_t reserve1 : 6;
  uint8_t pirState : 1; /* PIR State 0: No one moves 1: Someone is moving */
  uint8_t scopeTested : 1;
  /* Byte2 */
  uint8_t battery : 7;
  uint8_t reserve2 : 1;
  /* Byte3, 4 */
  uint16_t pirTime; // need to swap bytes
  /* Byte5 */
  uint8_t lightIntensity : 2;  /* Light intensity 00:Rserve 01:dark 10:bright 11:Reserve */
  uint8_t sensingDistance : 2; /* Sensing distance 00:Long 01:Middle 10:Short 11:Reserve */
  uint8_t iotState : 1;        /* IOT state 0:disable 1:enable */
  uint8_t ledState : 1;        /* LED state 0:disable 1:enable */
  uint8_t reserve3 : 1;
  uint8_t hiBitOfPirTime : 1; /* Since the last trigger PIR time（s）Highest bit(*65536) */
} MotionSensorBroadcastMessage;

class SwitchbotMotionSensorBroadcastHandler
{
public:
  virtual void onBroadcastMessage(MotionSensorBroadcastMessage *pMessage) = 0;
};

class SwitchbotMotionSensor : public SwitchbotBaseDevice
{
public:
  SwitchbotMotionSensor(NimBLEAddress addr) : SwitchbotBaseDevice(addr,                                                // device MAC addr
                                                               std::string("cba20d00-224d-11e6-9fb8-0002a5d5c51b"), // service uuid
                                                               std::string("cba20002-224d-11e6-9fb8-0002a5d5c51b")) // characteristics uuid
  {
    m_uuidNotifyCharacteristics = std::string("cba20003-224d-11e6-9fb8-0002a5d5c51b");
    m_pHandler = nullptr;
  }

  virtual ~SwitchbotMotionSensor(){};

protected:
  SwitchbotMotionSensorBroadcastHandler *m_pHandler;

public:
  void addBroadcastHandler(SwitchbotMotionSensorBroadcastHandler *pHandler)
  {
    m_pHandler = pHandler;
  }

protected:
  virtual void onBroadcastScanRspMessage(uint8_t *pMessage, size_t length);
};

#endif // !__SWITCHBOT_MOTION_SENSOR_H__