#include "sd_functions.hpp"

const int chipSelect = 10;
File DataLog;

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