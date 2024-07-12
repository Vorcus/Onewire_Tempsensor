#include <RTClib.h>
#include "gps_functions.hpp"

void initRTC();

String RTCDateTime();

void syncRTCtoGPS();
bool rtcInitialized();
bool rtcLostPower();