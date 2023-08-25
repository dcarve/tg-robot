#include "encoders.h"

#define HALL_RESOLUTION 341.2

void encodersSetupPins(){

    //encoder, motor 1
    pinMode(PB1, INPUT);  // amarelo
    pinMode(PB0, INPUT);  // verde

    //encoder, motor 2    
    pinMode(PB11, INPUT);  // amarelo
    pinMode(PB10, INPUT);  // verde
    
    //encoder, motor 3
    pinMode(PA7, INPUT);  // amarelo
    pinMode(PA6, INPUT);  // verde

};

int readEncoderCalc(int pos_i){
    int b = digitalRead(PB1);
    if(b>0){
        pos_i++;
    }
    else {
        pos_i--;
    }    
    return pos_i;
}

float calc_rpm(long currT, long prevT, int pos, int posPrev){

    float deltaT = ((float) (currT-prevT))/1.0e6;
    float pulse_per_rotation = (pos - posPrev)/deltaT;
    float rpm = 60*pulse_per_rotation/HALL_RESOLUTION;

    return rpm;
}


