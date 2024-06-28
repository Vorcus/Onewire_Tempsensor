#include "ftp_functions.hpp"

void initFTP()
{
    // --- FTP SETUP ---
    //
    // while (!Serial && millis() < 5000)
    //;

    // delay(500);

    Serial.print(F("\nStarting FTPClient_ListFiles on "));
    Serial.print(BOARD_NAME);
    Serial.print(F(" with "));
    Serial.println(SHIELD_TYPE);
    Serial.println(FTPCLIENT_GENERIC_VERSION);
}

void FTPAppendLine(String line)
{
    ftp.OpenConnection();
    ftp.ChangeWorkDir(dirName);
    ftp.InitFile(COMMAND_XFER_TYPE_ASCII);
    ftp.AppendFile(fileName);
    ftp.Write(line.c_str());
    ftp.Write("\n");
    ftp.CloseFile();
    Serial.println("FTP CloseConnection");
    ftp.CloseConnection();
}

void FTPLogTemperatures()
{
    if (GPS.fix == 1)
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