// Full video: https://youtu.be/xn_vOZKjWgU
// Created by Vstone on 2021/11/12.

#include <M5Stack.h>

// counter variables
int seconds = 0;
int minutes = 0;
unsigned long prevTime;

enum Modes {
  IDLE,
  COUNTDOWN,
  RING
};

enum Modes modes;

void setup() {
  // enable LCD, SD card, Serial and I2C port
  M5.begin();
  M5.Lcd.setTextSize(10);
  M5.Lcd.setCursor(140, 90);
  M5.Lcd.print(":");
  modes = IDLE; // sets IDLE mode
}

void loop() {
  // reads the state of the buttons
  M5.update(); 
  // display the timer
  M5.Lcd.setCursor(180, 90);
  M5.Lcd.print(seconds < 10 ? "0" : "");
  M5.Lcd.print(seconds);
  M5.Lcd.setCursor(60, 90);
  M5.Lcd.print(minutes < 10 ? "0" : "");
  M5.Lcd.print(minutes);

  switch (modes) {
    case IDLE:
      // reset the timer the A and B is pressed together
      if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
        seconds = 0;
        minutes = 0;
      }
      // increase the minute when A is pressed
      else if (M5.BtnA.wasPressed() && minutes < 99) {
        minutes++;
      }
      // increase the minute faster when A is held down
      else if (M5.BtnA.pressedFor(500) && minutes < 99) {
        minutes++;
        delay(50);
      }
      // increase the seconds when B is pressed
      else if (M5.BtnB.wasPressed() && seconds < 60) {
        seconds++;
        // rollover when second reaches 60
        if (seconds == 60) {
          seconds = 0;
          minutes++;
        }
      }
      // increase the seconds faster when B is held down
      else if (M5.BtnB.pressedFor(500) && seconds < 60) {
        seconds++;
        delay(50);
        if (seconds == 60) {
          seconds = 0;
          minutes++;
        }
      }
      // start the timer
      else if (M5.BtnC.wasPressed() && !(seconds == 0 && minutes == 0)) {
        prevTime = millis();
        modes = COUNTDOWN;
      }
      break;

    case COUNTDOWN:
      // paused the timer if C was pressed and go back to IDLE mode
      if (M5.BtnC.wasPressed()) {
        modes = IDLE;
        break;
      }
      else {
        // if seconds and minutes is not 0, keep counting down
        if (!(seconds == 0 && minutes == 0)) {
          if (millis() - prevTime >= 1000) {
            prevTime = millis();
            seconds--;
            // rollover when seconds reaches 0
            if (seconds < 0 && minutes > 0 ) {
              minutes--;
              seconds = 59;
            }
          }
          break;
        }
        // ring the alarm
        else {
          modes = RING;
          break;
        }
      }

    case RING:
      // stop alarm and return to IDLE mode when any button is pressed
      if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
        modes = IDLE;
        break;
      }
      else {
        // ring the alarm with a 700ms interval
        if (millis() - prevTime >= 700) {
          prevTime = millis();
          M5.Speaker.tone(900, 200);
        }
      }
  }
}
