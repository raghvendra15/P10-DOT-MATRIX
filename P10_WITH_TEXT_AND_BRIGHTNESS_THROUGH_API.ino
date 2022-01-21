#include <DMDESP.h>
#include <fonts/ElektronMart6x8.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "SystemFont5x7.h"
#include "Arial_black_16.h"

const char* ssid = "PLATFORUMA";
const char* password = "chetna@123";
const char* host = "https://ibsbus.herokuapp.com/p10api/";
const char* fingerprint = "94 FC F6 23 6C 37 D5 E7 92 78 3C 0B 5F AD 04 9E FD 9E A8";
const char* httpsPort = "443";
//----------------------------------------

//----------------------------------------DMD Configuration (P10 Panel)
const int WIDTH = 2;
const int HEIGHT = 1;
const uint8_t *FONT = Arial_Black_16;

char Text[] = {"RASHAIL TECH LABS PRIVATE LIMITED"};

DMDESP dmd1( WIDTH, HEIGHT);  //--> Number of Panels P10 used (Column, Row)
//----------------------------------------

//========================================================================VOID SETUP()
void setup() {
  //----------------------------------------DMDESP Setup
  dmd1.start(); //--> Run the DMDESP library
  dmd1.setFont(FONT); //--> Determine the font used
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());

  WiFiClientSecure client;
  client.setInsecure();
  client.connect(host, 443);
  HTTPClient http; //Object of class HTTPClient
  http.begin(client, host);

  int httpCodeResponse = http.GET();
  Serial.print("Response Code:  ");
  Serial.println(httpCodeResponse);
  if (httpCodeResponse > 0)
  {
    //    const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, http.getString());

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    JsonObject root_0 = doc[0];
    int root_0_id = root_0["id"]; // 1
    const char* root_0_device_name = root_0["device_name"]; // "P10"
    String root_0_device_message = root_0["device_message"]; // "Rashail Tech Labs PVT LTD."
    int root_0_device_brightness = root_0["device_brightness"]; // 200
    int root_0_device_scrolling_speed = root_0["device_scrolling_speed"]; // 40
    Serial.print("device_name");
    Serial.println(root_0_device_name);
    Serial.print("device_message");
    Serial.println(root_0_device_message);
    Serial.print("device_scrolling_speed");
    Serial.println( root_0_device_scrolling_speed);
    Serial.print("device_brightness");
    Serial.println(root_0_device_brightness);
    //STORE IN CONST CHAR
    // STRING TO CONST CHAR
    root_0_device_message.toCharArray(Text, 500);
    dmd1.setBrightness(root_0_device_brightness); //--> Brightness level
   
  }
  http.end(); //Close connection
  delay(5000);
  Serial.println("Magic");
}

void loop() {
  dmd1.drawLine(0, 0, 63, 0); //TOP
  dmd1.drawLine(0, 15, 63, 15); // BOTTOM
  dmd1.drawLine(0, 0, 0, 15);   // LEFT
  dmd1.drawLine(63, 0, 63, 15);  //RIGHT
  dmd1.loop(); //--> Run "Disp.loop" to refresh the LED

  Scrolling_Text(1, 35 ); //--> Show running text "Scrolling_Text(y position, speed);"
}

void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;
  int width = dmd1.width();
  dmd1.setFont(FONT);
  int fullScroll = dmd1.textWidth(Text) + width;
  if ((millis() - pM) > scrolling_speed) {
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    dmd1.drawText(width - x, y, Text);
  }
}

void Fetch_data() {
  WiFiClientSecure client;
  client.setInsecure();
  client.connect(host, 443);
  HTTPClient http; //Object of class HTTPClient
  http.begin(client, host);

  int httpCodeResponse = http.GET();
  Serial.print("Response Code:  ");
  Serial.println(httpCodeResponse);
  if (httpCodeResponse > 0)
  {
    //    const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, http.getString());

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    JsonObject root_0 = doc[0];
    int root_0_id = root_0["id"]; // 1
    const char* root_0_device_name = root_0["device_name"]; // "P10"
    const char* root_0_device_message = root_0["device_message"]; // "Rashail Tech Labs PVT LTD."
    int root_0_device_brightness = root_0["device_brightness"]; // 200
    int root_0_device_scrolling_speed = root_0["device_scrolling_speed"]; // 40
    Serial.print("device_name");
    Serial.println(root_0_device_name);
    Serial.print("device_message");
    Serial.println(root_0_device_message);
    Serial.print("device_brightness");
    Serial.println(root_0_device_brightness);
    Serial.print("device_scrolling_speed");
    Serial.println( root_0_device_scrolling_speed);
  }
  http.end(); //Close connection
  delay(1000);
}
