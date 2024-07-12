#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\wifi_functions.cpp"
#include "wifi_functions.hpp"

void initWiFi()
{
    // WiFi.setPins(8, 7, 4, 2);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting WiFi, SSID = ");
    Serial.println(WIFI_SSID);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\nIP address: ");
    Serial.println(WiFi.localIP());
}