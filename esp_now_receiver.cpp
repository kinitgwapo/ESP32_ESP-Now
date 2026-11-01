#include <Arduino.h>
#include <ESP32_NOW.h>

#define BYTES_MAX_COMPARISON 6
#define SUCCESS 0

#define MAC_QUEUE_LENGTH 10
#define MAC_LENGTH 6

QueueHandle_t mac_queue = NULL;

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

void esp_NOW_report()
{
  uint8_t mac[MAC_LENGTH];
  while(xQueueReceive(mac_queue, mac, 0) == pdTRUE)
  {
    char formatted_mac[18] = {0};

    snprintf(formatted_mac, sizeof(formatted_mac), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]
    );

    Serial.println("Broadcast received from STA MAC: " + String(formatted_mac));
  }
}