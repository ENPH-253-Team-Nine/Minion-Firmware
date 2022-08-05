#include <servoControl.h>

using namespace servos;

ServoManager::ServoManager()
{
    sonarServo = new Servo();
}

ServoManager::~ServoManager()
{
}

void ServoManager::setup()
{

    sonarServo->attach(PA6,500,2200,0);
}

void ServoManager::poll()
{
    // right now this only controls servo angle and not speed.
    // we can control speed but it will be some more work and more code.

    sonarServo->write(StateData::sonarServoPos);
}