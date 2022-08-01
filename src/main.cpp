#include <Arduino.h>

#include <stateMachine.h>
#include <lights.h>
#include <motorControl.h>
#include <stateData.h>
#include <trajectoryPlanning.h>
#include <sensors.h>
#include <bridge.h>
#include <arm.h>

long int lastrun;

long int lastSerialWrite;

#include <servoControl.h>

StateMachine::StateManager *stateManager = new StateMachine::StateManager();

lights::LightManager *lightManager = new lights::LightManager();

sensors::SensorManager *sensorManager = new sensors::SensorManager();

servos::ServoManager *servoManager = new servos::ServoManager();

void setup()
{
  lightManager->setup();
  motorManager->setup();
  sensorManager->setup();
  servoManager->setup();
  setupDataStore();

  lastSerialWrite = millis();

  Serial.begin(9600);
  Serial2.begin(9600);
  lastrun = millis();
}

void loop()
{
  sensorManager->poll();
  if (*StateData::state == StateMachine::StateEnum::Error)
  {
    StateData::clawServoPos = 180;
  }
  else
  {
    StateData::clawServoPos = 0;
  }

  stateManager->poll();
  lightManager->poll();
  servoManager->poll();

  if (millis() - lastSerialWrite >= 500) {
    Serial2.write(dataStore);
    lastSerialWrite = millis();
  }

}