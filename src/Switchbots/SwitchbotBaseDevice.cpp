
#include <Vector.h>
#include "SwitchbotBaseDevice.h"

/** notify handler management **/
struct NotifyCallbackEntry
{
  NimBLEClient *m_pClient;
  SwitchbotBaseDevice *m_pDevice;
};

Vector<NotifyCallbackEntry> g_notifyCallbackTable;
NotifyCallbackEntry g_notifyCallbackTableStorage[SWITCHOBT_MAX_DEVICES];



void SwitchbotBaseDevice::onConnect(NimBLEClient *pClient)
{
  Serial.println("MyBLEClientCallbacks::onConnect");
  m_connected = true;
}

void SwitchbotBaseDevice::onDisconnect(NimBLEClient *pClient)
{
  Serial.println("MyBLEClientCallbacks::onDisconnect");
  m_connected = false;
}

BLERemoteCharacteristic *SwitchbotBaseDevice::getCharacteristics()
{
  if (!connect())
  {
    return nullptr;
  }

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = m_pClient->getService(m_uuidService);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(m_uuidService.toString().c_str());
    return nullptr;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic *pRemoteCharacteristic = pRemoteService->getCharacteristic(m_uuidCharacteristics);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(m_uuidCharacteristics.toString().c_str());
    return nullptr;
  }

  return pRemoteCharacteristic;
}

boolean SwitchbotBaseDevice::connect()
{
  if (!m_found) {
    return false;
  }
  
  if (!m_pClient)
  {
    Serial.println("going to create client!");
    m_pClient = BLEDevice::createClient();
    if (!m_pClient)
    {
      Serial.println(" - Created client failed");
      return false;
    }
    m_pClient->setClientCallbacks(this);
  }

  if (!m_connected)
  {
    Serial.print("connected... to ");
    Serial.println(m_address.toString().c_str());
    if (!m_pClient->connect(m_address))
    {
      Serial.println(" m_pClient->connect() failed!");
      m_connected = false;
      return false;
    }
    m_connected = true;
    Serial.println(" m_pClient->connect() connected...");
    registerNotify();
  }
  return true;
}

boolean SwitchbotBaseDevice::sendValue(uint8_t *data, size_t size, boolean response)
{
  BLERemoteCharacteristic *pRemoteCharacteristic = getCharacteristics();
  if (!pRemoteCharacteristic)
  {
    Serial.println("cannot get remote characteristics object...");
    return false;
  }

  Serial.println("- writing value... ");
  if (!pRemoteCharacteristic->writeValue(data, size, response))
  {
    Serial.println("cannot write value... going to reset client!");
    m_connected = false;
    return false;
  }
  Serial.println("... writing value done.");
  return true;
}

NotifyCallbackEntry* _findNotifyEntry(NimBLEClient* pClient) {
  NotifyCallbackEntry* exists = nullptr;
  for (int i = 0; i < g_notifyCallbackTable.size(); i++)
  {
    if (g_notifyCallbackTable[i].m_pClient == pClient)
    {
      exists = &(g_notifyCallbackTable[i]);
      break;
    }
  }
  return exists;
}

void _registerClientToNotifyTable(NimBLEClient *pClient, SwitchbotBaseDevice *pDevice)
{
  if (!g_notifyCallbackTable.data())
  {
    g_notifyCallbackTable.setStorage(g_notifyCallbackTableStorage);
  }

  // check if pClient is registered already
  NotifyCallbackEntry* exists = _findNotifyEntry(pClient);

  if (!exists)
  {
    NotifyCallbackEntry entry;
    entry.m_pClient = pClient;
    entry.m_pDevice = pDevice;
    g_notifyCallbackTable.push_back(entry);
  }
}

void _switchbotNotifyCallbackInternal(BLERemoteCharacteristic *pBLERemoteCharacteristic, uint8_t *pData, size_t length, bool isNotify)
{
  Serial.print(") RESP: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print(pData[i], HEX);
    Serial.print(",");
  }
  Serial.println("");

  NotifyCallbackEntry* entry = _findNotifyEntry(pBLERemoteCharacteristic->getRemoteService()->getClient());
  if (entry) {
    entry->m_pDevice->onReceiveNotification(pBLERemoteCharacteristic, pData, length);
  } else {
    Serial.println("err: callback entry does not exist.");
  }
}

boolean SwitchbotBaseDevice::registerNotify()
{
  if (m_uuidNotifyCharacteristics.bitSize() <= 0) { // uuid is not set by sub class.
    return false;
  }

  Serial.print("registerNotify for ");
  Serial.println(m_uuidNotifyCharacteristics.toString().c_str());
  if (!connect())
  {
    return false;
  }

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = m_pClient->getService(m_uuidService);
  if (pRemoteService == nullptr)
  {
    Serial.print("Failed to find our service UUID for Notify: ");
    Serial.println(m_uuidService.toString().c_str());
    return false;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic *pRemoteCharacteristic = pRemoteService->getCharacteristic(m_uuidNotifyCharacteristics);
  if (pRemoteCharacteristic == nullptr)
  {
    Serial.print("Failed to find our characteristic UUID for Notify: ");
    Serial.println(m_uuidCharacteristics.toString().c_str());
    return false;
  }

  _registerClientToNotifyTable(m_pClient, this);
  pRemoteCharacteristic->subscribe(true, _switchbotNotifyCallbackInternal);
  return true;
}