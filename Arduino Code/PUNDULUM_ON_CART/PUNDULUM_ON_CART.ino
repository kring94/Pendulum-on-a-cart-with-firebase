#include "motor_control_TB6612FNG.h"
#include "angle_control_AS5600.h"
#include "wifi_instance.h"
// #include "bluetooth_instance.h"

///--------------------PID constants-------------------

float angle_setpoint = 61;         
float PID_p, PID_i, PID_d, PID_total;
//------------------------------------------------------

//---------------------Variables------------------------
int Read = 0;
float elapsedTime, time, timePrev;        //Variables for time control
float angle_previous_error, angle_error, angle_diference;
int period = 50;  //Refresh rate period of the loop is 50ms
//------------------------------------------------------

int speed = 65536;

void moving();
void PIDcontrol();

void setup() {
  // Serial.begin(115200);
  WIFIsetup();
  AS5600setup();
  TB6612Setup();

  time = millis();
}

void loop() {
  ReadRawAngle();
  checkQuadrant();  

  // // if (Firebase.ready()){
  // if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
  //   sendDataPrevMillis = millis();

  //   sendFloat(anglePath, angle);
  //   readData(modePath);
  //   readData(statePath);

  //   moving();
  // }



}
// ----------------
void moving() {
  readData(upPath);
  readData(downPath);
  readData(leftPath);
  readData(rightPath);
  readData(stopPath);
  if (stop) {
    brake();
  } else if (up) {
    forward(speed);
  } else if (down) {
    backward(speed);
  } else if (left) {
    toLeft(speed);
  } else if (right) {
    toRight(speed);
  }
}
// -----------------
void PIDcontrol() {
  
  if (millis() > time+period){
    angle = correctAngle();
    angle_error = angle_setpoint - angle;
    PID_p = kp * angle_error; //**** KP
    angle_diference = angle_error - angle_previous_error;
    PID_d  kd*((angle_error-angle_previous_error)/period);  //**** KD
    if(-3 < angle_diference && angle_error < 3)
    {
      PID_i = PID_i + (ki * angle_error);
    }
    else
    {
      PID_i = 0;
    }

    PID_total = PID_p + PID_i + PID_d;  
    PID_total = map(PID_total, -150, 150, 0, 150);
  
    if(PID_total < 20){PID_total = 20;}
    if(PID_total > 160) {PID_total = 160; } 
  
    // myservo.write(PID_total+30);  
    distance_previous_error = distance_error;
    Serial.println(PID_total)
    
  {
}
