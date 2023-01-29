#include "motor_control_TB6612FNG.h"
#include "angle_control_AS5600.h"
// #include "wifi_instance.h"
// #include "bluetooth_instance.h"

///--------------------PID constants-------------------
float angle=0.0;
float kp=15; //Mine 
float ki=0.405; //Mine
float kd=1780; //Mine 

float angle_setpoint = 0;         
float PID_p, PID_i, PID_d, PID_total;
//------------------------------------------------------

//---------------------Variables------------------------
int Read = 0;
float elapsedTime, currentTime, timePrev;        //Variables for time control
float angle_previous_error, angle_error, angle_diference;
int period = 50;  //Refresh rate period of the loop is 50ms
//------------------------------------------------------

int speed = 65536;
int startSpeed = 0;

void moving();
void PIDcontrol();

void setup() {
  Serial.begin(115200);
  //WIFIsetup();
  AS5600setup();
  TB6612Setup();

  currentTime = millis();
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
  PIDcontrol();


}
// ----------------
// void moving() {
//   readData(upPath);
//   readData(downPath);
//   readData(leftPath);
//   readData(rightPath);
//   readData(stopPath);
//   if (stop) {
//     brake();
//   } else if (up) {
//     forward(speed);
//   } else if (down) {
//     backward(speed);
//   } else if (left) {
//     toLeft(speed);
//   } else if (right) {
//     toRight(speed);
//   }
// }
// -----------------
void PIDcontrol() {
  
  if (millis() > currentTime+period){

    angle = correctAngle();
    if(angle > 298 && angle < 360) angle = map(angle,360,298,0,-60);
    // else if(angle > 0 && angle < 60){
    //   angle = map(angle,0,60,0,-60);
    // }
    Serial.println("Angulo: "+String(angle));
    angle_error = angle_setpoint - angle;
    PID_p = kp * angle_error; //**** KP
    angle_diference = angle_error - angle_previous_error;
    PID_d = kd*((angle_error-angle_previous_error)/period);  //**** KD
    if(-3 < angle_diference && angle_error < 3)
    {
      PID_i = PID_i + (ki * angle_error);
    }
    else
    {
      PID_i = 0;
    }

    PID_total = PID_p + PID_i + PID_d;  
    
    Serial.println("PID PRE: " + String(PID_total));    
    PID_total = map(PID_total, 0, 100, 0, speed-startSpeed);
    
  
    if(PID_total < -speed) PID_total = -speed;
    if(PID_total > speed) PID_total = speed; 
    Serial.println("PID POS: " + String(PID_total));


    Serial.println("Error PRE: "+ String(angle_error));
    if(angle_error < -0.51){
      forward(abs(PID_total)+startSpeed);
    }else if(angle_error > 0.51){
      backward(abs(PID_total)+startSpeed);
    } 
    else if( (angle_error > -0.5 && angle_error < 0.5) || angle_error < -55 || angle_error > 55){
      brake();
    }
    
    angle_previous_error = angle_error;
    
    Serial.println("Error POS: "+ String(angle_error));
    
  }
}
