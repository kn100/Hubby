#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

const char *SSID = "Your Wifi SSID";
const char *PWD = "Your Wifi Password";
AsyncWebServer server(80);

void toggle()
{
  Serial.println("Button pressed");
  digitalWrite(13, HIGH);
  delay(100);
  Serial.println("Button depressed");
  digitalWrite(13, LOW);
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

int switchToMac()
{
  int macMode = digitalRead(19);
  if (macMode == LOW)
  {
    toggle();
    return 1;
  }
  return 0;
}

int switchToPC()
{
  int macMode = digitalRead(19);
  if (macMode == HIGH)
  {
    Serial.println("Switching to PC mode");
    toggle();
    return 1;
  }
  return 0;
}

String currentMode()
{
  int macMode = digitalRead(19);
  if (macMode == HIGH)
  {
    return "Mac";
  }
  return "PC";
}

void connectToWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PWD);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup(){
  
  Serial.begin(9600);
  delay(3000);
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  // Toggle for usb switch
  pinMode(13, OUTPUT);
  // Mac LED
  pinMode(19, INPUT);
  connectToWiFi();

  server.on("/mac", HTTP_GET, [](AsyncWebServerRequest *request){ 
    if (switchToMac() == 1)
    {
      // http 200 okay
      request->send(200, "text/plain", "OK");
    } else 
    {
      request->send(409, "text/plain", "already in mac mode");
    }
  });

  server.on("/pc", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Switching to PC mode");
    if (switchToPC() == 1)
    {
      // http 200 okay
      request->send(200, "text/plain", "OK");
    } else 
    {
      request->send(409, "text/plain", "already in pc mode");
    }
  });

  server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request){ 
    request->send(200, "text/plain", currentMode());
  });

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.onNotFound(notFound);

  server.begin();
}

void loop()
{
  while (true)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      connectToWiFi();
    }
  }
}