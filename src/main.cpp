#include <Arduino.h>

#include <lights.h>
#include <stateData.h>
#include <sensors.h>
#include <servoControl.h>
#include <serialComm.h>

long int lastrun;

long int lastSerialWrite;

lights::LightManager *lightManager = new lights::LightManager();

sensors::SensorManager *sensorManager = new sensors::SensorManager();

servos::ServoManager *servoManager = new servos::ServoManager();

serialComm::SerialManager *serialManager = new serialComm::SerialManager();

void setup()
{
  lightManager->setup();
  sensorManager->setup();
  servoManager->setup();
  setupDataStore();
  serialManager->setup();

  lastSerialWrite = millis();

  Serial.begin(9600);
  lastrun = millis();
}

void loop()
{
  sensorManager->poll();
  lightManager->poll();
  servoManager->poll();
  serialManager->poll();

  if (millis() - lastSerialWrite >= 500) {
    lastSerialWrite = millis();
  }

}