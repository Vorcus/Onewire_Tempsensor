#include "gps_functions.hpp"
#include "rtc_functions.hpp"
#include "onewire_functions.hpp"
#include "wifi_functions.hpp"

#define USING_VSFTP_SERVER true

void initFTP();
void FTPAppendLine(String line);
void FTPLogTemperatures();