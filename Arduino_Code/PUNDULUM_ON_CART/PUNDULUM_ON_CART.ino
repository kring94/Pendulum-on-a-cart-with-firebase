#include "motor_control_TB6612FNG.h"
#include "angle_control_AS5600.h"

// #include "wifi_instance.h"

///--------------------PID constants-------------------
float angle=0.0;
float kp=15; //Mine 
float ki=15; //Mine
float angle_ki = 3;
float kd=1800; //Mine 1.7789

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
int min_speed = 5000;

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
  checkMagnetPresence();

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

    angle = correctAngle(); //Angulo de posición relativa respecto al punto inicial
    //angle = ReadRawAngle(); //Angulo de posición absoluta respecto al modulo
    if(angle > 296 && angle < 360) angle = -(359.4-angle);
    

    //Serial.println("Angulo: "+ String(angle));

    if(abs(angle)>=20) {
      brake("ON");
    } else {
     // Serial.println("Angulo ABS: "+ String(abs(angle)));
    
      angle_error = angle_setpoint - angle;
      PID_p = angle_error; //**** KP
      angle_diference = angle_error - angle_previous_error;
      PID_d = (angle_error-angle_previous_error)/(period);  //**** KD
      //PID_i = PID_i + (ki * angle_error); //***** KI
       
      if(-angle_ki < angle_diference && angle_error < angle_ki)
      {
        PID_i = PID_i + (period * angle_error*0.001);
      }
      else
      {
        PID_i = 0;
      }

      PID_total = kp*PID_p + ki*PID_i + kd*PID_d;  

      // Serial.print(PID_total);
      // Serial.print(",");  
      
      //Serial.println("PID PRE: " + String(PID_total));   
      
      // PID_total = map(PID_total, -75, 75, 0, speed);  
      if(PID_total>=0){
        PID_total = map(PID_total, 0, 150, 0, speed);        
      }else{
        PID_total = map(PID_total, -150, 0, 0, speed);
      }
      //Serial.println("PID POS: " + String(PID_total));   
      //Serial.println(PID_total);
      
    
      if(PID_total < -speed) PID_total = -speed;
      if(PID_total > speed) PID_total = speed; 
      //Serial.println("H: "); 
      Serial.print(65536);
      Serial.print(",");   
      Serial.print(PID_total);
      Serial.print(",");   
      Serial.println(-65536);
      //Serial.println("PID POS: " + String(PID_total));

      //Serial.println("Error PRE: "+ String(angle_error));
      if(angle_error < -0.51){
        forward(abs(PID_total));
      }else if(angle_error > 0.51){
        backward(abs(PID_total));
      } 
      else if(angle_error > -0.5 && angle_error < 0.5){
        brake("ON");
        PID_total = 0;        
      }
      angle_previous_error = angle_error;
      
      //Serial.println("Error POS: "+ String(angle_error));
    }
    
  }
}

