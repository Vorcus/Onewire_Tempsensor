// --- RTC ---
// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include <RTClib.h>
#include "gps_functions.hpp"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
// --- RTC END ---

void initRTC();

String RTCDateTime();

void syncRTCtoGPS();