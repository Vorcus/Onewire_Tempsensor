#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 10

#define TEMPERATURE_PRECISION 9

void initOneWireSensors();
void printAddress(DeviceAddress deviceAddress);
void printTemperature(DeviceAddress deviceAddress);
void printResolution(DeviceAddress deviceAddress);
void printSensorData(DeviceAddress deviceAddress);
void printSensorsData();
void requestTemperatures();
String getTemperature(DeviceAddress deviceAddress);
String getTemperatures();