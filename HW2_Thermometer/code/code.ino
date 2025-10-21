#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ====== LCD CONFIG ======
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// ====== PINS ======
#define TEMP_PIN A0
#define LED_PIN 13
#define BUTTON_PIN 2

// ====== GLOBAL VARIABLES ======
volatile bool tempFlag = false;
volatile bool buttonFlag = false;
volatile unsigned long lastButtonTime = 0;

bool useFahrenheit = false;
const int MAGIC_ADDR = 0;
const int UNIT_ADDR = 1;
const byte MAGIC_NUMBER = 42;

// ====== TIMER INTERRUPT ======
ISR(TIMER1_COMPA_vect) {
  tempFlag = true;
}

// ====== BUTTON INTERRUPT ======
void buttonISR() {
  unsigned long now = millis();
  if (now - lastButtonTime > 200) {  // Debounce 200 ms
    buttonFlag = true;
    lastButtonTime = now;
  }
}

// ====== EEPROM ======
void loadSettings() {
  if (EEPROM.read(MAGIC_ADDR) == MAGIC_NUMBER) {
    useFahrenheit = EEPROM.read(UNIT_ADDR);
  } else {
    EEPROM.write(MAGIC_ADDR, MAGIC_NUMBER);
    EEPROM.write(UNIT_ADDR, 0);
    useFahrenheit = false;
  }
}

void saveSettings() {
  EEPROM.write(UNIT_ADDR, useFahrenheit);
}

// ====== SETUP ======
void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.print("Booting...");

  loadSettings();

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // Set TIMER1 up (every 500 ms)
  noInterrupts(); // Disable interrupts
  TCCR1A = 0;  // Reset timer registers
  TCCR1B = 0;  // Reset timer registers
  TCNT1 = 0;   // Sets timer count to 0
  OCR1A = 31249;  // (16 MHz / (256 * 2 Hz)) - 1
  TCCR1B |= (1 << WGM12);  // Clear timer on compare match
  TCCR1B |= (1 << CS12);   // Set prescaler at 256
  TIMSK1 |= (1 << OCIE1A); // When A comparer matches values, executes interrupt routine
  interrupts(); // Enable interrupts

  lcd.clear();
  lcd.print("Ready!");
  delay(1000);
  lcd.clear();
}

// ====== LOOP ======
void loop() {
  // --- Read temperature ---
  if (tempFlag) {
    tempFlag = false;
    digitalWrite(LED_PIN, HIGH);

    int sensorValue = analogRead(TEMP_PIN);
    float voltage = sensorValue * (5.0 / 1023.0);
    float tempC = voltage * 100.0;  // Formula LM35
    float temp = useFahrenheit ? (tempC * 9.0 / 5.0 + 32.0) : tempC;

    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print(useFahrenheit ? " F " : " C ");
    lcd.setCursor(0, 1);
    lcd.print("Unit: ");
    lcd.print(useFahrenheit ? "Fahrenheit " : "Celsius   ");

    digitalWrite(LED_PIN, LOW);
  }

  // --- Pressed button ---
  if (buttonFlag) {
    buttonFlag = false;
    useFahrenheit = !useFahrenheit;
    saveSettings();

    lcd.clear();
    lcd.print("Changed to:");
    lcd.setCursor(0, 1);
    lcd.print(useFahrenheit ? "Fahrenheit" : "Celsius");
    delay(800);
    lcd.clear();
  }
}
