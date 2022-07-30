// boilerplate include safety
#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

// includes
#include <Arduino.h>
#include <stateData.h>

namespace lights
{
    class LightManager; // forward-declared

    /**
     * Represents a specific physical light
     *
     * @author Eric Souder <eric@souder.ca>
     */
    class Light
    {
        friend LightManager; // allow the manager to see it's private parts.

    public:
        /**
         * Enum of possible states for a light to be in.
         */
        enum class LightState
        {
            ON,
            OFF
        };

    protected:
        /**
         * Construct a new Light object
         *
         * @param pin Arduino pin the light is attached to
         * @param inverted If setting the pin high or low turn the light on
         */
        Light(int pin, bool inverted);

        /**
         * Construct a new Light object
         *
         * @param pin Arduino pin the light is attached to
         */
        Light(int pin);

        /**
         * Perform the arduno setup of a light. Pin assignments, etc.
         */
        void setup();

        /**
         * Set the state of a light object.
         * @see LightState
         * @param newState
         */
        void setLightState(Light::LightState newState);

        /**
         * Get the state of the light.
         *
         * @return Lights::Light::LightState
         */
        Light::LightState getLightState();

    private:
        int pin;              // physical pin for Arduino use
        LightState state;     // Current state of the light
        bool inverted;        // Whether setting the pin to LOW makes the light turn off or on
        uint32_t lastChanged; // When the light was last turned on or off
    };

    /**
     * Manages a collection of light objects.
     *
     * @author Eric Souder <eric@souder.ca>
     */
    class LightManager
    {
    public:
        /**
         * Construct a new Light Manager.
         */
        LightManager();

        /**
         * Destroy the Light Manager
         */
        ~LightManager();

        /**
         * Perform the once-per-arduino-loop 'poll' of the light manager,
         * updating the states of the lights based on internal logic and
         * reporting any approriate data to the approriate places.
         */
        void poll();

        /**
         * Perform the Arduino setup functions for the lights in the manager's
         * charge.
         *
         * @see Lights::Light#setup
         */
        void setup();

    private:
        enum EnumLights
        {
            BUILTIN_LED,
            _LENGTH // this is a nice (weird?) hack to easily get the lenth of the enum
                    // as long as this _LENGTH is at the end of the enum and everything is
                    // initialzed in default order, _LENGTH will always have the value
                    // of the length of the enum.
        };
        Light *lights[EnumLights::_LENGTH];        // Array of light objects to manage

        //state-specific light control subroutines

        void blink(uint16_t period, Light *light); // Make a light blink
        void beOn(Light *light);                   // Make a light be.. on
        void beOff(Light *light);                  // Make a light be off
    };
}

#endif