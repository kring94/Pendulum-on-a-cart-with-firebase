// ================================================================ //
//              Library for the use of the module AS5600            //
//================================================================= //
#include <Arduino.h>
#include <Wire.h> //This is for i2C

//---------------------------------------------------------------------------
//Magnetic sensor variables
int magnetStatus = 0; //value of the status register (MD, ML, MH)

int lowbyte; //raw angle 7:0
word highbyte; //raw angle 7:0 and 11:8
int rawAngle; //final raw angle 
float degAngle; //raw angle in degrees (360/4096 * [value between 0-4095])

int quadrantNumber, previousquadrantNumber; //quadrant IDs
float numberofTurns = 0; //number of turns
float correctedAngle = 0; //tared angle - based on the startup value
float startAngle = 0; //starting angle
float totalAngle = 0; //total absolute angular displacement
float previoustotalAngle = 0; //for the display printing

// setup function of module AS5600
void AS5600setup();
// Angle reading function
void ReadRawAngle();
// Angle correction function
void correctAngle();
// Check quadrant function
void checkQuadrant();
// Magnet presence function
void checkMagnetPresence();