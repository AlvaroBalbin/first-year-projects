#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "UoB-IoT";
const char* password = "c7ii7fzw"; // <- Replace this
const char* weatherUrl = "https://people.bath.ac.uk/lj386/PW/PW4/weatherapi.json";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void loop() {
  getJSON(weatherUrl);
  delay(60000); // wait 60 seconds
}

void getJSON(const char* url) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();

      JSONVar myObject = JSON.parse(payload);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      String weatherMain = myObject["weather"][0]["main"];
      String weatherIcon = myObject["weather"][0]["icon"];

      Serial.print("Weather: ");
      Serial.println(weatherMain);
      Serial.print("Icon: ");
      Serial.println(weatherIcon);
    } else {
      Serial.println("Error on HTTP request");
    }

    http.end();
  } else {
    Serial.println("Not connected to WiFi");
  }
}
