#include <Arduino.h>
#include <ESP32_NOW.h>

#define BYTES_MAX_COMPARISON 6
#define SUCCESS 0

#define MAC_QUEUE_LENGTH 10
#define MAC_LENGTH 6

#define MAX_DEVICES 40

QueueHandle_t mac_queue = NULL;

uint8_t recordedMACS[MAX_DEVICES][MAC_LENGTH];
uint8_t counter = 0;

void esp_NOW_receive_msg(const esp_now_recv_info_t *info, const uint8_t *data, int len, void *arg)
{
  if(memcmp(info->des_addr, ESP_NOW.BROADCAST_ADDR, BYTES_MAX_COMPARISON) == SUCCESS)
  {
    xQueueSend(mac_queue, info->src_addr, 0);
  }
}

void esp_NOW_receiver_init()
{
  mac_queue = xQueueCreate(MAC_QUEUE_LENGTH, MAC_LENGTH);
  while(mac_queue == NULL)
  {
    Serial.println("Failed to create MAC Queue");
    mac_queue = xQueueCreate(MAC_QUEUE_LENGTH, MAC_LENGTH);
    delay(100);
  }

  Serial.println("Successfully created MAC Queue");
  
  ESP_NOW.onNewPeer(esp_NOW_receive_msg, nullptr);
}

static void mac_print()
{
  Serial.println("// ----- Discovered Devices " + String(counter) + " ----- //");

  for(uint8_t i = 0; i < counter; i++)
  {
    char formatted_mac[18] = {0};

    snprintf(formatted_mac, sizeof(formatted_mac), "%02X:%02X:%02X:%02X:%02X:%02X",
              recordedMACS[i][0], recordedMACS[i][1], recordedMACS[i][2], recordedMACS[i][3], recordedMACS[i][4], recordedMACS[i][5]
    );

    Serial.println(String(i + 1) + ": " + String(formatted_mac));
  }

  Serial.println("// --------------------------------- //");
}

static void mac_check(const uint8_t *mac_array)
{
  // Check if MAC already recorded
  for(uint8_t i = 0; i < counter; i++)
  {
    if(memcmp(recordedMACS[i], mac_array, BYTES_MAX_COMPARISON) == 0)
    {
      return;
    }
  }

  // Check if MAC list is full
  if(counter >= MAX_DEVICES)
  {
    Serial.println("MAC Registry Full!");
    return;
  }

  // Record new MAC Address
  memcpy(recordedMACS[counter], mac_array, BYTES_MAX_COMPARISON);
  counter++;

  // Print only when new MAC address is recorded
  mac_print();
}

void esp_NOW_report()
{
  uint8_t mac[MAC_LENGTH];
  while(xQueueReceive(mac_queue, mac, 0) == pdTRUE)
  {
    mac_check(mac);
  }
}