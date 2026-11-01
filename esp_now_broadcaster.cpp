#include <Arduino.h>
#include <ESP32_NOW.h>

#define SUCCESS 1
#define FAIL 0

#define ESP_NOW_WIFI_CHANNEL 1
#define ANNOUNCE_MARKER 0xA5
#define ANNOUNCE_INTERVAL_MS 2000
#define ANNOUNCE_JITTER_MS 500

class ESP_NOW_Broadcast_Peer : public ESP_NOW_Peer
{
public:
  ESP_NOW_Broadcaster_Peer(const uint8_t *mac_addr, uint8_t channel, wifi_interface_t iface, const uint8_t *lmk) : ESP_NOW_Peer(ESP_NOW.BROADCAST_ADDR, channel, iface, lmk) {}

  ~ESP_NOW_Broadcast_Peer() { remove(); }

  bool begin() { return add(); }

  bool announce()
  {
    const uint8_t payload[1] = {ANNOUNCE_MARKER};
    return send(payload, sizeof(payload));
  }
};

ESP_NOW_Broadcast_Peer esp_now_broadcast_peer(ESP_NOW_WIFI_CHANNEL, WIFI_IF_STA, nullptr);
static unsigned long next_announce_ms = 0;

void esp_NOW_broadcast_init()
{
  bool err = esp_now_broadcast_peer.begin();
  while(err == FAIL)
  {
    Serial.println("Failed to register broadcast peer, retrying...");
    err = esp_now_broadcast_peer.begin();
    delay(100);
  }

  Serial.println("Broadcast peer registered; announcements will be sent from the STA interface");
  next_announce_ms = millis() + (esp_random() % ANNOUNCE_JITTER_MS);
}

void esp_NOW_announce()
{
  if((int32_t)(millis() - next_announce_ms) < 0) return;

  if(!esp_now_broadcast_peer.announce())
  {
    Serial.println("Failed to broadcast announcement");
  }

  next_announce_ms = millis() + ANNOUNCE_INTERVAL_MS + (esp_random() % ANNOUNCE_JITTER_MS);
}