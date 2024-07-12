#include <Adafruit_GPS.h>

#define GPSSerial Serial1

#define GPSECHO false

void initGPS();
void readGPS();
String GPSTime();
String GPSDate();
String GPSDateTime();
bool GPSfix();
uint8_t GPSyear();
uint8_t GPSmonth();
uint8_t GPSday();
uint8_t GPShour();
uint8_t GPSminute();
uint8_t GPSseconds();
uint16_t GPSmilliseconds();