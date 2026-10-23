#include <esp_mac.h>

String esp_WiFi_mac();

void setup() {
  Serial.begin(115200);

  Serial.println(esp_WiFi_mac());
}

void loop() {
}

String esp_WiFi_mac() {
  uint8_t raw_wifi_mac_address[6] = {0}; // Holds raw 6-byte Wi-Fi MAC address

  // returns ESP_OK if successful, then stores ESP32's raw Wi-Fi MAC address at raw_wifi_mac_address 6-byte array
  if(esp_read_mac(raw_wifi_mac_address, ESP_MAC_WIFI_STA) == ESP_OK) {
    char formatted_wifi_mac_address[40] = {0}; // Stores formatted 6-byte Wi-Fi MAC address

    snprintf(formatted_wifi_mac_address, sizeof(formatted_wifi_mac_address),
      "ESP32 Wi-Fi MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
      raw_wifi_mac_address[0], raw_wifi_mac_address[1], raw_wifi_mac_address[2],
      raw_wifi_mac_address[3], raw_wifi_mac_address[4], raw_wifi_mac_address[5]
    );
    return String(formatted_wifi_mac_address);
  }

  return "Error";
}