#include "pinOutIn.h"
#include "btCommands.h"
#include <Arduino.h>


void dummyCommand(){
    Serial.println("dummy command");
};

void runBtCommands(int command){

    if (command == 1) {
        Serial.println("Switch ON reles");
        switchOnReles();
    } else if (command == 2) {
        switchOffReles();
    } else if (command == 3) {
        dummyCommand();
    } else {
        
    }

};

void switchOnReles(){ //on in LOW
    digitalWrite(RELES, LOW);
};

void switchOffReles(){//off in HIGH
    digitalWrite(RELES, HIGH);
};