#include <Servo.h>
#include <LiquidCrystal.h>

// Objects
Servo feederServo;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pins
const int trigPin = 7;
const int echoPin = 8;
const int servoPin = 9;
const int buttonPin = 13;
const int ledPin = 10;
const int buzzerPin = 6;

// Variables
unsigned long lastFeedTime = 0;
unsigned long feedInterval = 5000;  // 5s auto feed interval
const int containerHeight = 20;     // 
const int lowFoodThreshold = 5;     // 
const int feedDuration = 1000;      // servo open for 1s

// --------------------------------
void setup() {
  Serial.begin(9600);
  feederServo.attach(servoPin);
  feederServo.write(0); // closed
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Smart Pet Feeder");
  delay(2000);
  lcd.clear();
}

// --------------------------------
void loop() {
  float foodLevel = measureFoodLevel();
  
  // Manual button
  if (digitalRead(buttonPin) == LOW) {
    feedPet("Manual");
    delay(300);
  }
  
  // Auto feed
  if (millis() - lastFeedTime >= feedInterval) {
    if (foodLevel > lowFoodThreshold) {
      feedPet("Auto");
      lastFeedTime = millis();
    } else {
      lowFoodAlert();
    }
  }
  
  // Update LCD
  updateDisplay(foodLevel);
  
  // Red LED alert
  if (foodLevel <= lowFoodThreshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  delay(1000);
}

// --------------------------------
float measureFoodLevel() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;
  
  float level = containerHeight - distance;
  if (level < 0) level = 0;
  if (level > containerHeight) level = containerHeight;
  
  return level;
}

// --------------------------------
void feedPet(String type) {
  Serial.println("Feeding: " + type);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(type + " Feed");
  
  tone(buzzerPin, 1000, 200);
  
  feederServo.write(90);
  delay(feedDuration);
  feederServo.write(0);
  
  tone(buzzerPin, 1500, 200);
  
  lcd.setCursor(0,1);
  lcd.print("Done!");
  delay(1000);
}

// --------------------------------
void updateDisplay(float level) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Food: ");
  lcd.print(level,1);
  lcd.print("cm");
  
  lcd.setCursor(0,1);
  if (level <= lowFoodThreshold) {
    lcd.print("REFILL NEEDED!");
  } else {
    unsigned long t = (feedInterval - (millis() - lastFeedTime))/1000;
    lcd.print("Next: ");
    lcd.print(t);
    lcd.print("s");
  }
}

// --------------------------------
void lowFoodAlert() {
  for (int i=0; i<2; i++) {
    tone(buzzerPin, 2000, 200);
    delay(300);
  }
}