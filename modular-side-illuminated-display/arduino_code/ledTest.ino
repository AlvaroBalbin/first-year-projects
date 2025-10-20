const int symbolPin = A2;    // A2 pin
const int pwmChannel = 0;    // Use PWM channel 0
const int freq = 5000;       // PWM frequency
const int resolution = 8;    // 8-bit resolution (values from 0–255)
const int dutyCycle = 128;   // 50% brightness 100% = 225

void setup() {
  // Set up PWM on A2
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(symbolPin, pwmChannel);
  ledcWrite(pwmChannel, dutyCycle);  // Set 50% brightness
}

void loop() {
  // No need for loop unless you're animating brightness
}