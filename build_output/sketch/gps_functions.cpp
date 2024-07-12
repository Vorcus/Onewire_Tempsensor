#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\gps_functions.cpp"
#include "gps_functions.hpp"

Adafruit_GPS GPS(&GPSSerial);

void initGPS()
{
    GPS.begin(9600);
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    delay(1000);
    GPSSerial.println(PMTK_Q_RELEASE);
}

void readGPS()
{
    char c = GPS.read();
    if (GPSECHO)
        if (c)
            Serial.print(c);
    if (GPS.newNMEAreceived())
    {
        if (!GPS.parse(GPS.lastNMEA()))
            return;
    }
}

String GPSTime()
{
    String time = "";
    if (GPS.fix)
    {
        if (GPS.hour < 10)
        {
            time += '0';
        }
        time += GPS.hour;
        time += ':';
        if (GPS.minute < 10)
        {
            time += '0';
        }
        time += GPS.minute;
        time += ':';
        if (GPS.seconds < 10)
        {
            time += '0';
        }
        time += GPS.seconds;
        time += '.';
        if (GPS.milliseconds < 10)
        {
            time += "00";
        }
        else if (GPS.milliseconds > 9 && GPS.milliseconds < 100)
        {
            time += "0";
        }
        time += GPS.milliseconds;
    }
    return time;
}

String GPSDate()
{
    String date = "";
    if (GPS.fix)
    {
        date += "20";
        date += GPS.year;
        date += '/';
        date += GPS.month;
        date += '/';
        date += GPS.day;
    }
    return date;
}

String GPSDateTime()
{
    String dateTime = "";
    if (GPS.fix)
    {
        dateTime += GPSDate();
        dateTime += " ";
        dateTime += GPSTime();
    }
    return dateTime;
}

bool GPSfix()
{
    return GPS.fix;
}

uint8_t GPSyear()
{
    return GPS.year;
}

uint8_t GPSmonth()
{
    return GPS.month;
}
uint8_t GPSday()
{
    return GPS.day;
}
uint8_t GPShour()
{
    return GPS.hour;
}
uint8_t GPSminute()
{
    return GPS.minute;
}
uint8_t GPSseconds()
{
    return GPS.seconds;
}
uint16_t GPSmilliseconds()
{
    return GPS.milliseconds;
}