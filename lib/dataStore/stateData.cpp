#include "stateData.h"

uint8_t StateData::sonarServoPos;

int StateData::sonarSweep[36];

uint8_t StateData::sonarProcessing::idolWidth;
uint8_t StateData::sonarProcessing::idolWidthError;
int StateData::sonarProcessing::backgroundMatchFactorPercent = 80;
int StateData::sonarProcessing::backgroundMatchAmountPercent = 5;

int StateData::sonarProcessing::algorithm = 1 ;
int StateData::sonarHeadingOutput;

uint16_t StateData::lineL;
uint16_t StateData::lineR;
uint16_t StateData::edgeReflectanceFL;
uint16_t StateData::edgeReflectanceFR;
uint16_t StateData::edgeReflectanceBL;
uint16_t StateData::edgeReflectanceBR;
int16_t StateData::sonarObjectAngle;

int StateData::receivedByte;