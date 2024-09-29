
#include "SwitchbotApp.h"
#include "SwitchbotAdvCallbacks.h"

// Global app object
CSwitchbotApp SwitchbotApp;

typedef struct
{
  SBA_MESSAGE_ID id;
  char data[20];
} MessageData;

void CSwitchbotApp_Task(void *pvParams)
{
  CSwitchbotApp &app = *((CSwitchbotApp *)pvParams);
  app.task();
}

void CSwitchbotApp::task()
{
  /** initial advertised device scan */
  doScan();

  /** scan blocked context so long time, let's reset queue so that received message during scanning will be discorded */
  xQueueReset(queueOnApp);

  /** main loop starts */
  MessageData receivedData;
  while (1)
  {
    if (xQueuePeek(queueOnApp, &receivedData, 0) == pdPASS)
    {
      // メッセージを実際に取り出す
      xQueueReceive(queueOnApp, &receivedData, 0);
      switch (receivedData.id)
      {
      case SBA_MESSAGE_PRESS_BUTTON:
        onPressButton();
        break;
      case SBA_MESSAGE_LONG_PRESS_BUTTON:
        onLongPressButton();
        break;
      default:
        break;
      }
    }

    loop();

    delay(10);
  }
}

void CSwitchbotApp::sendMessage(SBA_MESSAGE_ID messageId)
{
  MessageData msg;
  msg.id = messageId;
  msg.data[0] = 'A';
  msg.data[1] = 0x0;
  xQueueSendToBack(queueOnApp, &msg, portTICK_PERIOD_MS);
}

void CSwitchbotApp::doScan()
{
#if 1
  onScanStart();

  BLEScan *pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new SwitchbotAdvertisedDeviceCallbacks(devices, numOfDevice));
  pBLEScan->setActiveScan(false);                    // active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(15); // scanTime = 15 sec

  onScanEnded();

#else
  BLEScan *pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new SwitchbotAdvCallbacks(devices, numOfDevice), true);
  pBLEScan->setActiveScan(false);                    // active scan uses more power, but get results faster
  BLEScanResults foundDevices = pBLEScan->start(0); // Infinite scan time
#endif 
}

void CSwitchbotApp::onScanStart()
{
  MessageData msg;
  msg.id = SBA_MESSAGE_LED_PATTERN_BASE + ILLUM_LED_PATTERN_PROGRESS;
  msg.data[0] = 0x0;
  xQueueSendToBack(queueOnMain, &msg, portTICK_PERIOD_MS);
}

void CSwitchbotApp::onScanEnded()
{
  Serial.println("BLE scan done...");

  MessageData msg;

  msg.id = SBA_MESSAGE_LED_PATTERN_BASE + ILLUM_LED_PATTERN_NONE;
  msg.data[0] = 'A';
  msg.data[1] = 0x0;
  xQueueSendToBack(queueOnMain, &msg, portTICK_PERIOD_MS);
}

void CSwitchbotApp::setup()
{
  BLEDevice::init("");

  createDevices();

  queueOnMain = xQueueCreate(16, sizeof(MessageData));
  queueOnApp = xQueueCreate(64, sizeof(MessageData));
  xTaskCreateUniversal(CSwitchbotApp_Task, "CSwitchbotApp_Task", 8192, (void *const)this, 1, NULL, APP_CPU_NUM);
}

void CSwitchbotApp::loop()
{
  /** so far, nothing to do here **/
}

void CSwitchbotApp::setIllumLedPattern(uint32_t illumLedPatter)
{
  MessageData msg;
  msg.id = SBA_MESSAGE_LED_PATTERN_BASE + illumLedPatter;
  msg.data[0] = 0x0;
  xQueueSendToBack(queueOnMain, &msg, portTICK_PERIOD_MS);
}

boolean CSwitchbotApp::receiveMessage(SBA_MESSAGE_ID *pMessageId)
{
  if (!pMessageId)
    return false;

  if (!queueOnMain) {
    return false;
  }

  MessageData receivedData;
  if (xQueuePeek(queueOnMain, &receivedData, 0) == pdPASS)
  {
    // 受信したメッセージを処理
    Serial.print("Received message: ID=");
    Serial.print(receivedData.id);
    Serial.print(", Data=");
    Serial.println(receivedData.data);

    // メッセージを実際に取り出す
    xQueueReceive(queueOnMain, &receivedData, 0);
    *pMessageId = receivedData.id;
    return true;
  }
  return false;
}

void SwitchbotAdvertisedDeviceCallbacks::onResult(NimBLEAdvertisedDevice* advertisedDevice)
{
  Serial.printf("Advertised Device: %s \n", advertisedDevice->toString().c_str());
  Serial.print("device type = ");
  Serial.println(advertisedDevice->getAddressType());

  NimBLEAddress addr = advertisedDevice->getAddress();
  for (int i = 0; i < numOfDevice; i++)
  {
    if (addr.equals(devices[i]->m_address))
    {
      Serial.print("found device: ");
      Serial.println(addr.toString().c_str());
      devices[i]->foundByAdvertising();
      if (isAllDevicesFound())
      {
        advertisedDevice->getScan()->stop();
        break;
      }
    }
  }
}

boolean SwitchbotAdvertisedDeviceCallbacks::isAllDevicesFound()
{
  for (int i = 0; i < numOfDevice; i++)
  {
    if (!devices[i]->isFound())
    {
      return false;
    }
  }
  return true;
}