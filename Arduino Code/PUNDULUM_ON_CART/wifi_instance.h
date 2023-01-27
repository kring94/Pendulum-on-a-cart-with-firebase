// ================================================================ //
//                      For the use of the WIFI                     //
//================================================================= //
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Variables for WIFI and Firebase cofiguration


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "ADVANCED"
#define WIFI_PASSWORD "3115770876ANTO"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAAYc4rOWxmwlEGmUe8Li8h4Cl__GDfayY"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "ronald9412@gmail.com"
#define USER_PASSWORD "15446941204"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://esp32-pfcd-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String tempPath;
String humPath;
String presPath;
String ledPath;

float temperature;
float humidity;
float pressure;
const int led = 5;//led
const int potPin = 34;//analogo read
int potValue = 0;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 50;

// Setup function WIFI
void WIFIsetup();

// Initialize WiFi
void initWiFi();

// Write float values to the database
void sendFloat(String path, float value);

// Read data from Database
void readData(String path);
