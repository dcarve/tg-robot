#include "pinOutIn.h"
#include "btCommands.h"
#include <Arduino.h>
#include "serial.h"




void runBtCommands(int command){

    if (command == 1) {
        switchOnReles();
    } else if (command == 2) {
        switchOffReles();
    } else {
        dummyCommand();
    }

};

void dummyCommand(){
    sendBluetoothMessageLn("dummy command");
};

void switchOnReles(){ //on in LOW
    sendBluetoothMessageLn("Switch ON reles");
    Serial.println("Switch ON reles");
    digitalWrite(RELES, LOW);
};

void switchOffReles(){//off in HIGH
    sendBluetoothMessageLn("Switch OFF reles");
    Serial.println("Switch OFF reles");
    digitalWrite(RELES, HIGH);
};