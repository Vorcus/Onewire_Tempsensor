#include <SD.h>
#include "gps_functions.hpp"
#include "rtc_functions.hpp"
#include "onewire_functions.hpp"

const int chipSelect = 10;
File DataLog;
char fileName[] = "datalog.txt";

void initSD();
void SDAppendLine(String line);
void SDLogTemperatures();