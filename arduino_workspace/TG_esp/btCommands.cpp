#include "pinOutIn.h"
#include "btCommands.h"
#include <Arduino.h>




void runBtCommands(int command){

    if (command == 1) {
        switchOnReles();
    } else if (command == 2) {
        switchOffReles();
    } else if (command == 3) {
        dummyCommand();
    } else {
        
    }

};

void dummyCommand(){
    Serial.println("dummy command");
};

void switchOnReles(){ //on in LOW
    Serial.println("Switch ON reles");
    digitalWrite(RELES, LOW);
};

void switchOffReles(){//off in HIGH
    Serial.println("Switch OFF reles");
    digitalWrite(RELES, HIGH);
};