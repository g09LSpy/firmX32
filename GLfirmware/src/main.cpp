#include <Arduino.h>
#include <TFT_eSPI.h>

// Modules
#include "modules/ui.h"
#include "modules/wifi.h"
#include "modules/ble.h"
#include "modules/nrf24.h"
#include "modules/cc1101.h"

TFT_eSPI tft = TFT_eSPI();  // TFT driver object

void setup() {
  Serial.begin(115200);
  
  // Init display
  tft.init();
  tft.setRotation(1);  // Pas aan als scherm oriëntatie anders is
  tft.fillScreen(TFT_BLACK);

  // Init modules
  initWiFi();
  initBLE();
  initNRF24();
  initCC1101();

  // Init interface
  initUI(tft);
}

void loop() {
  handleUI();  // UI blijft draaien (menu's en joystick invoer)
}

