#include "BluetoothSerial.h"


BluetoothSerial SerialBT;

void setupSerial(){
    Serial.begin(115200);
};

void setupBluetooth(const char* deviceName) {
    SerialBT.begin(deviceName);  // Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");
};


void sendBluetoothMessageLn(const String& message) {
    SerialBT.println(message);  // Send message via Bluetooth
};

void sendBluetoothMessage(const String& message) {
    SerialBT.print(message);  // Send message via Bluetooth
};


bool BluetoothAvailable(){
    return SerialBT.available();
};

char readBluetoothMessage(){
    return SerialBT.read();
};


void sendLogSpeed(
    float direction_angle,
    float linear_speed_percent,
    long w1,
    long w2,
    long w3
) {

    
    Serial.print("D_A: ");
    Serial.print(direction_angle, 4);
    Serial.print(", L_S_P: ");
    Serial.println(linear_speed_percent, 4);

    
    Serial.print("S_P_S_1: ");
    Serial.print(w1);    
    Serial.print(", S_P_S_2: ");
    Serial.print(w2);
    Serial.print(", S_P_S_3: ");
    Serial.println(w3);

};
