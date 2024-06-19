// --- Temperature Sensors ---
//
// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 10 on the Arduino
#define ONE_WIRE_BUS 10
#define TEMPERATURE_PRECISION 9

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
//DeviceAddress T1, T2;

// Assign address manually. The addresses below will need to be changed
// to valid device addresses on your bus. Device address can be retrieved
// by using either oneWire.search(deviceAddress) or individually via
// sensors.getAddress(deviceAddress, index)
DeviceAddress T1 = { 0x28, 0xD8, 0xF4, 0x49, 0xF6, 0x2F, 0x3C, 0x29 };
DeviceAddress T2 = { 0x28, 0x98, 0x67, 0x49, 0xF6, 0x15, 0x3C, 0x6F };
DeviceAddress T3 = { 0x28, 0xCC, 0xE7, 0x49, 0xF6, 0x11, 0x3C, 0x6D };
//
// --- Temperature Sensors END ---

// --- GPS ---
#include <Adafruit_GPS.h>
// what's the name of the hardware serial port?
#define GPSSerial Serial1
// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
// --- GPS END ---

// --- SD ---
#include <SD.h>
const int chipSelect = 10;
File DataLog;
// --- SD END ---

// --- RTC ---
// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
// --- RTC END ---

// --- FTP ---
//
#include "Arduino.h"
#include "defines.h"
#include <FTPClient_Generic.h>

// To use `true` with the following PASV mode asnswer from server, such as `VSFTP`
// 227 Entering Passive Mode (192,168,2,112,157,218)
// Using `false` with old style PASV answer, such as `FTP_Server_Teensy41` library
// 227 Entering Passive Mode (4043483328, port 55600)
#define USING_VSFTP_SERVER true

#if USING_VSFTP_SERVER

// Change according to your FTP server
char ftp_server[] = "172.18.204.75";

char ftp_user[] = "raspy";
char ftp_pass[] = "5RaspiFoxes";

char dirName[] = "/files";

#else

// Change according to your FTP server
char ftp_server[] = "172.18.204.75";

char ftp_user[] = "raspy";
char ftp_pass[] = "5RaspiFoxes";

char dirName[] = "/files";

#endif

// FTPClient_Generic(char* _serverAdress, char* _userName, char* _passWord, uint16_t _timeout = 10000);
FTPClient_Generic ftp(ftp_server, ftp_user, ftp_pass, 60000);
//
// --- FTP END ---

uint32_t gps_timer = millis();
uint32_t rtc_timer = millis();
//uint32_t loop_counter = (0);

void setup(void) {
  WiFi.setPins(8, 7, 4, 2);
  // start serial port
  Serial.begin(9600);  //GPS+FTP+WiFi library examples requested 115200, RTC example requested 57600, SD and Sensor called for 9600. Seems to work fine 6/16/24 on 9600.

  // --- SENSORS SETUP ---
  //
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // Search for devices on the bus and assign based on an index. Ideally,
  // you would do this to initially discover addresses on the bus and then
  // use those addresses and manually assign them (see above) once you know
  // the devices on your bus (and assuming they don't change).
  //
  // method 1: by index
  if (!sensors.getAddress(T1, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(T2, 1)) Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(T3, 2)) Serial.println("Unable to find address for Device 2");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(T1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(T2);
  Serial.println();

  Serial.print("Device 2 Address: ");
  printAddress(T3);
  Serial.println();

  // set the resolution to 9 bit per device
  sensors.setResolution(T1, TEMPERATURE_PRECISION);
  sensors.setResolution(T2, TEMPERATURE_PRECISION);
  sensors.setResolution(T3, TEMPERATURE_PRECISION);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(T1), DEC);
  Serial.println();

  Serial.print("Device 1 Resolution: ");
  Serial.print(sensors.getResolution(T2), DEC);
  Serial.println();

  Serial.print("Device 2 Resolution: ");
  Serial.print(sensors.getResolution(T3), DEC);
  Serial.println();
  //
  // --- SENSORS SETUP END ---

  // --- SD SETUP ---
  //
  // Open serial communications and wait for port to open:
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
  //
  // --- SD SETUP END ---

  // --- GPS SETUP ---
  //
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);  // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  //GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
  //
  // --- GPS SETUP END ---

  // --- FTP SETUP ---
  //
  //while (!Serial && millis() < 5000)
  //;

  //delay(500);

  Serial.print(F("\nStarting FTPClient_ListFiles on "));
  Serial.print(BOARD_NAME);
  Serial.print(F(" with "));
  Serial.println(SHIELD_TYPE);
  Serial.println(FTPCLIENT_GENERIC_VERSION);

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting WiFi, SSID = ");
  Serial.println(WIFI_SSID);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nIP address: ");
  Serial.println(WiFi.localIP());
//
// --- FTP SETUP END ---


// --- RTC SETUP ---
//
#ifndef ESP8266
  while (!Serial)
    ;  // wait for serial port to connect. Needed for native USB
#endif

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
}

//if (!rtc.initialized() || rtc.lostPower()) {
//Serial.println("RTC is NOT initialized, let's set the time!");
//delay(2500);
// When time needs to be set on a new device, or after a power loss, the
// following line sets the RTC to the date & time this sketch was compiled
//rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

// This line sets the RTC with an explicit date & time, for example to set
// January 21, 2014 at 3am you would call:
// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
//
// Note: allow 2 seconds after inserting battery or applying external power
// without battery before calling adjust(). This gives the PCF8523's
// crystal oscillator time to stabilize. If you call adjust() very quickly
// after the RTC is powered, lostPower() may still return true.
//
//  --- RTC SETUP END ---



// ---TempSens Functions START---
// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress) {
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return;
  }
  //Serial.print("Temp C: ");
  //Serial.print(tempC);
  //Serial.print(" Temp F: ");
  //Serial.print(DallasTemperature::toFahrenheit(tempC));
  //Serial.println("");
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress) {
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress) {
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}
// ---TempSens Functions END---

void loop(void) {
  // --- GPS + LOGGING LOOP START ---
  //
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    //Serial.print(GPS.lastNMEA());    // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA()))  // this also sets the newNMEAreceived() flag to false
      return;                        // we can fail to parse a sentence in which case we should just wait for another

    // approximately every 20 seconds or so, print out the current stats
    if (millis() - gps_timer > 20000) {
      gps_timer = millis();  // reset the gps_timer
      DataLog = SD.open("datalog.txt", FILE_WRITE);
      if (DataLog) {
        if (GPS.fix = 1) {
          // SD LOG
          DataLog.print("\n");
          if (GPS.hour < 10) { DataLog.print('0'); }
          DataLog.print(GPS.hour, DEC);
          DataLog.print(':');
          if (GPS.minute < 10) { DataLog.print('0'); }
          DataLog.print(GPS.minute, DEC);
          DataLog.print(':');
          if (GPS.seconds < 10) { DataLog.print('0'); }
          DataLog.print(GPS.seconds, DEC);
          DataLog.print('.');
          if (GPS.milliseconds < 10) {
            DataLog.print("00");
          } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
            DataLog.print("0");
          }
          DataLog.print(GPS.milliseconds);
          DataLog.print(",");
          DataLog.print(" ");
          DataLog.print(GPS.day);
          DataLog.print('/');
          DataLog.print(GPS.month);
          DataLog.print("/20");
          DataLog.print(GPS.year);
          DataLog.print(",");
          // SD LOG END
          // Convert int variables to String
          int gpsHour = GPS.hour;
          String gpsHourStr = String(gpsHour);

          int gpsMinute = GPS.minute;
          String gpsMinuteStr = String(gpsMinute);

          int gpsSecond = GPS.seconds;
          String gpsSecondStr = String(gpsSecond);

          int gpsMilis = GPS.hour;
          String gpsMilisStr = String(gpsMilis);
          // FTP LOG
          ftp.OpenConnection();
          ftp.ChangeWorkDir(dirName);
          ftp.InitFile(COMMAND_XFER_TYPE_ASCII);
          ftp.AppendFile("datalog.txt");

          if (GPS.hour < 10) { ftp.Write("/n0"); }
          ftp.Write(gpsHourStr.c_str());
          ftp.Write(":");
          if (GPS.minute < 10) { ftp.Write("0"); }
          ftp.Write(gpsMinuteStr.c_str());
          ftp.Write(":");
          //if (GPS.seconds < 10) { ftp.Write("0"); }
          //ftp.Write(GPS.seconds, DEC);
          //ftp.Write(".");
          //if (GPS.milliseconds < 10) {
          //  ftp.Write("00");
          //} else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
          //  ftp.Write("0");
          //}
          //ftp.Write(GPS.milliseconds));
          //ftp.Write(",");
          //ftp.Write(" ");
          //ftp.Write(GPS.day));
          //ftp.Write("/");
          //ftp.Write(GPS.month));
          //ftp.Write("/20");
          //ftp.Write(GPS.year));
          //ftp.Write(",");
          ftp.CloseFile();

          Serial.println("FTP CloseConnection");
          ftp.CloseConnection();
          // FTP LOG END
          Serial.print("\nGPS Time: ");
          if (GPS.hour < 10) { Serial.print('0'); }
          Serial.print(GPS.hour, DEC);
          Serial.print(':');
          if (GPS.minute < 10) { Serial.print('0'); }
          Serial.print(GPS.minute, DEC);
          Serial.print(':');
          if (GPS.seconds < 10) { Serial.print('0'); }
          Serial.print(GPS.seconds, DEC);
          Serial.print('.');
          if (GPS.milliseconds < 10) {
            Serial.print("00");
          } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
            Serial.print("0");
          }
          Serial.print(GPS.milliseconds);
          Serial.print(",");
          Serial.print(" ");
          Serial.print(GPS.day);
          Serial.print('/');
          Serial.print(GPS.month);
          Serial.print("/20");
          Serial.print(GPS.year);
          Serial.print(",");

          Serial.print("RTC Time:");
          DateTime now = rtc.now();
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.print(':');
          Serial.print(now.second(), DEC);
          Serial.print(',');
          Serial.print(' ');
          Serial.print(now.day(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.year(), DEC);
          Serial.println();

          //Serial.print("Fix: ");
          //Serial.print((int)GPS.fix);
          //Serial.print(" quality: ");
          //Serial.print((int)GPS.fixquality);
          //DataLog.print("Fix: ");
          //DataLog.print((int)GPS.fix);
          //DataLog.print(" quality: ");
          //DataLog.print((int)GPS.fixquality);
          //DataLog.print(",");

          //if (GPS.fix) {
          //DataLog.print("Location: ");
          //DataLog.print(GPS.latitude, 4);
          //DataLog.print(GPS.lat);
          //DataLog.print(", ");
          //DataLog.print(GPS.longitude, 4);
          //DataLog.println(GPS.lon);
          //DataLog.print("Speed (knots): ");
          //DataLog.println(GPS.speed);
          //DataLog.print("Angle: ");
          //DataLog.println(GPS.angle);
          //DataLog.print("Altitude: ");
          //DataLog.println(GPS.altitude);
          //DataLog.print("Satellites: ");
          //DataLog.println((int)GPS.satellites);
          //DataLog.print("Antenna status: ");
          //DataLog.println((int)GPS.antenna);
          DataLog.close();
        }
        if (GPS.fix = 0) {
          DateTime now = rtc.now();
          Serial.print(now.hour(), DEC);
          Serial.print(':');
          Serial.print(now.minute(), DEC);
          Serial.print(':');
          Serial.print(now.second(), DEC);
          Serial.print(',');
          Serial.print(' ');
          Serial.print(now.day(), DEC);
          Serial.print('/');
          Serial.print(now.month(), DEC);
          Serial.print('/');
          Serial.print(now.year(), DEC);
          Serial.println();

          //Serial.print(" (");
          //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
          //Serial.print(") ");
          DataLog.close();
        }
      }
      //
      // ---  GPS + LOGGING LOOP END ---

      // --- TempSens LOOP START ---
      //
      DataLog = SD.open("datalog.txt", FILE_WRITE);
      if (DataLog) {
        // call sensors.requestTemperatures() to issue a global te mperature
        // request to all devices on the bus
        Serial.print("Requesting temperatures...");
        sensors.requestTemperatures();
        Serial.println("DONE");

        // print the device information
        printData(T1);
        printData(T2);
        printData(T3);

        DataLog.print(" ");
        DataLog.print(sensors.getTempC(T1));
        DataLog.print(",");
        DataLog.print(" ");
        DataLog.print(sensors.getTempC(T2));
        DataLog.print(",");
        DataLog.print(" ");
        DataLog.print(sensors.getTempC(T3));
        DataLog.close();
      }
    }
  }
  // --- TempSens LOOP END ---
  //Test

  //Test End
  // --- RTC Loop START ---
  if (millis() - rtc_timer > 20000) {
    if (!rtc.initialized() || rtc.lostPower() || GPS.day == (1) && GPS.hour == (1) && GPS.minute == (1) && millis() - rtc_timer > 61000) {
      rtc_timer = millis();
      if (GPS.fix = 0) {
        Serial.print("No GPS fix to sync RTC");
      } else {
        Serial.println("RTC is NOT initialized, let's set the time!");
        //loop_counter = loop_counter + (1);

        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(GPS.year, GPS.month, GPS.day, GPS.hour, GPS.minute, GPS.seconds));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
        //
        // Note: allow 2 seconds after inserting battery or applying external power
        // without battery before calling adjust(). This gives the PCF8523's
        // crystal oscillator time to stabilize. If you call adjust() very quickly
        // after the RTC is powered, lostPower() may still return true.
        DateTime now = rtc.now();

        Serial.print(now.year(), DEC);
        Serial.print('/');
        Serial.print(now.month(), DEC);
        Serial.print('/');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();
        //
        // --- RTC Loop END ---
      }
    }
  }
}
