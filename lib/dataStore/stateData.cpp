#include "stateData.h"

uint8_t StateData::sonarServoPos;

MinionDataStore *dataStore;

void setupDataStore() {
    dataStore = new MinionDataStore();

    dataStore->lineL = 0;
    dataStore->lineR = 0;
    dataStore->edgeReflectanceFL = 0;
    dataStore->edgeReflectanceFR = 0;
    dataStore->edgeReflectanceBL = 0;
    dataStore->edgeReflectanceBR = 0;
    dataStore->sonarL = 0;
    dataStore->sonarR = 0;
}

