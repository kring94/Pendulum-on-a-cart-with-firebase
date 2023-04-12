// ================================================================ //
//                      For the use of WIFI                     //
//================================================================= //
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Variables for WIFI and Firebase cofiguration
#include "private_settings.h"

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID _WIFI_SSID
#define WIFI_PASSWORD _WIFI_PASSWORD

// Insert Firebase project API Key
#define API_KEY _API_KEY

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL _USER_EMAIL
#define USER_PASSWORD _USER_PASSWORD

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL _DATABASE_URL

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;

String anglePath;
String kiPath;
String kpPath;
String kdPath;
String statePath;
String modePath;
String upPath;
String downPath;
String leftPath;
String rightPath;
String stopPath;

float angle=0.0;
float kp=7; //Mine was 2
float ki=5; //Mine was 0.1
float kd=65536*0.1; //Mine was 15

bool state = false;
bool mode = false;
bool up = false;
bool down = false;
bool left = false;
bool right = false;
bool stop = false;


// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 100;

// Setup function WIFI
void WIFIsetup();

// Initialize WiFi
void initWiFi();

// Write float values to the database
void sendFloat(String path, float value);

// Read data from Database
void readData(String path);
float readNumData(String path);

void WIFIsetup() {
  // Initialize BME280 sensor
  //initBME();
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  anglePath = databasePath + "/angle";
  kiPath = databasePath + "/ki";
  kpPath = databasePath + "/kp";
  kdPath = databasePath + "/kd";
  statePath = databasePath + "/state";
  modePath = databasePath + "/mode";
  upPath = databasePath + "/up";
  downPath = databasePath + "/down";
  leftPath = databasePath + "/left";
  rightPath = databasePath + "/right";
  stopPath = databasePath + "/stop";
}

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Write float values to the database
void sendFloat(String path, float value) {
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)) {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}
// Read data from database
void readData(String path) {
  String readIncoming = "";
  if (Firebase.RTDB.getString(&fbdo, path.c_str())) {
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if (fbdo.dataType() == "string") {
      readIncoming = fbdo.stringData();
      Serial.println("DATA: " + readIncoming);
      if (readIncoming == "ON") {
        if(path == modePath) mode = true;     
        if(path == statePath) state = true;   
        if(path == upPath) up = true;   
        if(path == downPath) down = true;   
        if(path == leftPath) left = true;   
        if(path == rightPath) right = true;   
        if(path == stopPath) stop = true;   
      } else {
        if(path == modePath) mode = false;     
        if(path == statePath) state = false;   
        if(path == upPath) up = false;   
        if(path == downPath) down = false;   
        if(path == leftPath) left = false;   
        if(path == rightPath) right = false;   
        if(path == stopPath) stop = false;            
      }      
    }
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

// Read data from database
float readNumData(String path) {
  float readIncoming = 0.0;
  if (Firebase.RTDB.getString(&fbdo, path.c_str())) {
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
    if (fbdo.dataType() == "double") {
      readIncoming = fbdo.floatData();
      Serial.print("DATA: ");
      Serial.println(readIncoming);
      // if (readIncoming == "ON") {
      //   return true;  //Return based on String ON/OFF
      // } else {
      //   return false;
      // }
      return readIncoming;
    }
  } else {
    return 999.9;
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}
