#include "motor_control_TB6612FNG.h"
#include "angle_control_AS5600.h"
#include "wifi_instance.h"
// #include "bluetooth_instance.h"




void setup() {
  Serial.begin(115200);
  WIFIsetup();
  AS5600setup();

}

void loop() {
  ReadRawAngle();
  checkQuadrant();
  angle = correctAngle();

  if(Firebase.ready()){
    sendFloat(anglePath,angle);
    mode = readData(modePath);
    state = readData(statePath);
    
  }
}
