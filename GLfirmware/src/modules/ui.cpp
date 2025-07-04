// ui.cpp
#include "ui.h"
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "wifi.h"
#include "ble.h"
#include "nrf24.h"
#include "cc1101.h"

// Joystick pinconfiguratie
#define JOY_UP     36
#define JOY_DOWN   39
#define JOY_LEFT   34
#define JOY_RIGHT  35
#define JOY_CLICK  32
#define JOY_SET    33
#define JOY_RST    25

TFT_eSPI* tft_ptr;

enum MenuState {
  MAIN_MENU,
  WIFI_MENU,
  BLE_MENU,
  NRF_MENU,
  CC_MENU
};

MenuState currentMenu = MAIN_MENU;
int currentOption = 0;

const char* mainOptions[] = {
  "WiFi Tools",
  "BLE Tools",
  "NRF24 Tools",
  "CC1101 Tools"
};
const int numMainOptions = sizeof(mainOptions) / sizeof(mainOptions[0]);

const char* wifiOptions[] = {
  "WiFi Scan",
  "WiFi Deauth",
  "WiFi Beacons"
};
const int numWiFiOptions = sizeof(wifiOptions) / sizeof(wifiOptions[0]);

const char* bleOptions[] = {
  "BLE Spam"
};
const int numBLEOptions = sizeof(bleOptions) / sizeof(bleOptions[0]);

const char* nrfOptions[] = {
  "NRF24 Scan",
  "NRF24 Jammer"
};
const int numNRFOptions = sizeof(nrfOptions) / sizeof(nrfOptions[0]);

const char* ccOptions[] = {
  "CC1101 Sniff",
  "CC1101 Jammer"
};
const int numCCOptions = sizeof(ccOptions) / sizeof(ccOptions[0]);

void initUI(TFT_eSPI& tft) {
  tft_ptr = &tft;
  pinMode(JOY_UP, INPUT_PULLUP);
  pinMode(JOY_DOWN, INPUT_PULLUP);
  pinMode(JOY_CLICK, INPUT_PULLUP);
  pinMode(JOY_LEFT, INPUT_PULLUP);
  pinMode(JOY_RIGHT, INPUT_PULLUP);
  pinMode(JOY_SET, INPUT_PULLUP);
  pinMode(JOY_RST, INPUT_PULLUP);
  drawMenu();
}

void drawMenu() {
  tft_ptr->fillScreen(TFT_BLACK);
  tft_ptr->setTextSize(2);
  for (int i = 0; i < getCurrentOptionCount(); ++i) {
    if (i == currentOption) {
      tft_ptr->setTextColor(TFT_GREEN);
    } else {
      tft_ptr->setTextColor(TFT_WHITE);
    }
    tft_ptr->drawString(getCurrentOptionName(i), 10, 30 + i * 25);
  }
}

const char* getCurrentOptionName(int index) {
  switch (currentMenu) {
    case MAIN_MENU: return mainOptions[index];
    case WIFI_MENU: return wifiOptions[index];
    case BLE_MENU: return bleOptions[index];
    case NRF_MENU: return nrfOptions[index];
    case CC_MENU:  return ccOptions[index];
  }
  return "";
}

int getCurrentOptionCount() {
  switch (currentMenu) {
    case MAIN_MENU: return numMainOptions;
    case WIFI_MENU: return numWiFiOptions;
    case BLE_MENU: return numBLEOptions;
    case NRF_MENU: return numNRFOptions;
    case CC_MENU:  return numCCOptions;
  }
  return 0;
}

void handleUI() {
  static unsigned long lastDebounce = 0;
  if (millis() - lastDebounce < 200) return;

  if (digitalRead(JOY_UP) == LOW) {
    currentOption = (currentOption - 1 + getCurrentOptionCount()) % getCurrentOptionCount();
    drawMenu();
    lastDebounce = millis();
  } else if (digitalRead(JOY_DOWN) == LOW) {
    currentOption = (currentOption + 1) % getCurrentOptionCount();
    drawMenu();
    lastDebounce = millis();
  } else if (digitalRead(JOY_LEFT) == LOW || digitalRead(JOY_SET) == LOW) {
    currentMenu = MAIN_MENU;
    currentOption = 0;
    drawMenu();
    lastDebounce = millis();
  } else if (digitalRead(JOY_CLICK) == LOW) {
    executeOption(currentMenu, currentOption);
    lastDebounce = millis();
  }
}

void executeOption(MenuState menu, int option) {
  switch (menu) {
    case MAIN_MENU:
      switch (option) {
        case 0: currentMenu = WIFI_MENU; break;
        case 1: currentMenu = BLE_MENU; break;
        case 2: currentMenu = NRF_MENU; break;
        case 3: currentMenu = CC_MENU;  break;
      }
      currentOption = 0;
      drawMenu();
      return;
    case WIFI_MENU:
      if (option == 0) wifiScan();
      else if (option == 1) wifiDeauth();
      else if (option == 2) wifiBeaconSpam();
      break;
    case BLE_MENU:
      if (option == 0) bleSpam();
      break;
    case NRF_MENU:
      if (option == 0) nrf24SpectrumScan();
      else if (option == 1) nrf24Jammer();
      break;
    case CC_MENU:
      if (option == 0) cc1101SpectrumSniff();
      else if (option == 1) cc1101Jammer();
      break;
  }
  delay(1000);
  drawMenu();
}
