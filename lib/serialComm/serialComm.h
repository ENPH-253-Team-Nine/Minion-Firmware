#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <Arduino.h>

#include <stateData.h>

namespace serialComm {

    // SHOULD MATCH THE FORMAT OF DataPacket in Master-Firmware exactly!
    struct DataPacket {
        uint16_t lineL;
        uint16_t lineR;

        uint16_t edgeReflectanceFL;
        uint16_t edgeReflectanceFR;
        uint16_t edgeReflectanceBL;
        uint16_t edgeReflectanceBR;

        // Minion determines relative angle to target; target depends on robot state.
        // Examples: treasure if in treasure hunting states, arch if in arch state.
        int16_t sonarObjectAngle; 

    };  

    class SerialManager 
    {
    public:
        /**
         * @brief Construct a new Serial Manager object
         * 
         */
        SerialManager();

        /**
         * @brief Destroy the Serial Manager object
         * 
         */
        ~SerialManager();

        /**
         * @brief Perform the Arduino setup functions for the lights in the manager's
         * 
         */
        void setup();

        /**
         * @brief Perform the once-per-arduino-loop 'poll' of the light manager,
         * updating the states of the lights based on internal logic and
         * reporting any approriate data to the approriate places.
         */
        void poll();

    private:
        /**
         * @brief Send commands to minion.
         * 
         */
        void receiveCommands();

        /**
         * @brief Receive data from minion.
         * 
         */
        void sendData();


        enum SerialCommands {
            DEPLOY_BRIDGE,
            SWEEP_ON,
            SWEEP_OFF,
            _LENGTH
        } serialCommands;

    };

}

#endif
