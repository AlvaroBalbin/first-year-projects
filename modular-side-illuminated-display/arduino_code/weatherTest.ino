#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "UoB-IoT";
const char* password = "c7ii7fzw"; // <- Replace this
const char* weatherUrl = "https://people.bath.ac.uk/lj386/PW/PW4/weatherapi.json";

// Your pin setup
const int cloudPin = A0;
const int sunPin = A1;
const int rainPin = A2;
const int windPin = D8;
const int snowPin = D9;
const int lightningPin = D10;
const int warningPin = D2;

void setup() {
  Serial.begin(115200);
  pinMode(cloudPin, OUTPUT);
  pinMode(sunPin, OUTPUT);
  pinMode(rainPin, OUTPUT);
  pinMode(windPin, OUTPUT);
  pinMode(snowPin, OUTPUT);
  pinMode(lightningPin, OUTPUT);
  pinMode(warningPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void loop() {
  getWeatherAndSetPins(wetherUrl);
  delay(60000); // Call every 60 sec
}

void getWeatherAndSetPins(const char* url) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      JSONVar data = JSON.parse(payload);

      if (JSON.typeof(data) == "undefined") {
        Serial.println("JSON parsing failed");
        return;
      }

      String mainWeather = data["weather"][0]["main"];
      String description = data["weather"][0]["description"]; // optional
      Serial.print("Weather: ");
      Serial.println(mainWeather);

      // Reset all pins first
      resetPins();

      // Set brightness (0–255); we'll simulate this with simple logic
      int brightness = getIntensityFromDescription(description);

      if (mainWeather == "Clouds") analogWrite(cloudPin, brightness);
      else if (mainWeather == "Clear") analogWrite(sunPin, brightness);
      else if (mainWeather == "Rain") analogWrite(rainPin, brightness);
      else if (mainWeather == "Snow") analogWrite(snowPin, brightness);
      else if (mainWeather == "Thunderstorm") analogWrite(lightningPin, brightness);
      else if (mainWeather == "Wind" || description.indexOf("wind") != -1) analogWrite(windPin, brightness);

      // Simulated weather warning (based on custom condition)
      if (description.indexOf("storm") != -1 || description.indexOf("warning") != -1) {
        digitalWrite(warningPin, HIGH);
      }

    } else {
      Serial.println("HTTP error");
    }
    http.end();
  }
}

// Brightness helper: crude estimation
int getIntensityFromDescription(String desc) {
  desc.toLowerCase();
  if (desc.indexOf("light") != -1) return 80;
  else if (desc.indexOf("moderate") != -1) return 160;
  else if (desc.indexOf("heavy") != -1 || desc.indexOf("intense") != -1) return 255;
  else return 120; // default medium
}

void resetPins() {
  analogWrite(cloudPin, 0);
  analogWrite(sunPin, 0);
  analogWrite(rainPin, 0);
  analogWrite(windPin, 0);
  analogWrite(snowPin, 0);
  analogWrite(lightningPin, 0);
  digitalWrite(warningPin, LOW);
}
