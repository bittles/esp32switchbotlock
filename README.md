# Control SwitchBot smart lock with ESP32 (M5 Atom S3 Lite) BLE
Simple translation to english with Vivaldi's translate with American amazon links.
## About this repository
It is a program that controls [SwitchBot smart locks](https://www.amazon.com/SwitchBot-Electronic-Deadbolt-Existing-Vacation/dp/B09Z6WCL6P) using ESP32's BLE (Bluetooth Low Energy) protocol.

I am using an ESP32 Arduino on PlatformIO.

By default, this code allows you to open and close smart locks via BLE.  Under src/Switchbots, there are also codes for SwitchBot products other than smart locks, so if you modify them a little, you can operate other devices (for example, [smart light bulbs](https://us.switch-bot.com/products/switchbot-color-bulb?variant=43179775492329), tape lights, etc.) with BLE.

![動作中動画](https://tech-landlord.com/wp-content/uploads/2024/10/SwitchBot-M5-lock.gif)
Press the M5 Atom S3 Lite button to lock, and press and hold the button to unlock.

For more information, I have also written an explanatory article on my blog ''[Tech Landlord](https://tech-landlord.com)''. The protocol details are also organized in [the article](https://tech-landlord.com/articles/switchbot-smartlock-with-ble-api-explaining-protocol-esp32-m5-atoms3), so it should be helpful when using the code in this repository.

## SwitchBot BLE API (About protocol)

There is documented information about the BLE API for SwitchBot products on the following sites.

* [SwitchBotAPI-BLE (Github)](https://github.com/OpenWonderLabs/SwitchBotAPI-BLE)

However, although the above API specifications include commands for some SwitchBot products, there is no mention of SwitchBot smart locks.

On the other hand, smart lock BLE operations are implemented as an implementation of the following library written in TypeScript that uses NodeJS.

* [node-switchbot (Github)](https://github.com/OpenWonderLabs/node-switchbot)

Therefore, the code in this repository is a port of the necessary parts from the [node-switchbot (Github)](https://github.com/OpenWonderLabs/node-switchbot) TypeScript code for ESP32 Arduino.


## Payload encryption

Payload encryption is required to operate SwitchBot smart locks with BLE.

The KeyId and EncryptionKey required for encryption must be set appropriately in src/DeviceAddr_example.h (). If used, src/DeviceAddr.h must be renamed

This value is obtained by the following program.

To use it, you will need your SwitchBot cloud login information.

* [switchbot-get-encryption-key (Github)](
https://github.com/shizuka-na-kazushi/switchbot-get-encryption-key)

（Or [Danielhiversen/pySwitchbot](https://github.com/Danielhiversen/pySwitchbot/tree/master?tab=readme-ov-file#obtaining-locks-encryption-key) is also available)

Regarding KeyId and EncryptionKey, a little information is written in the [setKey() description of the node-switchbot](https://github.com/OpenWonderLabs/node-switchbot?tab=readme-ov-file#setkey-method) library.

The cipher uses AES 128bit CTR mode.

## Dependencies

It relies on the following libraries.

* m5stack/M5Unified@^0.1.16
* fastled/FastLED@^3.7.0
* m5stack/M5AtomS3@^1.0.0
* h2zero/NimBLE-Arduino@^1.4.0
* janelia-arduino/Vector@^1.2.2

（BLE uses lightweight NimBLE](https://github.com/h2zero/NimBLE-Arduino)を使用）

