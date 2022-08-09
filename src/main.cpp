#include <Arduino.h>
#include <vector>

#include <lights.h>
#include <stateData.h>
#include <sensors.h>


long int lastrun;
#include <servoControl.h>
#include <serialComm.h>



long int lastrun;

uint32_t lastSerialWrite;

lights::LightManager *lightManager = new lights::LightManager();

sensors::SensorManager *sensorManager = new sensors::SensorManager();

servos::ServoManager *servoManager = new servos::ServoManager();
//int sonarReadings[36];
//int readingsIndex = 0;

//Ultrasonic* ultrasonic = new Ultrasonic(PA8, PA10, 5000);
int degree = 0;

void smoothReadings(int* arr, int len);

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
  servoManager->poll();
  lightManager->poll();

}
void smoothReadings(int* arr, int len){
  int newArr[len];
  for(int i = 1; i<len-1; i++){
    if(arr[i]>=arr[i-1]*1.4 && arr[i]>=arr[i+1]*1.4){
      newArr[i] = (arr[i-1]*arr[i+1])/2;
    }
  }
  arr = newArr;
}

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

