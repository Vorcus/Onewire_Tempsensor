#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\sd_functions.cpp"
#include "sd_functions.hpp"

const int chipSelect = 10;
File DataLog;
char filename()
{
    RTC_PCF8523 rtc;
    DateTime now = rtc.now();
    sprintf(fileName, "%04lu%02lu%02lu.csv", now.year(), now.month(), now.day());
    return fileName[13];
}

void initSD()
{
    Serial.print("Initializing SD card...");
    if (!SD.begin(10))
    {
        Serial.println("Card failed, or not present");
        while (1)
            ;
    }
    Serial.println("card initialized.");
}

void SDAppendLine(String line)
{
    DataLog = SD.open(fileName, FILE_WRITE);
    if (DataLog)
    {
        DataLog.println(line);
        DataLog.close();
    }
    else
    {
        Serial.print("error opening: ");
        Serial.println(fileName);
    }
}

void SDLogTemperatures()
{
    if (GPSfix() == 1)
    {
        String dataline = GPSDateTime();
        dataline += ",";
        dataline += getTemperatures();
        SDAppendLine(dataline);
    }
    else
    {
        String dataline = RTCDateTime();
        dataline += ",";
        dataline += getTemperatures();
        SDAppendLine(dataline);
    }
}