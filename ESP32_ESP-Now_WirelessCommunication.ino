#include "esp_now_lib.h"

void esp_NOW_report();

void setup() {
  esp_WiFi_Setup();
}

void loop() {
  esp_NOW_report();
}