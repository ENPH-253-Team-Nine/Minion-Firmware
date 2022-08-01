#ifndef STATE_DATA_H
#define STATE_DATA_H

#include <stdint.h>


namespace StateData{

    extern uint8_t sonarServoPos;
}

struct MinionDataStore {
    uint16_t lineL;
    uint16_t lineR;

    uint16_t edgeReflectanceFL;
    uint16_t edgeReflectanceFR;
    uint16_t edgeReflectanceBL;
    uint16_t edgeReflectanceBR;

    uint16_t sonarL;
    uint16_t sonarR;
};

void setupDataStore();


#endif