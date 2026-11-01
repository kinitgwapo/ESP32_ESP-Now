#include "esp_now_lib.h"

#include <Arduino.h>

#include <WiFi.h>
#include <ESP32_NOW.h>

/*
** @brief Error Macro Codes
**
** @Note  Used by esp_WiFi_interface(), esp_NOW_init() for reconfiguration, re-initialization.
*/
#define SUCCESS 1
#define FAIL 0

static void esp_WiFi_interface();
static void esp_WiFi_channel();
static void esp_NOW_init();
static String esp_WiFi_mac();
static void esp_NOW_receive();

/*
** @brief ESP-NOW receiver callbacks
**        Defined from esp_now_receiver.cpp for ESP-NOW
*/
void esp_NOW_receiver_init();

/*
** @brief ESP-NOW broadcaster callbacks
**        Defined from esp_now_broadcaster.cpp for ESP-NOW
*/
void esp_NOW_broadcaster_init();

void esp_WiFi_Setup()
{
  Serial.begin(115200); // Sets PC to ESP32 UART Communication
  
  esp_WiFi_interface(); // Set WiFi Interface to Acess-Point & Station

  esp_WiFi_channel(); // Check current WiFi radio channel & might configure WiFi radio channel

  esp_NOW_init(); // Initialize ESP-NOW
  
  Serial.println(esp_WiFi_mac()); // Prints out Formatted 6-byte Station & Access-Point Wi-Fi MAC addresses

  esp_NOW_receiver_init(); // Initialize receiver mac queue, receiver callback

  esp_NOW_broadcast_init();
}





static void esp_WiFi_interface()
{
  bool err = WiFi.mode(WIFI_AP_STA);
  while(err == FAIL) // Runs indefinitely until the WiFi Interface mode configuration becomes successful
  {
    Serial.println("Failed to configure WiFi Interface, retrying...");
    err = WiFi.mode(WIFI_AP_STA);
    delay(100);
  }
  if(err == SUCCESS)
  {
    Serial.println("Successfully configured WiFi Interface; Using AP & STA mode");
  }
}

static void esp_WiFi_channel()
{
  Serial.println("Current WiFi Radio Channel: " + String(WiFi.channel()));
}

static void esp_NOW_init()
{
  bool err = ESP_NOW.begin();
  while(err == FAIL) // Runs indefinitely until the ESP-NOW initialization becomes successful
  {
    Serial.println("Failed to initialize ESP-NOW, retrying...");
    err = ESP_NOW.begin();
    delay(100);
  }
  if(err == SUCCESS)
  {
    Serial.println("Successfully initialized ESP-NOW; Functionalities of ESP-NOW can now be used");
  }
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
    snprintf(
      formatted_wifi_mac_address, sizeof(formatted_wifi_mac_address),
      "ESP32 Wi-Fi Station MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n"
      "ESP32 Wi-Fi AP MAC Address: %02X:%02X:%02X:%02X:%02X:%02X",
      raw_sta_mac[0], raw_sta_mac[1], raw_sta_mac[2], raw_sta_mac[3], raw_sta_mac[4], raw_sta_mac[5],
      raw_ap_mac[0], raw_ap_mac[1], raw_ap_mac[2], raw_ap_mac[3], raw_ap_mac[4], raw_ap_mac[5]
    );

    return String(formatted_wifi_mac_address); // Returns a formatted AP & STA if successful
  }

  return "Failed to retrieve ESP32 Wi-Fi MAC address"; // Returns an error if esp_read_mac was unsuccessful
}