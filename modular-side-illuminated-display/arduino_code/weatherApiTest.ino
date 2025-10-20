#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// WiFi credentials
const char* ssid = "UoB-IoT";
const char* password = "c7ii7fzw";

// OpenWeatherMap API
const char* apiKey = "4274e06f288318211422b3a4b8e05f94";
const char* lat = "51.3793";
const char* lon = "-2.3272";

// Pin assignments
const int cloudPin = A0;
const int sunPin = A1;
const int rainPin = A2;
const int windPin = D8;
const int snowPin = D9;
const int lightningPin = D10;
const int warningPin = D2;

void setup() {
  Serial.begin(115200);

  // Set pin modes
  pinMode(cloudPin, OUTPUT);
  pinMode(sunPin, OUTPUT);
  pinMode(rainPin, OUTPUT);
  pinMode(windPin, OUTPUT);
  pinMode(snowPin, OUTPUT);
  pinMode(lightningPin, OUTPUT);
  pinMode(warningPin, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
}

void loop() {
  String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + String(lat) + "&lon=" + String(lon) + "&appid=" + String(apiKey);
  getWeatherAndSetPins(url);
  delay(60000); // Wait 1 minute before next check
}

void getWeatherAndSetPins(String url) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url.c_str());
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      JSONVar data = JSON.parse(payload);

      if (JSON.typeof(data) == "undefined") {
        Serial.println("JSON parsing failed.");
        return;
      }

      String mainWeather = data["weather"][0]["main"];
      String description = data["weather"][0]["description"];
      String icon = data["weather"][0]["icon"];
      float windSpeed = (double) data["wind"]["speed"];
      int cloudCover = data["clouds"]["all"];

      Serial.println("Weather: " + mainWeather);
      Serial.println("Description: " + description);
      Serial.println("Wind speed: " + String(windSpeed));
      Serial.println("Cloud cover: " + String(cloudCover));

      resetPins();
      int brightness = getIntensityFromLiveData(description, cloudCover, windSpeed);

      if (mainWeather == "Clouds") analogWrite(cloudPin, brightness);
      else if (mainWeather == "Clear") analogWrite(sunPin, brightness);
      else if (mainWeather == "Rain") analogWrite(rainPin, brightness);
      else if (mainWeather == "Snow") analogWrite(snowPin, brightness);
      else if (mainWeather == "Thunderstorm") analogWrite(lightningPin, brightness);
      else if (description.indexOf("wind") != -1) analogWrite(windPin, brightness);

      // Warning pin logic (storm, heavy wind/rain, etc.)
      if (description.indexOf("storm") != -1 || description.indexOf("warning") != -1 || windSpeed > 10) {
        digitalWrite(warningPin, HIGH);
      }

    } else {
      Serial.println("HTTP error: " + String(httpCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

int getIntensityFromLiveData(String desc, int cloudPercent, float windSpeed) {
  desc.toLowerCase();

  if (desc.indexOf("cloud") != -1) {
    return map(cloudPercent, 0, 100, 80, 255);
  } else if (desc.indexOf("wind") != -1) {
    return constrain(windSpeed * 50, 80, 255);
  } else if (desc.indexOf("heavy") != -1) {
    return 255;
  } else if (desc.indexOf("moderate") != -1) {
    return 160;
  } else if (desc.indexOf("light") != -1) {
    return 100;
  }

  return 120;
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
