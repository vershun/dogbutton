#include <Arduino.h>
#include <WiFiClientSecure.h>
#include "twilio.hpp"

// WiFi SSID
const char *ssid = "";
// WiFi Password
const char *password = "";

// Twilio's root CA fingerprint. This will change.
const char *fingerprint = "72 1C 17 31 85 E2 7E 0D F9 D4 C2 5B A0 0E BD B7 E2 06 26 ED";

// Twilio account information
const char *accountSID = "";
const char *authToken = "";
String fromNumber = "";
String targetNumber = ""
String message = "Woof! I want outside!";

Twilio *twilio;

RTC_DATA_ATTR bool firstBoot = true;

void sendNotification(const String& target) {
  String response;
  bool success = twilio->send_message(
          target,
          fromNumber,
          message,
          response,
          ""
        );
  if(!success) {
    Serial.println("Failed to send twilio message");
    Serial.println(response);
  }
}

void sleep() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_4, HIGH);
  Serial.println("Sleeping...");
  delay(200);
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(9800);
  if(firstBoot) {
    firstBoot = false;
    sleep();
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("Connected with IP: %s\n", WiFi.localIP());
  twilio = new Twilio(accountSID, authToken, fingerprint);
  Serial.println("Sending notification");
  sendNotification(targetNumber);
  Serial.println("Pausing for 20 seconds...");
  delay(20000);
  sleep();
}

void loop() {}
