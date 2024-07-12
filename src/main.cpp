#include <Arduino.h>
#include "onewire_functions.hpp"
#include "sd_functions.hpp"
#include "gps_functions.hpp"
#include "ftp_functions.hpp"
#include "wifi_functions.hpp"
#include "rtc_functions.hpp"

uint32_t gps_timer = millis();
uint32_t rtc_timer = millis();

void setup(void)
{
  Serial.begin(9600);
  initWiFi();
  initOneWireSensors();
  initSD();
  initGPS();
  initFTP();
  initRTC();
}
void loop(void)
{
  readGPS();
  if (millis() - gps_timer > 20000)
  {
    gps_timer = millis();
    SDLogTemperatures();
    FTPLogTemperatures();
    printSensorsData();
  }

  if (millis() - rtc_timer > 20000)
  {
    if (!rtcInitialized())
    {
      rtc_timer = millis();
      Serial.println("RTC is NOT initialized, syncing RTS to GPS time");
      syncRTCtoGPS();
      Serial.print("RTC Time: ");
      Serial.println(RTCDateTime());
    }
    if (rtcLostPower())
    {
      rtc_timer = millis();
      Serial.println("RTC lost power, syncing RTS to GPS time");
      syncRTCtoGPS();
      Serial.print("RTC Time: ");
      Serial.println(RTCDateTime());
    }
    if ((GPSday() == 1) && (GPShour() == 1) && (GPSminute() == 1) && (millis() - rtc_timer > 61000))
    {
      rtc_timer = millis();
      Serial.println("[describe error here], syncing RTS to GPS time");
      syncRTCtoGPS();
      Serial.print("RTC Time: ");
      Serial.println(RTCDateTime());
    }
  }
}