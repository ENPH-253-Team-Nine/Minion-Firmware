#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <stateData.h>

namespace sensors
{
    class SensorManager; //forward declared

    enum class SwitchState{
        OPEN,
        CLOSED
    };

    class AbstractPolledSensor
    {
        friend SensorManager;
    protected:
        AbstractPolledSensor(void *storeLocation, int pin);
        void* storeLocation;
        int pin;
        void setup(); 
    private:
        
        virtual void read() = 0;
    };

    class AbstractInterruptSensor
    {
        friend SensorManager;
    protected:
        AbstractInterruptSensor(void *storeLocation, int pin);
        void* storeLocation;
        int pin;
        void setup(); 
    private:
        virtual void handler() = 0;
    };

    class IRFrequency : private AbstractPolledSensor
    {
        friend SensorManager;
    private:
        IRFrequency(void *storeLocation, int pin);
        void setup();
        void read();
    };

    class Switch : AbstractPolledSensor{
        friend SensorManager;        
    private:
        Switch(void *storeLocation, int pin);
        void setup();
        void read();
    };

    class ReflectanceSensor : AbstractPolledSensor{
        friend SensorManager;
    private:
        ReflectanceSensor(void *storeLocation, int pin);
        void setup();
        void read();
    };

    class HallSensor : AbstractPolledSensor{
        friend SensorManager;
    private:
        HallSensor(void *storeLocation, int pin);
        void setup();
        void read();
    };

    class Button : AbstractInterruptSensor{
        Button(void *storeLocation, int pin);
        void setup();
        virtual void handler();
    };

    class Encoder{ //encoders feel the need to be special.
        friend SensorManager;
    public:
        enum class EncoderValue{ENCODER_HIGH, ENCODER_LOW} ;
    private:
        Encoder(void *storeLocation, int pinA, int pinB);
        void setup();
        void handler();

        class EncoderInterrupt : AbstractInterruptSensor{
            friend Encoder;
            EncoderInterrupt(int pin, std::function<void()> upperHandle);
            void setup();
            inline void handler() {};
            std::function<void()> upperHandle;
        } *encoderA;

        class EncoderPoll : AbstractPolledSensor{
            friend Encoder;
            EncoderPoll(Encoder::EncoderValue* storeData, int pin);
            void setup();
            void read();
        } *encoderB;

        void *storeLocation;
        EncoderValue encoderBValue;
    
    };



    class SensorManager
    {
    public:
        SensorManager();
        ~SensorManager();
        void setup();
        void poll();

    private:
        enum polledSensorEnum{
            IR_STRENGTH_LEFT,
            IR_STRENGTH_RIGHT,
            CLAW_LIMIT_SWITCH,
            CLAW_REFLECT,
            CLAW_HALL_EFFECT,
            _LENGTH_POLLED
        };

        enum interruptSensorEnum{
            HMI_1,
            HMI_2,
            HMI_3,
            HMI_4,
            _LENGTH_INTERRUPT
        };

        enum encoderEnum{
            ENCODER_LEFT,
            ENCODER_RIGHT,
            _LENGTH_ENCODER
        };

        AbstractPolledSensor* polledSensors[polledSensorEnum::_LENGTH_POLLED];
        AbstractInterruptSensor* interruptedSensors[interruptSensorEnum::_LENGTH_INTERRUPT];
        Encoder * encoders[encoderEnum::_LENGTH_ENCODER];
    };

}

#endif