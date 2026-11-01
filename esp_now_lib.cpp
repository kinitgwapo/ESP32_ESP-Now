#include "esp_now_lib.h"

#include <Arduino.h>

#include <ESP_NOW.h>

#include <esp_mac.h>

static String esp_WiFi_mac();

void esp_WiFi_Setup()
{
  // Sets PC to ESP32 UART Communication
  Serial.begin(115200);

  // Prints out Formatted 6-byte Station & Access-Point Wi-Fi MAC addresses
  Serial.println(esp_WiFi_mac());
}

static String esp_WiFi_mac()
{
  uint8_t raw_sta_mac[6] = {0}; // Holds raw 6-byte Station Wi-Fi MAC address
  uint8_t raw_ap_mac[6] = {0}; // Holds raw 6-byte Acces-Point Wi-Fi MAC address
  char formatted_wifi_mac_address[97] = {0}; // Stores formatted 6-byte Station & Access-Point Wi-Fi MAC addresses

  // returns ESP_OK if successful, then stores ESP32's raw Wi-Fi MAC addresses at their respective 6-byte array
  if(esp_read_mac(raw_sta_mac, ESP_MAC_WIFI_STA) == ESP_OK && esp_read_mac(raw_ap_mac, ESP_MAC_WIFI_SOFTAP) == ESP_OK)
  {
    // Formats the 6-byte Station & Access-Point Wi-Fi MAC address into a string
    Serial.println(
      snprintf(
        formatted_wifi_mac_address, sizeof(formatted_wifi_mac_address),
        "ESP32 Wi-Fi Station MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n"
        "ESP32 Wi-Fi AP MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
        raw_sta_mac[0], raw_sta_mac[1], raw_sta_mac[2], raw_sta_mac[3], raw_sta_mac[4], raw_sta_mac[5],
        raw_ap_mac[0], raw_ap_mac[1], raw_ap_mac[2], raw_ap_mac[3], raw_ap_mac[4], raw_ap_mac[5]
      )
    );

    return String(formatted_wifi_mac_address);
  }

  return "Failed to retrieve ESP32 Wi-Fi MAC address"; // Returns an error if esp_read_mac was unsuccessful
}