#ifndef STATE_DATA_H
#define STATE_DATA_H

#include <stdint.h>


namespace StateData{

    extern uint8_t sonarServoPos;
    namespace sonarProcessing{
        extern uint8_t idolWidth;
        extern uint8_t idolWidthError;
        extern int backgroundMatchFactorPercent;
        extern int backgroundMatchAmountPercent;
        extern int algorithm;
    }

    extern int sonarSweep[36];
    extern int sonarHeadingOutput;
}


    extern uint16_t lineL;
    extern uint16_t lineR;

    extern uint16_t edgeReflectanceFL;
    extern uint16_t edgeReflectanceFR;
    extern uint16_t edgeReflectanceBL;
    extern uint16_t edgeReflectanceBR;

    extern int16_t sonarObjectAngle;

    extern int receivedByte;
}

#endif