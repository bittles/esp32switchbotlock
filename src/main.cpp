#include <M5AtomS3.h>
#include <NimBLEDevice.h>
#include "SwitchbotApp.h"

void setup()
{
  Serial.begin(115200); // シリアル通信の開始
  // while (!Serial)
  // {
  //   ;
  // } // シリアル通信の準備ができるまで待つ
  Serial.flush();
  delay(1200);
  Serial.println("M5AtomS3 initializing...OK");

  AtomS3.begin(true); // Init M5AtomS3Lite.
  AtomS3.dis.setBrightness(10);
  Serial.println("Setup complete");

  // App
  SwitchbotApp.setup();
}

boolean longPress = false;
void handleButton()
{
  /** button control **/
  if (AtomS3.BtnA.wasReleased())
  {
    if (!longPress)
    {
      SwitchbotApp.sendMessage(SBA_MESSAGE_PRESS_BUTTON);
    }
    longPress = false;
  }

  if (AtomS3.BtnA.pressedFor(1500))
  {
    if (!longPress)
    {
      SwitchbotApp.sendMessage(SBA_MESSAGE_LONG_PRESS_BUTTON);
    }
    longPress = true;
  }
}

void handleAppMessage()
{
  /** handling message from SwitchbotApp */
  SBA_MESSAGE_ID messageId;
  if (SwitchbotApp.receiveMessage(&messageId))
  {
    Serial.print("pattern received: ");
    Serial.println(messageId);
    uint32_t illumLedPattern = messageId - SBA_MESSAGE_LED_PATTERN_BASE;
    // colorLedAnim->setPattern(illumLedPattern);
  }
}

void loop()
{
  AtomS3.update();

  handleButton();

  handleAppMessage();
}
