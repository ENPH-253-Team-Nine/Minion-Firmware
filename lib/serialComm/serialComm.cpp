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

    // Testing:

    // StateData::lineL += 1;
    // StateData::lineR += 2;

    // StateData::edgeReflectanceFL += 3;
    // StateData::edgeReflectanceFR += 4;
    // StateData::edgeReflectanceBL += 5;
    // StateData::edgeReflectanceBR -= 1;
    
    // StateData::sonarObjectAngle = 90;
}

void SerialManager::receiveCommands() {
    while(true) {
        int receivedByte = Serial.read();
        StateData::receivedByte = receivedByte;

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
    DataPacket *packet = new DataPacket(); 

    packet->lineL = StateData::lineL;
    packet->lineR = StateData::lineR;
    packet->edgeReflectanceFL = StateData::edgeReflectanceFL;
    packet->edgeReflectanceFR = StateData::edgeReflectanceFR;
    packet->edgeReflectanceBL = StateData::edgeReflectanceBL;
    packet->edgeReflectanceBR = StateData::edgeReflectanceBR;

    const char * sendBuf = (const char *)packet;

    Serial1.write(sendBuf, sizeof(*packet));
}