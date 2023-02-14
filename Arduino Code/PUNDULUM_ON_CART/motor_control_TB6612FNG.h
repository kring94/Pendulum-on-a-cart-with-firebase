// ================================================================ //
//                  for use of the module TB6612FNG                 //
//================================================================= //
#include <Arduino.h>
// pins definition
//Motor A
const int AIN1 = 23;
const int AIN2 = 17;
const int PWMA = 16;
const int BIN1 = 19;
const int BIN2 = 18;
const int PWMB = 5;
const int STBYA = 15;
//Motor B
const int AIN11 = 14;
const int AIN22 = 33;
const int PWMAA = 32;
const int BIN11 = 25;
const int BIN22 = 26;
const int PWMBB = 27;
const int STBYB = 13;

const int frecuencia = 5000;
const int resolution = 16;  //65,536 leves for pwm

void TB6612Setup();
void motorA1(String direction, int speed);
void motorA2(String direction, int speed);
void motorB1(String direction, int speed);
void motorB2(String direction, int speed);
void brake(String brake);

void forward(int maxSpeed);
void backward(int maxSpeed);
void toLeft(int maxSpeed);
void toRight(int maxSpeed);


void TB6612Setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBYA, OUTPUT);
  ledcSetup(0, frecuencia, resolution);
  ledcAttachPin(PWMA, 0);
  ledcSetup(0, frecuencia, resolution);
  ledcAttachPin(PWMB, 0);

  pinMode(AIN11, OUTPUT);
  pinMode(AIN22, OUTPUT);
  pinMode(BIN11, OUTPUT);
  pinMode(BIN22, OUTPUT);
  pinMode(STBYB, OUTPUT);
  ledcSetup(0, frecuencia, resolution);
  ledcAttachPin(PWMAA, 0);
  ledcSetup(0, frecuencia, resolution);
  ledcAttachPin(PWMBB, 0);
}
//Forward engines
void motorA1(String direction, int speed) {
  if (direction == "forward") {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(0, speed);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(0, speed);
  }
}

void motorA2(String direction, int speed) {
  if (direction == "forward") {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(0, speed);
  } else {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(0, speed);
  }
}

//Back engines
void motorB1(String direction, int speed) {
  if (direction == "forward") {

    digitalWrite(AIN11, HIGH);
    digitalWrite(AIN22, LOW);
    ledcWrite(0, speed);
  } else {
    digitalWrite(AIN11, LOW);
    digitalWrite(AIN22, HIGH);
    ledcWrite(0, speed);
  }
}

void motorB2(String direction, int speed) {
  if (direction == "forward") {
    digitalWrite(BIN11, HIGH);
    digitalWrite(BIN22, LOW);
    ledcWrite(0, speed);
  } else {
    digitalWrite(BIN11, LOW);
    digitalWrite(BIN22, HIGH);
    ledcWrite(0, speed);
  }
}

//Brake
void brake(String brake) {
  if(brake=="ON") {
    digitalWrite(STBYA, LOW);
    digitalWrite(STBYB, LOW);

    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(AIN11, LOW);
    digitalWrite(AIN22, LOW);

    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    digitalWrite(BIN11, LOW);
    digitalWrite(BIN22, LOW);
  }else if(brake=="OFF") {
    digitalWrite(STBYA, HIGH);
    digitalWrite(STBYB, HIGH);    
  }
  
}

// displacement of the vehicle

// fordward

void forward(int speed) {
  brake("OFF");
  motorA1("forward", speed);
  motorA2("forward", speed);
  motorB1("forward", speed);
  motorB2("forward", speed);
}

// backward
void backward(int speed) {
  brake("OFF");
  motorA1("backward", speed);
  motorA2("backward", speed);
  motorB1("backward", speed);
  motorB2("backward", speed);
  
}
// left

void toLeft(int speed) {
  brake("OFF");
  motorA1("backward", speed);
  motorA2("backward", speed);
  motorB1("forward", speed);
  motorB2("forward", speed);
}

// right

void toRight(int speed) {
  brake("OFF");
  motorA1("forward", speed);
  motorA2("forward", speed);
  motorB1("backward", speed);
  motorB2("backward", speed);  
}
