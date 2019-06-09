/*********************************/
//       Clock Project           //
//    Author: PAWEŁ WRZESIEŃ     //
//       DATE: 09.12.18          //
/*********************************/

#define MODE_CLOCK 1
#define MODE_SETTINGS 2

#include "DHT.h"
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>

#include "Settings.h"
#include "SerialLogs.h"
#include "Display.h"


// Variable defining current working mode
volatile byte currentMode = MODE_CLOCK;

// Variables to store processor time info
long currentClockTime = 0;
long clockTimeOfPreviousTimePrint = 0;
long clockTimeOfPreviousDHTPrint = 0;

// Variables to store current states
float humidity = 0;
float temperature = 0;
Time current_time;

// Buttons states
volatile bool left_arrow_button_state = LOW;
volatile bool right_arrow_button_state = LOW;
volatile bool mode_button_state = LOW;

// Time of button press
long left_arrow_button_activation_time = 0;
long right_arrow_button_activation_time = 0;
long mode_button_activation_time = 0;


// Buttons last release date
long left_arrow_button_last_release_time = 0;
long right_arrow_button_last_release_time = 0;
long mode_button_last_release_time = 0;


// Date Time Settings values
// Structure containing XY position of data on LCD
struct lcd_position {
  String str;
  byte x, y;
};

const lcd_position hours_position = {"HOURS", 5, 0};
const lcd_position minutes_position = {"MINUTES", 8, 0};
const lcd_position seconds_position = {"SECONDS", 11, 0};

const lcd_position day_position = {"DAYS", 4, 1};
const lcd_position month_position = {"MONTHS", 7, 1};
const lcd_position year_position = {"YEARS", 12, 1};


// Settings Cursor values
byte cursor_position = 0;

lcd_position possible_possitions[6] = { hours_position, minutes_position, seconds_position, day_position, month_position, year_position };
byte available_cursor_positions = sizeof(possible_possitions) / sizeof(lcd_position);
long clockTimeOfPreviousCursorBlink = 0;
byte cursorOn = 0;


// Functions definicions
void mode_button_state_change();
void left_arrow_button_state_change();
void right_arrow_button_state_change();


// Declaration of RTC Clock
// RST, DAT, CLK
DS1302 rtc(RTC_RST_PIN, RTC_DAT_PIN, RTC_CLK_PIN);

// Declaration of DHT11 sensor
DHT dht;

// Declaration of IIC2 LCD display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Setting up for 0x27


void setup()
{

  // Setup Serial connection
  Serial.begin(9600);

  /* SETUP FOR DHT Sensor*/
  dht.setup(DHT_PIN);

  /* SETUP FOR RTC */
  // Set the clock to run-mode
  rtc.halt(false);
  // Disable the write protection
  rtc.writeProtect(false);
  current_time = rtc.getTime();

  /* SETUP FOR LCD */
  lcd.begin(16, 2);  // Init LCD 2x16
  lcd.backlight(); // attach backlight

  // Initialize buttons
  pinMode(LEFT_ARROW, INPUT);
  pinMode(RIGHT_ARROW, INPUT);
  pinMode(MODE, INPUT);
  attachInterrupt(digitalPinToInterrupt(LEFT_ARROW), left_arrow_button_state_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ARROW), right_arrow_button_state_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MODE), mode_button_state_change, CHANGE);
}

void loop()
{
  currentClockTime = millis();

  if (currentMode == MODE_CLOCK) {
    // if time to refresh current time
    if (currentClockTime > clockTimeOfPreviousTimePrint + TIME_REFRESH_PERIOD) {
      Serial.println("CLOCK");
      current_time = rtc.getTime();
      update_time(lcd, current_time);
      printTime(rtc);
      Serial.println("");
      clockTimeOfPreviousTimePrint = millis();
    }

    // if time to refresh current temperature and humidity info
    if (currentClockTime > clockTimeOfPreviousDHTPrint + DHT_REFRESH_PERIOD) {
      humidity = dht.getHumidity();
      temperature = dht.getTemperature();
      update_humidity(lcd, humidity);
      update_temperature(lcd, temperature);
      printDHTInfo(dht, DHT_PIN);
      clockTimeOfPreviousDHTPrint = millis();
    }
  }
  if (currentMode == MODE_SETTINGS) {
    // Serial.println(cursor_position);
    lcd.setCursor(possible_possitions[cursor_position].x, possible_possitions[cursor_position].y);

    // Blinking cursor
    if (currentClockTime > clockTimeOfPreviousCursorBlink + CURSOR_BLINKING_PERIOD) {
      if (cursorOn) {
        lcd.cursor();
        cursorOn = 0;
        clockTimeOfPreviousCursorBlink = millis();
      }
      else {
        lcd.cursor();
        cursorOn = 1;
        clockTimeOfPreviousCursorBlink = millis();
      }
    }

    if (currentClockTime > clockTimeOfPreviousTimePrint + TIME_SETTINGS_REFRESH_PERIOD) {
      Serial.println("SETTINGS");
      display_date_time_settings(lcd, current_time);
      Serial.println("");
      clockTimeOfPreviousTimePrint = millis();
    }
  }

}


/*
   Functions section
*/

// LEFT ARROW
void left_arrow_button_state_change() {
  left_arrow_button_state = digitalRead(LEFT_ARROW);

  // If get pushed
  if (left_arrow_button_state == 1) {
    left_arrow_button_activation_time = millis();
  }
  // If get released
  else {

    // If was not released in last few ms (To avoid multiple actions in case of unwanted voltage interrupts)
    if (left_arrow_button_last_release_time + BUTTONS_COOLDOWN < millis()) {

      // if left arrow short pressed
      if (millis() - left_arrow_button_activation_time < MIN_LONG_PRESS_TIME) {
        Serial.println("Left arrow short pressed");

        // Go to previous field to modify
        if (currentMode == MODE_SETTINGS) {


          cursor_position = cursor_position - 1;
          if (cursor_position == 255) cursor_position = 5;
          cursor_position = cursor_position % available_cursor_positions;

        }

      } else {
        // if left arrow long pressed
        Serial.println("Left arrow long pressed");
      }
      left_arrow_button_last_release_time = millis();
    }
  }
}


// RIGHT ARROW
void right_arrow_button_state_change() {
  right_arrow_button_state = digitalRead(RIGHT_ARROW);

  // If get pushed
  if (right_arrow_button_state == 1) {
    right_arrow_button_activation_time = millis();
  }
  // If get released
  else {

    // If was not released in last few ms (To avoid multiple actions in case of unwanted voltage interrupts)
    if (left_arrow_button_last_release_time + BUTTONS_COOLDOWN < millis()) {

      // if right arrow short pressed
      if (millis() - right_arrow_button_activation_time < MIN_LONG_PRESS_TIME) {
        Serial.println("Right arrow short pressed");

        // Go to next field to modify
        if (currentMode == MODE_SETTINGS) {

          cursor_position = cursor_position + 1;
          cursor_position = cursor_position % available_cursor_positions;
        }

      } else {
        // if right arrow long pressed
        Serial.println("Right arrow long pressed");
      }
      right_arrow_button_last_release_time = millis();
    }
  }
}

// Mode button
void mode_button_state_change() {
  mode_button_state = digitalRead(MODE);

  // If get pushed
  if (mode_button_state == 1) {
    mode_button_activation_time = millis();
  }
  // If get released
  else {

    // If was not released in last few ms (To avoid multiple actions in case of unwanted voltage interrupts)
    if (left_arrow_button_last_release_time + BUTTONS_COOLDOWN < millis()) {

      // if mode button short pressed
      if (millis() - mode_button_activation_time < MIN_LONG_PRESS_TIME) {
        Serial.println("MODE short press");


        if (currentMode == MODE_SETTINGS) {
          // Adjusting new time parameters deppending on cursor position
          if (possible_possitions[cursor_position].str == "HOURS") current_time.hour = (current_time.hour + 1) % 24;
          if (possible_possitions[cursor_position].str == "MINUTES") current_time.min = (current_time.min + 1) % 60;
          if (possible_possitions[cursor_position].str == "SECONDS") current_time.sec = (current_time.sec + 1) % 60;
          if (possible_possitions[cursor_position].str == "DAYS") current_time.date = (current_time.date + 1) % 31;
          if (possible_possitions[cursor_position].str == "MONTHS") current_time.mon = (current_time.mon + 1) % 12;
          if (possible_possitions[cursor_position].str == "YEARS") current_time.year = (current_time.year + 1) % 2100;

        }

      } else {

        // if mode button long pressed
        Serial.println("MODE long press");

        // Variable used to detect if mode was changed within current button press
        bool modeChanged = 0;

        // Activate MODE SETTINGS
        if (currentMode == MODE_CLOCK && !modeChanged) {
          currentMode = MODE_SETTINGS;
          modeChanged = 1;
          cursor_position = 0;
          Serial.println("Changing mode to SETTINGS");
        }

        // Activate MODE CLOCK
        if (currentMode == MODE_SETTINGS && !modeChanged) {
          currentMode = MODE_CLOCK;
          modeChanged = 1;
          cursorOn = 0;

          // Set new time to RTC
          rtc.setTime(current_time.hour, current_time.min, current_time.sec);
          rtc.setDate(current_time.date, current_time.mon, current_time.year);
          Serial.println("Changing mode to MODE");
        }

        modeChanged = 0;
      }
      mode_button_last_release_time = millis();
    }
  }
}
