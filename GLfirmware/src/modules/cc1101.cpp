// cc1101.cpp
#include "cc1101.h"
#include "ELECHOUSE_CC1101_SRC_DRV.h"

// Pas deze pins aan naar jouw hardwareconfiguratie
#define CC1101_CS    5
#define CC1101_GDO0  4

void initCC1101() {
  ELECHOUSE_cc1101.setSpiPin(CC1101_CS); // Chip Select pin
  ELECHOUSE_cc1101.setGDO(0, CC1101_GDO0); // GDO0 pin

  if (ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("[CC1101] Chip gedetecteerd.");
    ELECHOUSE_cc1101.Init();
    ELECHOUSE_cc1101.setMHZ(433.0); // of 868.0 / 315.0 / 915.0 afhankelijk van module
    ELECHOUSE_cc1101.setModulation(2); // 2-FSK
    ELECHOUSE_cc1101.setPA(12); // Vermogen (0-12)
    Serial.println("[CC1101] Init geslaagd op 433 MHz");
  } else {
    Serial.println("[CC1101] Fout: chip niet gevonden.");
  }
}

void cc1101SpectrumSniff() {
  Serial.println("[CC1101] Spectrum sniffen gestart...");
  for (float freq = 433.0; freq <= 435.0; freq += 0.1) {
    ELECHOUSE_cc1101.setMHZ(freq);
    delay(50);
    int rssi = ELECHOUSE_cc1101.getRssi();
    Serial.printf("Frequentie %.1f MHz - RSSI: %d dBm\n", freq, rssi);
  }
  Serial.println("[CC1101] Spectrum scan voltooid.");
}

void cc1101Jammer() {
  Serial.println("[CC1101] Jamming gestart...");
  byte noise[10] = {0xAA};
  for (int i = 0; i < 1000; i++) {
    ELECHOUSE_cc1101.SendData(noise, sizeof(noise));
    delayMicroseconds(200);
  }
  Serial.println("[CC1101] Jamming voltooid.");
}
