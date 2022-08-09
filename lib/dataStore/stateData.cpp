#include "stateData.h"

uint8_t StateData::sonarServoPos;

int StateData::sonarSweep[36];

uint8_t StateData::sonarProcessing::idolWidth;
uint8_t StateData::sonarProcessing::idolWidthError;
int StateData::sonarProcessing::backgroundMatchFactorPercent = 80;
int StateData::sonarProcessing::backgroundMatchAmountPercent = 5;

int StateData::sonarProcessing::algorithm = 1 ;
int StateData::sonarHeadingOutput;

MinionDataStore *dataStore = new MinionDataStore();

void setupDataStore() {
    dataStore->lineL = 0;
    dataStore->lineR = 0;
    dataStore->edgeReflectanceFL = 0;
    dataStore->edgeReflectanceFR = 0;
    dataStore->edgeReflectanceBL = 0;
    dataStore->edgeReflectanceBR = 0;
    dataStore->sonarL = 0;
    dataStore->sonarR = 0;
}

