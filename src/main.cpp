#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

char * endpoint = "https://ecse-three-led-api-v2.onrender.com/api/state";

//Initializing pins
const int ledPin1 = 8;
const int ledPin2 = 7;
const int ledPin3 = 6;

//2D array 8 by 3 with the truth table sequence
bool led_sequence [8][3]= { 
  {false,false,false},        
  {false,false,true },
  {false,true,false},
  {false,true,true },
  {true,false,false},
  {true,false,true },
  {true,true,false},
  {true,true,true },
};
void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS); //Wifi Name & Password linked to env.h file
  
// Connecting waiting. Will show fullstops till connected
  Serial.println("Connecting");
  while (WiFi.status()!= WL_CONNECTED){
    delay (550);
    Serial.print(".");
  }

// If connected, shows successful connection with the IP address
  Serial.print("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println (WiFi.localIP());
}

void loop() {
  // Check Wifi connectivity status is successful
  if (WiFi.status()== WL_CONNECTED){
    HTTPClient http;

    String http_response; //Store the HTTP response code
    http.begin(endpoint); //Starts connection with the server                             
    

    //Start of For Loop
    for (int i = 0; i < 8; i++){
    //Unique API Key linked in the env.h File
    http.addHeader("X-API-Key", API_KEY);  
    //JSON Document
    StaticJsonDocument<1024> doc;                     
    doc["light_switch_1"] = led_sequence[i][0];
    doc["light_switch_2"] = led_sequence[i][1];
    doc["light_switch_3"] = led_sequence[i][2];

    digitalWrite(ledPin1, led_sequence[i][0]);
    digitalWrite(ledPin2, led_sequence[i][1]);
    digitalWrite(ledPin3, led_sequence[i][2]);


    String httpRequest; 
    serializeJson(doc,httpRequest);
    //Send PUT request
    int httpResponseCode = http.PUT(httpRequest);  
                                      
    if (httpResponseCode > 0){
      Serial.print("HTTP Response Code:  ");
      Serial.println(httpResponseCode);

      Serial.print("Response from server: ");
      http_response = http.getString(); //this is getting a response from the server
      Serial.println(http_response);
    }
    else {
      Serial.println("Failed to connect to server");
      Serial.print("Error Code:  ");
      Serial.print(httpResponseCode);
    }
    delay(2000);
    } //End of For Loop

    //Ending HTTP connection
    http.end();
  }
  else{return;}
}