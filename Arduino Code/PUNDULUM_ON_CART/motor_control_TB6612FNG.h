// ================================================================ //
//          Library for the use of the module TB6612FNG             //
//================================================================= //
#include <Arduino.h>
// Variables for ESP32 PWM


class Driver
{
private:
	// pins_two_motors[STBY, PWMA, AIN1, AIN2, PWMB, BIN1, BIN2]
	int pins_two_motors[7];
	// pins_one_motor[STBY, PWMA, AIN1, AIN2]
	int pins_one_motor[4];

	boolean state = HIGH;
	//int value_a;
	//int value_b;

public:
	Driver() {}
	Driver(int, int, int, int, int, int, int);
	Driver(int, int, int, int);
	void pins_mode_one_motor();
	void pins_mode_two_motors();
	bool set_state_motor(bool);
	bool get_state_motor();
	void motor_a(int);
	void motor_b(int);
	void motors(int, int);
	void brake(bool, bool, int);
	bool stby_state();
};


// Setup function TB6612FNG for ESP32

void TB6612FNGsetup(int PWMA, int PWMB);