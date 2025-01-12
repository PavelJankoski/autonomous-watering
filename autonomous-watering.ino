#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht.h>

// OLED display configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // Address for the OLED display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 sensor configuration
#define DHTPIN 7       // Pin connected to the DHT11 OUT pin
dht DHT;

// Soil moisture sensor configuration
#define sensorPin A0   // Analog pin connected to the soil moisture sensor
#define MOTOR_PIN 3    // Pin connected to the motor pump
#define THRESHOLD 300  // Threshold for soil moisture to activate the motor

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Loop forever if the display cannot initialize
  }

  // Display splash screen
  display.display();
  delay(2000);

  // Clear the display
  display.clearDisplay();

  // Initialize motor pin as OUTPUT
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.println("Setup complete.");
}

void loop() {
  // Read the current soil moisture value
  int soilMoistureValue = analogRead(sensorPin);

  // Read temperature and humidity from DHT11 sensor
  int chk = DHT.read11(DHTPIN);
  float temperature = DHT.temperature;
  float humidity = DHT.humidity;

  // Handle DHT sensor errors
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: Failed to read from DHT sensor!");
  }

  // Print sensor values to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Update the OLED display with sensor values
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println("Soil moist: " + String(soilMoistureValue));
  display.println("Temp: " + String((int)temperature) + " *C");
  display.println("Humidity: " + String((int)humidity) + " %");

  // Control the motor pump based on soil moisture
  if (soilMoistureValue > THRESHOLD) {
    digitalWrite(MOTOR_PIN, LOW); // Turn on motor pump
    display.println("Motor: ON");
  } else {
    digitalWrite(MOTOR_PIN, HIGH); // Turn off motor pump
    display.println("Motor: OFF");
  }

  // Display everything
  display.display();

  // Pause for 1.5 seconds before the next loop iteration
  delay(1500);
}
