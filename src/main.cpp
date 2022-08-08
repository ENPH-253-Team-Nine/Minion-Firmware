#include <Arduino.h>

#include <lights.h>
#include <stateData.h>
#include <sensors.h>
#include <servoControl.h>
#include <serialComm.h>

int freeMemory();

long int lastrun;

uint32_t lastSerialWrite;

lights::LightManager *lightManager = new lights::LightManager();

sensors::SensorManager *sensorManager = new sensors::SensorManager();

servos::ServoManager *servoManager = new servos::ServoManager();

serialComm::SerialManager *serialManager = new serialComm::SerialManager();

void setup()
{
  lightManager->setup();
  sensorManager->setup();
  servoManager->setup();
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

  if (millis() - lastSerialWrite >= (uint32_t)1000) {
    lastSerialWrite = millis();
    Serial.print("~~~ Get: "); Serial.println(StateData::receivedByte);

    Serial.print("~~~ Send: ");
    Serial.print(StateData::lineL); Serial.print(", ");
    Serial.print(", ");
    Serial.print(StateData::edgeReflectanceBR); Serial.print(", ");
    Serial.println();
  }
  delay(100);
}

