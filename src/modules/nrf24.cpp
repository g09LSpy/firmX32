// nrf24.cpp
#include "nrf24.h"
#include <SPI.h>
#include <RF24.h>

// Voorbeeld GPIO-pins (aanpassen naar je eigen schema)
#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);

void initNRF24() {
  if (!radio.begin()) {
    Serial.println("[NRF24] Init mislukt. Check verbinding.");
    return;
  }
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(100); // Bijvoorbeeld kanaal 100
  Serial.println("[NRF24] Init geslaagd.");
}

void nrf24SpectrumScan() {
  Serial.println("[NRF24] Spectrum scan gestart...");
  radio.setAutoAck(false);
  radio.stopListening();

  for (int i = 0; i <= 125; i++) {
    radio.setChannel(i);
    delayMicroseconds(130);
    if (radio.testCarrier()) {
      Serial.printf("Kanaal %d: ACTIEF\n", i);
    } else {
      Serial.printf("Kanaal %d: stil\n", i);
    }
  }
  Serial.println("[NRF24] Scan voltooid.");
}

void nrf24Jammer() {
  Serial.println("[NRF24] Jamming gestart...");
  radio.stopListening();
  byte payload[32] = {0xFF};
  radio.openWritingPipe(0xF0F0F0F0E1LL);

  for (int i = 0; i < 1000; i++) {
    radio.write(&payload, sizeof(payload));
    delayMicroseconds(200);
  }

  Serial.println("[NRF24] Jamming klaar.");
}
