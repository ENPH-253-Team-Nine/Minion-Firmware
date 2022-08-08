#include "serialComm.h"

using namespace serialComm;

SerialManager::SerialManager() {}

void SerialManager::setup() {
    Serial1.setRx(PB7);
    Serial1.setTx(PB6);
    Serial1.begin(9600);
}

void SerialManager::poll() {
    receiveCommands();
    sendData();
}

void SerialManager::receiveCommands() {
    while(true) {
        int receivedByte = Serial.read();

        switch (receivedByte) {
            case DEPLOY_BRIDGE:
                break;
            case SWEEP_OFF:
                break;
            case SWEEP_ON:
                break;
            default:
                // Covers receivedByte = -1 (no data) as well as garbage cases.
                return;
        }
    }
}

void SerialManager::sendData() {
    const char * sendBuf = (const char *)dataStore;
    Serial1.write(sendBuf, sizeof(*dataStore));
}