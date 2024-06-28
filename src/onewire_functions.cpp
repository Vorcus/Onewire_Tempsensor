#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include "onewire_functions.hpp"

void initOneWireSensors()
{
  sensors.begin();

  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode())
    Serial.println("ON");
  else
    Serial.println("OFF");

  if (!sensors.getAddress(T1, 0))
    Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(T2, 1))
    Serial.println("Unable to find address for Device 1");
  if (!sensors.getAddress(T3, 2))
    Serial.println("Unable to find address for Device 2");

  Serial.print("Device 0 Address: ");
  printAddress(T1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(T2);
  Serial.println();

  Serial.print("Device 2 Address: ");
  printAddress(T3);
  Serial.println();

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
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16)
      Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
  {
    Serial.println("Error: Could not read temperature data");
    return;
  }
}

void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

void printSensorData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void printSensorsData()
{
  printSensorData(T1);
  printSensorData(T2);
  printSensorData(T3);
}

void requestTemperatures()
{
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
}

String getTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  return String(tempC);
}

String getTemperatures()
{
  requestTemperatures();
  String temps = "";
  temps += getTemperature(T1);
  temps += ", ";
  temps += getTemperature(T2);
  temps += ", ";
  temps += getTemperature(T3);
  return temps;
}