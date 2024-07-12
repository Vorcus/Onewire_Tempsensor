#line 1 "C:\\Users\\Trey\\Documents\\git\\Onewire_Tempsensor\\src\\ftp_functions.hpp"
#include <FTPClient_Generic.h>
#include "defines.hpp"
#include "gps_functions.hpp"
#include "rtc_functions.hpp"
#include "onewire_functions.hpp"

#define USING_VSFTP_SERVER true

void initFTP();
void FTPAppendLine(String line);
void FTPLogTemperatures();