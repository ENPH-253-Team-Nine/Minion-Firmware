#include <Arduino.h>
#include <vector>

#include <lights.h>
#include <stateData.h>
#include <sensors.h>


long int lastrun;

#include <servoControl.h>


lights::LightManager *lightManager = new lights::LightManager();

sensors::SensorManager *sensorManager = new sensors::SensorManager();

servos::ServoManager *servoManager = new servos::ServoManager();
//int sonarReadings[36];
//int readingsIndex = 0;

//Ultrasonic* ultrasonic = new Ultrasonic(PA8, PA10, 5000);
int degree = 0;

void smoothReadings(int* arr, int len);

void setup()
{
  lightManager->setup();
  sensorManager->setup();
  servoManager->setup();

  Serial.begin(9600);
  lastrun = millis();
}

void loop()
{
  sensorManager->poll();
  servoManager->poll();
  lightManager->poll();

  // StateData::sonarServoPos = degree;
  // servoManager->poll();
  // delay(30);
  // sonarReadings[readingsIndex] = ultrasonic->read();
  // degree+=5;
  // readingsIndex++;
  // sensorManager->poll();
  // lightManager->poll();

  // if(degree == 180){
  //   delay(100);
  //   degree = 0;
  //   readingsIndex = 0;
  //   //smoothReadings(sonarReadings, 36);
  //   StateData::sonarServoPos = degree;
  //   Serial.print("[");
  //   for(int i = 0; i<36; i++){
  //     Serial.println(sonarReadings[i]);
  //     //Serial.print(", ");
  //   }
  //   for(int i = 0; i<=350; i++){
  //   Serial.println(0);
  //   }
  //   //Serial.println("]");
  //   servoManager->poll();
  //   delay(2000);
  //}
  

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