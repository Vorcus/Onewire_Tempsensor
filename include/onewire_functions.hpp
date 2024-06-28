#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 10
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define TEMPERATURE_PRECISION 9

DeviceAddress T1 = {0x28, 0xD8, 0xF4, 0x49, 0xF6, 0x2F, 0x3C, 0x29};
DeviceAddress T2 = {0x28, 0x98, 0x67, 0x49, 0xF6, 0x15, 0x3C, 0x6F};
DeviceAddress T3 = {0x28, 0xCC, 0xE7, 0x49, 0xF6, 0x11, 0x3C, 0x6D};

void initOneWireSensors();
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void printResolution(DeviceAddress deviceAddress);
void printSensorData(DeviceAddress deviceAddress);
void printSensorsData();
void requestTemperatures();
String getTemperature(DeviceAddress deviceAddress);
String getTemperatures();