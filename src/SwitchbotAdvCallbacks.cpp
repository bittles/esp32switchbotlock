#include "SwitchbotAdvCallbacks.h"

#include "DeviceAddr.h"

void SwitchbotAdvCallbacks::onResult(NimBLEAdvertisedDevice* advertisedDevice)
{
  // Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
  // Serial.print("device type = ");
  // Serial.println(advertisedDevice.getAddressType());

  NimBLEAddress addr = advertisedDevice->getAddress();
  if (addr.equals(motionSensorAddr))
  {
    // Serial.print("found device: ");
    // Serial.println(addr.toString().c_str());
    handlePayload(advertisedDevice->getPayload(), advertisedDevice->getPayloadLength());
  }

  for (int i = 0; i < numOfDevice; i++)
  {
    if (addr.equals(devices[i]->m_address))
    {
      callDevice(devices[i], advertisedDevice->getPayload(), advertisedDevice->getPayloadLength());
    }
  }
}

typedef struct
{
  uint8_t length;
  uint8_t type;
  uint8_t payload[];
} BlePacketAdvStruct;

void SwitchbotAdvCallbacks::callDevice(SwitchbotBaseDevice *device, uint8_t *pPayload, size_t payloadLength)
{
  if (!device)
  {
    return;
  }

  BlePacketAdvStruct *packetAdvStruct = (BlePacketAdvStruct *)pPayload;
  while (0 < payloadLength)
  {
    size_t len = packetAdvStruct->length;

    switch(packetAdvStruct->type) {
      case 0x01:
        /** nothing todo */
        break;
      case 0xff:
        /** ADV_IND */
        device->onBroadcastAdvIndMessage(packetAdvStruct->payload, len);
        break;
      case 0x16:
        /** Scan_RSP */
        device->onBroadcastScanRspMessage(packetAdvStruct->payload, len);
        break;
      default:
        /** unknown type*/
        break;
    }

    pPayload += (len + 1);
    payloadLength -= (len + 1);
    packetAdvStruct = (BlePacketAdvStruct *)pPayload;
  }
}

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

void SwitchbotAdvCallbacks::handlePayload(uint8_t *pPayload, size_t payloadLength)
{

  // printPayload(pPayload, payloadLength);

#if 0
  /*****
   *
   */
  if (payloadLength < 33)
    return;

  if (pPayload[24] == 0x64)
  { // device type is 'd'=0x64 for Touch sensor
    TouchSensorPacket *packet = (TouchSensorPacket *)(pPayload + 24);
    Serial.print(">>> open state: ");
    Serial.print(packet->openState);
    Serial.print(", moving state: ");
    Serial.print(packet->pirState);
    Serial.print(", device type: ");
    Serial.print(String(packet->deviceType, 16).c_str());
    Serial.print(", blight: ");
    Serial.print(packet->blight);
    Serial.print(", packet[3]: ");
    Serial.print(String(*(pPayload + 24 + 3), 16).c_str());
    Serial.println("");
  }
#endif
}