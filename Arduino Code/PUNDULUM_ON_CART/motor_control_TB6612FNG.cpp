#include "motor_control_TB6612FNG.h"

void TB6612FNGsetup(int PWMA, int PWMB) {
  ledcSetup(0, 5000, 8);
  ledcAttachPin(PWMA, 0);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(PWMB, 0);  
}

//Constructor for to start one motor
Driver::Driver(int stby, int pwma, int ain1, int ain2)
{
	pins_one_motor[0] = stby;
	pins_one_motor[1] = pwma;
	pins_one_motor[2] = ain1;
	pins_one_motor[3] = ain2;
}
//Constructor for to start two motors
Driver::Driver(int stby, int pwma, int ain1, int ain2, int pwmb, int bin1, int bin2)
{
	pins_two_motors[0] = stby;
	pins_two_motors[1] = pwma;
	pins_two_motors[2] = ain1;
	pins_two_motors[3] = ain2;
	pins_two_motors[4] = pwmb;
	pins_two_motors[5] = bin1;
	pins_two_motors[6] = bin2;

}

//Change state motors 
bool Driver::set_state_motor(bool state)
{
	this->state = state;
}

//Return state motors
bool Driver::get_state_motor()
{
	return state;
}

//pin initializer for one motors
void Driver::pins_mode_one_motor()
{
	pinMode(pins_one_motor[0], OUTPUT);
	pinMode(pins_one_motor[1], OUTPUT);
	pinMode(pins_one_motor[2], OUTPUT);
	pinMode(pins_one_motor[3], OUTPUT);

}
//pin initializer for two motors
void Driver::pins_mode_two_motors()
{
	pinMode(pins_two_motors[0], OUTPUT);
	pinMode(pins_two_motors[1], OUTPUT);
	pinMode(pins_two_motors[2], OUTPUT);
	pinMode(pins_two_motors[3], OUTPUT);
	pinMode(pins_two_motors[4], OUTPUT);
	pinMode(pins_two_motors[5], OUTPUT);
	pinMode(pins_two_motors[6], OUTPUT);
}
// Function motor a
void Driver::motor_a(int value_a)
{

	if (value_a >= 0)
	{
		digitalWrite(pins_two_motors[2], HIGH);
		digitalWrite(pins_two_motors[3], LOW);
	}
	else
	{
		digitalWrite(pins_two_motors[2], LOW);
		digitalWrite(pins_two_motors[3], HIGH);
		value_a *= -1;
	}
	//analogWrite(pins_two_motors[1], value_a);
  ledcWrite(0, value_a);
}
// Function motor b
void Driver::motor_b(int value_b)
{

	if (value_b >= 0)
	{
		digitalWrite(pins_two_motors[5], HIGH);
		digitalWrite(pins_two_motors[6], LOW);
	}
	else
	{
		digitalWrite(pins_two_motors[5], LOW);
		digitalWrite(pins_two_motors[6], HIGH);
		value_b *= -1;
	}
	//analogWrite(pins_two_motors[4], value_b);
  ledcWrite(1, value_b);
}
// Function two motors
void Driver::motors(int value_a, int value_b)
{
	digitalWrite(pins_two_motors[0], get_state_motor());
	motor_a(value_a);
	motor_b(value_b);
}
// Brake function
void Driver::brake(bool value_a, bool value_b, int value)
{
	digitalWrite(pins_two_motors[0], HIGH);
	if (value_b)
	{
		digitalWrite(pins_two_motors[5], HIGH);
		digitalWrite(pins_two_motors[6], HIGH);
		//analogWrite(pins_two_motors[4], value);
    ledcWrite(1, value);
	}
	if (value_a)
	{
		digitalWrite(pins_two_motors[2], HIGH);
		digitalWrite(pins_two_motors[3], HIGH);
		//analogWrite(pins_two_motors[1], value);
    ledcWrite(0, value);
	}
}

bool Driver::stby_state()
{
	return digitalRead(pins_two_motors[0]);
}