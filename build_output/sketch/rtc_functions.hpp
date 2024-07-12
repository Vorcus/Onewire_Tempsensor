#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\rtc_functions.hpp"
#include <RTClib.h>
#include "gps_functions.hpp"

void initRTC();

String RTCDateTime();

void syncRTCtoGPS();
bool rtcInitialized();
bool rtcLostPower();