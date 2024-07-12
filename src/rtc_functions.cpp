#include "rtc_functions.hpp"
#include "gps_functions.hpp"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

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
    if (GPSfix() == 1)
    {
        rtc.adjust(DateTime(GPSyear(), GPSmonth(), GPSday(), GPShour(), GPSminute(), GPSmilliseconds()));
    }
    else
    {
        Serial.println("No GPS fix to sync RTC");
    }
}

bool rtcInitialized()
{
    return rtc.initialized();
}

bool rtcLostPower()
{
    return rtc.lostPower();
}