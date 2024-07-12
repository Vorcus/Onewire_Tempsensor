#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\sd_functions.hpp"
#include <SD.h>
#include "gps_functions.hpp"
#include "rtc_functions.hpp"
#include "onewire_functions.hpp"

void initSD();
void SDAppendLine(String line);
void SDLogTemperatures();