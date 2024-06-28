#include <FTPClient_Generic.h>
#include "defines.hpp"
#include "gps_functions.hpp"
#include "rtc_functions.hpp"
#include "onewire_functions.hpp"

#define USING_VSFTP_SERVER true

char ftp_server[] = "172.18.204.75";
char ftp_user[] = "raspy";
char ftp_pass[] = "5RaspiFoxes";
char dirName[] = "/files";
char fileName[] = "datalog.txt";

FTPClient_Generic ftp(ftp_server, ftp_user, ftp_pass, 60000);

void initFTP();
void FTPAppendLine(String line);
void FTPLogTemperatures();