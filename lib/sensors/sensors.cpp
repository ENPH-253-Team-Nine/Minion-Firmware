#include "sensors.h"

using namespace sensors;

AbstractPolledSensor::AbstractPolledSensor(void *storeLocation, int pin){
    this->storeLocation = storeLocation;
    this->pin = pin;
}

void AbstractPolledSensor::setup(){
    pinMode(pin, INPUT_PULLUP);
}

AbstractInterruptSensor::AbstractInterruptSensor(void *storeLocation, int pin){
    this->storeLocation = storeLocation;
    this->pin = pin;
}

void AbstractInterruptSensor::setup(){
    pinMode(pin, INPUT_PULLUP);
}

/* IR Frequency*/

IRFrequency::IRFrequency(void *storeLocation, int pin) : AbstractPolledSensor(storeLocation, pin) {}

void IRFrequency::setup() {
    AbstractPolledSensor::setup();
}

void IRFrequency::read(){
    *((double *) storeLocation) = analogRead(pin); //pointer fuckery
}

/* Switch */

Switch::Switch(void *storeLocation, int pin) : AbstractPolledSensor(storeLocation, pin) {}

void Switch::setup(){
    AbstractPolledSensor::setup();
}

void Switch::read(){
    switch(digitalRead(pin)){
        case HIGH:
            *((SwitchState *) storeLocation) = SwitchState::CLOSED;
            break;
        case LOW:
            *((SwitchState *) storeLocation) = SwitchState::OPEN;
            break;
    }    
}

/* Reflectance */

ReflectanceSensor::ReflectanceSensor(void *storeLocation, int pin) : AbstractPolledSensor(storeLocation, pin) {}

void ReflectanceSensor::setup(){
    AbstractPolledSensor::setup();
}

void ReflectanceSensor::read(){
    *((int *) storeLocation) = analogRead(pin);
}

/* Hall Effect */

HallSensor::HallSensor(void *storeLocation, int pin) : AbstractPolledSensor(storeLocation, pin) {}

void HallSensor::setup(){
    AbstractPolledSensor::setup();
}

void HallSensor::read(){
    *((double *) storeLocation) = analogRead(pin);
}

/* Button */

Button::Button(void *storeLocation, int pin) : AbstractInterruptSensor(storeLocation, pin) {}

void Button::setup(){
    AbstractInterruptSensor::setup();
    std::function<void()> passableHandle = [=]{this->handler();};  //oh god more terrible pointer fuckery, now with lambdas
                                                                    //https://stackoverflow.com/questions/7582546/using-generic-stdfunction-objects-with-member-functions-in-one-class
    attachInterrupt(digitalPinToInterrupt(pin), passableHandle, RISING);
}

void Button::handler(){
    //IDK what this button is supposed to do at the moment.
}


/* Encoder */

//I am not confident in this

Encoder::Encoder(void *storeLocation, int pinA, int pinB){
    encoderA = new EncoderInterrupt(pinA, [=]{this->handler();});
    encoderB = new EncoderPoll(&encoderBValue, pinB);

    this->storeLocation = storeLocation;
}

void Encoder::setup(){
    encoderA->setup();
    encoderB->setup();
}

void Encoder::handler(){
    encoderB->read();
    switch(encoderBValue){
        case EncoderValue::ENCODER_HIGH:
            *((int *) storeLocation)-=1;
            break;
        case EncoderValue::ENCODER_LOW:
            *((int *) storeLocation)+=1;
            break;
    }
}


Encoder::EncoderInterrupt::EncoderInterrupt(int pin, std::function<void()> upperHandle) : AbstractInterruptSensor(nullptr, pin){
    this->upperHandle = upperHandle;
}

void Encoder::EncoderInterrupt::setup(){
    AbstractInterruptSensor::setup();
    attachInterrupt(digitalPinToInterrupt(pin), upperHandle, RISING);
}

Encoder::EncoderPoll::EncoderPoll(Encoder::EncoderValue* storeData, int pin) : AbstractPolledSensor(storeData, pin) {}

void Encoder::EncoderPoll::setup(){
    AbstractPolledSensor::setup();
}

void Encoder::EncoderPoll::read(){
        switch(digitalRead(pin)){
        case HIGH:
            *((Encoder::EncoderValue *) storeLocation) = Encoder::EncoderValue::ENCODER_HIGH;
            break;
        case LOW:
            *((Encoder::EncoderValue *) storeLocation) = Encoder::EncoderValue::ENCODER_LOW;
            break;
    } 
}

/* Sonar*/

SonarSensor::SonarSensor(){
    echoR = PA10;
    trigR = PA9;
    echoL = PA10;
    trigL = PA8;

    StateData::sonarServoPos = 0;
    degreeIndex = 0;

    sonarR = new NewPing(trigR,echoR,100);
    //sonarL = new NewPing(trigL, echoL, 100);

    lastReadingTime = millis();

    this->processor = new sonar::Processor();
}

void SonarSensor::setup(){
    
}

void SonarSensor::poll(){
    if(lastReadingTime+readingsDelay_ms <= millis()){
        //take a reading at the previous degree
        currentSweep[degreeIndex] = sonarR->ping_cm();
        //set the next degree for the next servo manager poll
        degreeIndex = advanceIndex(degreeIndex);
        StateData::sonarServoPos = getDegreeFromIndex(degreeIndex);
        lastReadingTime = millis();
    }
}

void SonarSensor::read(){
    if(sweepReady){
        for(int i=0; i<=350; i++){
            //Serial.println(0);
        }
        for(int i = 0; i<36;i++){
            StateData::sonarSweep[i] = currentSweep[i];

            Serial.println(currentSweep[i]); //debug only
        }
        processor->process();
        Serial.print("target hdg: "); Serial.println(StateData::sonarHeadingOutput);
        sweepReady = false;
        
    }
}

int SonarSensor::advanceIndex(int index){
    if(index>=35){
        degreeDelta = -1;
        sweepReady = true;
    } else if(index<=0){
        degreeDelta = 1;
        sweepReady = true;
    }

    return index+degreeDelta;


}

/* Sensor Manager */

SensorManager::SensorManager(){
    sonarSensor = new SonarSensor();

    //not dealing with interrupts at the moment, don't know what to do with the HMI


    polledSensors[polledSensorEnum::LINE_LEFT] = new ReflectanceSensor(&StateData::lineL,PA5);
    polledSensors[polledSensorEnum::LINE_RIGHT] = new ReflectanceSensor(&StateData::lineR,PA4);
    polledSensors[polledSensorEnum::EDGE_FL] = new ReflectanceSensor(&StateData::edgeReflectanceFL,PA3);
    polledSensors[polledSensorEnum::EDGE_FR] = new ReflectanceSensor(&StateData::edgeReflectanceFR,PA2);
    polledSensors[polledSensorEnum::EDGE_BL] = new ReflectanceSensor(&StateData::edgeReflectanceBL,PA1);
    polledSensors[polledSensorEnum::EDGE_BR] = new ReflectanceSensor(&StateData::edgeReflectanceBR,PA0);

}

void SensorManager::poll(){
    //for(AbstractPolledSensor* sensor : polledSensors){
     //   sensor->read();
    //}
    sonarSensor->poll();
    sonarSensor->read();
    
}

void SensorManager::setup(){
    //for(AbstractPolledSensor* sensor : polledSensors){
    //    sensor->setup();
    //}
    /*for(AbstractInterruptSensor* sensor : interruptedSensors){
        sensor->setup();
    }*/ //temp ignoring
    //for(Encoder* sensor : encoders){
    //    sensor->setup();
    //}

    // for(Encoder* sensor : encoders){
    //     sensor->setup();
    // }
}
