// Full video: https://youtu.be/YYfUboIX4U8
// Created by Vstone on 2021/09/08.

#include <M5Stack.h>
int x, y, r, color;
int x_speed, y_speed;

void setup() {
  M5.begin();     //Initialize
  x = 160;        //center of x-axis
  y = 120;        //center of y-axis
  r = 30;         //radius of circle
  color = 65535;  //white
  x_speed = 5;
  y_speed = 5;
}

void loop() {
  M5.Lcd.clear(BLACK);
  M5.Lcd.fillCircle(x, y, r, color);
  x += x_speed;
  y += y_speed;
  
  if (x + r == 320 || x - r == 0) {
    x_speed = -x_speed;
    color = random(65535);
  }
  if (y + r == 240 || y - r == 0) {
    y_speed = -y_speed;
    color = random(65535);
  }
  delay(50);
}
