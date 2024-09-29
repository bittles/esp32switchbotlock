
#ifndef __SENSOR_ACTIONS_H__
#define __SENSOR_ACTIONS_H__

#include "Switchbots/SwitchbotMotionSensor.h"
#include "Switchbots/SwitchbotContactSensor.h"

class MotionSensorAction : public SwitchbotMotionSensorBroadcastHandler
{
protected:
  boolean lightEnabled;

public:
  MotionSensorAction();
  virtual void onBroadcastMessage(MotionSensorBroadcastMessage *pMessage);
};

class ContactSensorAction : public SwitchbotContactSensorBroadcastHandler
{
protected:
  boolean lightEnabled;
  
public:
  ContactSensorAction();
  virtual void onBroadcastMessage(ContactSensorBroadcastMessage *pMessage);
};

#endif // !__SENSOR_ACTIONS_H__