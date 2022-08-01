#include "lights.h"

using namespace lights;
/*Light manager class - one instance ever */

LightManager::LightManager()
{
    lights[EnumLights::BUILTIN_LED] = new Light(LED_BUILTIN, true);
}

LightManager::~LightManager() // pretty boilerplate
{
    for (Light *light : lights)
    {
        delete light;
    }
}

void LightManager::poll()
{
    for (Light *light : lights)
    {
        int x = 0;
        // determine some other way to run the lights without the state machine
    }
}


void LightManager::setup()
{
    for (Light *light : lights)
    {
        light->setup();
    }
}

/* State-dependant functionalities*/

void LightManager::blink(uint16_t period, Light *light)
{
    long int curTime = millis();
    // if more time has elapsed since you changed than the length of the period, toggle the light.
    if (curTime >= light->lastChanged + period)
    {
        // this breaks if you have more than just on/off states and is probably bad practice, but...
        // it's kinda cool and onelining it is the lazy, eric approved approach
        light->setLightState(static_cast<Light::LightState>(!static_cast<bool>(light->getLightState())));
    }
}

void LightManager::beOn(Light *light)
{
    if (!(light->getLightState() == Light::LightState::ON))
    {
        light->setLightState(Light::LightState::ON);
    }
}

void LightManager::beOff(Light *light)
{
    if (!(light->getLightState() == Light::LightState::OFF))
    {
        light->setLightState(Light::LightState::OFF);
    }
}

/** Light class**/
/* One instance of this class per physical LED*/

Light::Light(int pin, bool inverted)
{
    this->pin = pin;
    this->inverted = inverted;
    this->state = Light::LightState::OFF;
}

Light::Light(int pin)
{
    this->pin = pin;
    this->inverted = false;
}

void Light::setLightState(LightState newState)
{
    state = newState;
    switch (state)
    {
    case Light::LightState::ON:
        digitalWrite(pin, !inverted);
        break;

    case Light::LightState::OFF:
        digitalWrite(pin, inverted);
        break;
    }
    lastChanged = millis();
}

// I am aware that reading something like this makes the word 'light' look not real
// It's called 'semantic satiation' (https://en.wikipedia.org/wiki/Semantic_satiation)
// but I promise being organized like this will actually be less confusing in the
// long run.
Light::LightState Light::getLightState()
// Decoded:
//          Light - the Light class
//               ::LightState - this class will return something from the 'LightState' enum
//                            Lights - the namespace again
//                                  ::Light - the Light class again
//                                         ::getLightState() - the actual name of this function
{
    return state;
}

void Light::setup()
{
    pinMode(pin, OUTPUT);
}
