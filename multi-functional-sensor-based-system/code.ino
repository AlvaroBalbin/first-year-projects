
#include <LiquidCrystal.h> // include the library for the LCD



// define some macros connected to ESP32

#define RS 3  

#define E  4   

#define D4 5   

#define D5 6   

#define D6 7   

#define D7 8   



// define peripheral connections 

#define TMP36_PIN A0 

#define LED_PIN A2 



// start the LCD

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);



void setup() {



    

    // Serial.begin(115200); - useful for debugging or testing( but not completely necessary)



    // create pin modes for components

    pinMode(TMP36_PIN, INPUT);

    pinMode(LED_PIN, OUTPUT);



    // make a little start up message

    lcd.begin(16, 2); 

    lcd.print("System Ready");

    delay(1000); // gives user time to read it

    lcd.clear(); // clear LCD for further messages

}



void loop() {

    int sensorValue = analogRead(TMP36_PIN); // get sensor value

    float voltage = sensorValue * (3.3 / 4095.0);  // convert analog to voltage

    float temperature = (voltage - 0.5) * 100.0;  // now convert the voltage to Celsius



    // get the LED state and set a condition for it (>20C)

    bool ledState = (temperature > 20); 

    digitalWrite(LED_PIN, ledState);



    // although LCD been cleared do it again

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("TEMP: ");

    lcd.print(temperature, 1); // display temperature onto LCD with one decimal place

    lcd.print(" C");



    /* useful for debugging or for testing

    Serial.print("Temperature Value: ");

    Serial.println(temperature, 6); // print the temperature values to 6 decimal places

    */



    // move cursor to next line for LED condition

    lcd.setCursor(0, 1);

    lcd.print("LED: ");

    lcd.print(ledState ? "ON " : "OFF");



    delay(1000); // create a delay - avoiding flickering also useful for serial monitor tracking

}

ab4647 as6198

