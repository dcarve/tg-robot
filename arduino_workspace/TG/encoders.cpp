#include "encoders.h"

#define HALL_RESOLUTION 341.2

void encodersSetupPins(){

    // encoder, motor 1
    pinMode(PB1, INPUT);  // amarelo
    pinMode(PB0, INPUT);  // verde interrupção do tipo rising

    // encoder, motor 2    
    pinMode(PB11, INPUT);  // amarelo
    pinMode(PB10, INPUT);  // verde interrupção do tipo rising
    
    // encoder, motor 3
    pinMode(PA7, INPUT);  // amarelo
    pinMode(PA6, INPUT);  // verde interrupção do tipo rising

};

void calcEncoder(byte encoder_pin_yellow, volatile int *pos){
    int b = digitalRead(encoder_pin_yellow);
    int increment=0;

    if(b>0){
        increment = 1;
    }
    else {
        increment =-1;
    }
    pos = pos + increment;
}

float calc_rpm(long currT, long prevT, int pos, int posPrev){

    float deltaT = ((float) (currT-prevT))/1.0e6;
    float pulse_per_rotation = (pos - posPrev)/deltaT;
    float rpm = 60*pulse_per_rotation/HALL_RESOLUTION;

    return rpm;
}


float low_pass_filter_first_order(float currRpm, float prevRpm, float prevFilterRpm){

    float posFilterRpm = 0.52188555*prevFilterRpm + 0.23905722*currRpm + 0.23905722*prevRpm;

    return posFilterRpm;
}




