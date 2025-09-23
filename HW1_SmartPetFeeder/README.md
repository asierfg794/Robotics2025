# Smart Pet Feeder - Code README

## Overview
This Arduino code creates an automatic pet feeder that dispenses food every 5 seconds and monitors food levels using sensors.

## Hardware Setup
**Components needed:**
- Arduino Uno
- Servo motor (pin 9)
- Ultrasonic sensor HC-SR04 (pins 7, 8)
- LCD 16x2 (pins 12, 11, 5, 4, 3, 2)
- Push button (pin 13)
- LED (pin 10)
- Buzzer (pin 6)

## How the Code Works

### Main Functions

**`measureFoodLevel()`**
- Uses ultrasonic sensor to measure food in container
- Sends sound wave and measures return time
- Converts time to distance, then calculates food level
- Returns food level in centimeters

**`feedPet(String type)`**
- Controls the feeding process
- Shows message on LCD ("Manual" or "Auto")
- Makes sound with buzzer
- Opens servo for 1 second to dispense food
- Shows "Done!" when finished

**`updateDisplay(float level)`**
- Updates LCD screen with current information
- Line 1: Shows food level in cm
- Line 2: Shows countdown to next feed OR "REFILL NEEDED!"

**`lowFoodAlert()`**
- Makes warning sound when food is low
- Plays two beep sounds

### Program Flow
1. **Setup**: Initialize all components and show welcome message
2. **Main Loop** (repeats every 1 second):
   - Measure food level with sensor
   - Check if button is pressed → feed manually
   - Check if 5 seconds passed → feed automatically
   - Update LCD display
   - Turn on LED if food is low

## Key Variables
```cpp
feedInterval = 5000;        // Feed every 5 seconds
containerHeight = 20;       // Container is 20cm tall
lowFoodThreshold = 5;       // Warning when <5cm food left
feedDuration = 1000;        // Servo stays open 1 second
```

## Safety Features
- Won't feed if food level is too low
- Button has delay to prevent multiple presses
- Food level is limited between 0 and container height
- Clear feedback with LCD, LED, and sounds

## How to Use
1. Upload code to Arduino
2. Fill food container
3. System feeds automatically every 5 seconds
4. Press button for manual feeding
5. Red LED warns when food is low
6. Refill when LCD shows "REFILL NEEDED!"

## Troubleshooting
- **LCD blank**: Check connections and power
- **No feeding**: Check servo connections and food level
- **Wrong distance**: Move objects away from sensor
- **No sound**: Check buzzer connections

## Customization
Change these values to adjust behavior:
- `feedInterval`: Change feeding frequency
- `lowFoodThreshold`: Change warning level  
- `feedDuration`: Change portion size
- `containerHeight`: Match your container size