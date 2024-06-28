#include <Adafruit_GPS.h>

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

#define GPSECHO false

void initGPS();
void readGPS();
String GPSTime();
String GPSDate();
String GPSDateTime();