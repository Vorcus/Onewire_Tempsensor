#include "ftp_functions.hpp"
#include "ftp_client.hpp"

char ftp_server[] = "172.18.204.75";
char ftp_user[] = "raspy";
char ftp_pass[] = "5RaspiFoxes";
char dirname[] = "/files";
char filename[] = "datalog.txt";

FTPClient_Generic ftp(ftp_server, ftp_user, ftp_pass, 60000);

void initFTP()
{
    // code to initialize the FTP client
    // currently nothing is needed here
}

void FTPAppendLine(String line)
{
    ftp.OpenConnection();
    ftp.ChangeWorkDir(dirname);
    ftp.InitFile(COMMAND_XFER_TYPE_ASCII);
    ftp.AppendFile(filename);
    ftp.Write(line.c_str());
    ftp.Write("\n");
    ftp.CloseFile();
    Serial.println("FTP CloseConnection");
    ftp.CloseConnection();
}

void FTPLogTemperatures()
{
    if (GPSfix() == 1)
    {
        String dataline = GPSDateTime();
        dataline += ",";
        dataline += getTemperatures();
        FTPAppendLine(dataline);
    }
    else
    {
        String dataline = RTCDateTime();
        dataline += ",";
        dataline += getTemperatures();
        FTPAppendLine(dataline);
    }
}