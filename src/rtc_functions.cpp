#include "rtc_functions.hpp"

void initRTC()
{
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
}

String RTCDateTime()
{
    DateTime now = rtc.now();
    String dateTime = "";
    dateTime += now.year();
    dateTime += '/';
    dateTime += now.month();
    dateTime += '/';
    dateTime += now.day();
    dateTime += ' ';
    dateTime += now.hour();
    dateTime += ':';
    dateTime += now.minute();
    dateTime += ':';
    dateTime += now.second();
    return dateTime;
}

void syncRTCtoGPS()
{
    if (GPS.fix == 1)
    {
        rtc.adjust(DateTime(GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, GPS.seconds));
    }
    else
    {
        Serial.println("No GPS fix to sync RTC");
    }
}