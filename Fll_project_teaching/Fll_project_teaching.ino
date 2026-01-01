#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Define the pin and sensor type
#define DHTPIN 7 // what pin it is plugged into
#define DHTTYPE DHT11 // what type of humidity sensor it is 
#define LDR_PIN A0  // light sensor pin

// Create objects
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD I2C address and size (16 columns, 2 rows)
DHT dht(DHTPIN, DHTTYPE); // defines the type of humidity sensor and what it is plugged into

void setup() {
  Serial.begin(9600); // starts serial communication to view values on computer

  lcd.init(); // initialize LCD
  lcd.backlight(); // turn on LCD backlight

  dht.begin(); // tells the humidity/temperature sensor to start working

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print("System Starting"); 
  Serial.println("System Ready"); // a message that shows there are no errors in the setup 
  delay(2000); // delay so the user can read message
  lcd.clear(); // remove the message from screen
}

void loop() {
  float humidity = dht.readHumidity(); // reads humidity in the room from the DHT sensor
  float temperature = dht.readTemperature(); // reads temperature in Celsius by default
  int lightValue = analogRead(LDR_PIN); // reads the light sensor’s value between 0-1023 depending on light

  // Debug output to Serial Monitor
  Serial.print("Light value: "); // prints text for clarity
  Serial.println(lightValue); // prints the number from the sensor

  // Check if light level is bright or dark based on threshold
  String lightStatus;
  if (lightValue < 500) { // if less than 500 means bright
    lightStatus = "Bright";
    Serial.println("Bright");
  } else {
    lightStatus = "Dark";
    Serial.println("Dark");
  }

  // Check for DHT sensor failure
  if (isnan(humidity) || isnan(temperature)) { // if values are not real numbers
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    Serial.println("Sensor Error!");
    delay(2000);
    return; // skip the rest of the loop and restart reading
  }

  // Convert Celsius to Fahrenheit
  float tempF = (temperature * 1.8) + 32;

  // DISPLAY DATA ON LCD
  lcd.clear(); // clear old data before writing new data

  // First row: Temperature and Humidity
  lcd.setCursor(0, 0); 
  lcd.print("T:");
  lcd.print(tempF, 1); // one decimal place
  lcd.print("F H:");
  lcd.print(humidity, 0); // no decimals for humidity
  lcd.print("%");

  // Second row: Light value and status (Bright/Dark)
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(lightValue); 
  lcd.print(" ");
  lcd.print(lightStatus);

  // Serial Output for Monitoring
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Temp: ");
  Serial.print(tempF);
  Serial.println(" F");

  delay(2000); // delay before reading again
}
