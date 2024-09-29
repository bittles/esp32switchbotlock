# ESP32 (M5Stick Atom S3 Light) BLEでSwitchBotスマートロックを制御

## 本Repositoryについて

ESP32のBLE（Bluetooth Low Energy）プロトコルで[SwitchBot スマートロック](https://amzn.to/47K3dew)を制御するプログラムです。

PlatformIOで*ESP32*の*Arduino*を使用しています。

デフォルトでは、スマートロックの開閉をBLE経由で行うためのコードです。
`src/Switchbots`以下に、スマートロック以外のSwitchBot製品のコードもありますので、少し改造すれば他の機器（例えば、[スマート電球](https://amzn.to/4eIQ47W)や、[テープライト](https://amzn.to/4enQf8K)など）もBLEで操作できます。

M5Stick Atom S3 Lightのボタンを押すと**Lock**（施錠）、ボタンの長押しで**Unlock**（解錠）します。

詳しくは、筆者のブログ記事「[テック大家さん](https://tech-landlord.com)」もご覧下さい。

## SwitchBot BLE API (プロトコル)について

SwitchBot製品のBLE APIについては、以下のサイトに文書化された情報があります。

* [SwitchBotAPI-BLE (Github)](https://github.com/OpenWonderLabs/SwitchBotAPI-BLE)

ただし、上記のAPI仕様ではSwitchBotのいくつかの製品のコマンドは書かれているものの、SwitchBotスマートロックについては、記述がありません。

一方、以下のNodeJSを使うTypeScriptで書かれたライブラリの実装としてスマートロックのBLE操作が実装されています。

* [node-switchbot (Github)](https://github.com/OpenWonderLabs/node-switchbot)


そこで、本Repositoryのコードは、[node-switchbot (Github)](https://github.com/OpenWonderLabs/node-switchbot)のTypeScriptのコードから必要な部分を*ESP32 Arduino*用に移植したものになっています。


## ペイロードの暗号化

SwitchBotスマートロックをBLEで操作するためには、ペイロードの暗号化が必要です。

`src/DeviceAddr_example.h`に暗号化に必要な`KeyId`と`EncryptionKey`を適切に設定する必要があります。（使用する場合は、`src/DeviceAddr.h`に名前変更が必要）

この値は、以下のプログラムで取得します。

使用には、SwitchBotのクラウドのログイン情報が必要になります。

* [switchbot-get-encryption-key (Github)](
https://github.com/shizuka-na-kazushi/switchbot-get-encryption-key)

（または、[Danielhiversen/pySwitchbot](https://github.com/Danielhiversen/pySwitchbot/tree/master?tab=readme-ov-file#obtaining-locks-encryption-key)も利用可能）


`KeyId`と`EncryptionKey`に関しては、[node-switchbotライブラリのsetKey()の説明](https://github.com/OpenWonderLabs/node-switchbot?tab=readme-ov-file#setkey-method)に情報が少し書かれています。

暗号は、AES 128bit CTRモードを使用します。

## 依存関係

以下のライブラリに依存しています。

* m5stack/M5Unified@^0.1.16
* fastled/FastLED@^3.7.0
* m5stack/M5AtomS3@^1.0.0
* h2zero/NimBLE-Arduino@^1.4.0
* janelia-arduino/Vector@^1.2.2

（BLEは軽量の[NimBLE](https://github.com/h2zero/NimBLE-Arduino)を使用）

