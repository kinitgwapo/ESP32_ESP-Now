#ifndef ES_NOW_LIB_H
#define ES_NOW_LIB_H

/*
** @brief Setup ESP-Now
**
** @Note  esp_WiFi_mac() is called inside a print function to print out the WiFi MAC Address
**
** @returns
**        Nothing
*/
void esp_WiFi_Setup();

/*
** @brief ESP WiFi MAC Address
**        Takes the manufactured board's WiFi MAC Address and arranged in a neat hex format.
**
** @Note  This function is called by esp_WiFi_Setup() and is not needed to be called again.
**        It is also private to the source file, modify it if you wish to use the function somewhere.
**
** @returns
**        Succesful:  A 6-byte formatted Wi-Fi MAC address string
**        Failed:     Failed to retrieve ESP32 Wi-Fi MAC address
**
String esp_WiFi_mac();
*/

#endif