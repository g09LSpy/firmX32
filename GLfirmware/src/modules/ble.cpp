// ble.cpp
#include "ble.h"
#include <NimBLEDevice.h>

void initBLE() {
  NimBLEDevice::init("");  // Lege naam, we adverteren zelf spoofed data
  NimBLEDevice::setPower(ESP_PWR_LVL_P9); // Max vermogen
}

void bleSpam() {
  static const char* bleNames[] = {
    "AirPods Max",
    "Galaxy Buds",
    "iPhone 15",
    "Pixel 9 Pro",
    "PlayStation 5",
    "BLE_Pentest",
    "Free_Speakers",
    "Smartwatch",
    "Nintendo Switch",
    "SmartTV"
    "Big D Energy"
  };

  Serial.println("[BLE] Starting BLE Advertising Spoof...");

  for (int i = 0; i < sizeof(bleNames) / sizeof(bleNames[0]); ++i) {
    NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();

    // Stop vorige advertentie
    pAdvertising->stop();
    delay(100);

    // Stel spoofed device naam in
    NimBLEDevice::setDeviceName(bleNames[i]);
    NimBLEAdvertisementData adData;
    adData.setName(bleNames[i]);
    adData.setFlags(0x06); // LE General Discoverable Mode + BR/EDR not supported
    
    pAdvertising->setAdvertisementData(adData);
    pAdvertising->start();

    Serial.printf("[BLE] Advertising as: %s\n", bleNames[i]);
    delay(500);
  }

  Serial.println("[BLE] Finished BLE name rotation.");
}
