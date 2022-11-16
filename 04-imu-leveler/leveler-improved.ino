// Full video: https://youtu.be/9j5W7DZLtIs
// Created by Vstone on 2021/12/24.

#include <Arduino.h>
#define M5STACK_MPU6886
#include <M5Stack.h>

//Variables for storing inertial sensor data
float accX = 0.0F;  
float accY = 0.0F;
float accZ = 0.0F;

float offsetArr[3];

const int MOVING_AVG = 10;
float movingAvgX[MOVING_AVG];
float movingAvgY[MOVING_AVG];
int idx = 0;

int prev_x = 0;
int prev_y = 0;

void readMultipleData(float * meanAcc, int sampleCounts)
{
  float ax, ay, az;
  ax = 0.0;
  ay = 0.0;
  az = 0.0;


  for (int i = 0; i < sampleCounts; i++) 
  {
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    ax += accX;
    ay += accY;
    az += accZ;
  }
  meanAcc[0] = ax / sampleCounts;
  meanAcc[1] = ay / sampleCounts;
  meanAcc[2] = az / sampleCounts;
}

// function to draw the grid on the display
void drawGrid() 
{
  M5.Lcd.drawLine(41, 120, 279, 120, CYAN);
  M5.Lcd.drawLine(160, 1, 160, 239, CYAN);
  M5.Lcd.drawCircle(160, 120, 119, CYAN);
  M5.Lcd.drawCircle(160, 120, 60, CYAN);
}

// function to draw a spot on the display
void drawSpot(int ax, int ay) 
{
  int x, y;
  x = map(constrain(ax, -300, 300), -300, 300, 40, 280);
  y = map(constrain(ay, -300, 300), -300, 300, 240, 0);

  // M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillCircle(prev_x, prev_y, 7, BLACK);
  drawGrid();
  M5.Lcd.fillCircle(x, y, 7, WHITE);
  prev_x = x;
  prev_y = y;
}

void setup() 
{
  // Initialize M5Stack and IMU
  M5.begin();
  M5.IMU.Init();
  readMultipleData(offsetArr, 50);
  for (int i = 0; i < MOVING_AVG; i++) 
  {
    movingAvgX[i] = offsetArr[0];
    movingAvgY[i] = offsetArr[1];
  }
}

void loop() 
{
  float meanAcc[3];
  float ax, ay;
  // M5.IMU.getAccelData(&accX, &accY, &accZ);
  readMultipleData(meanAcc, 50);

  movingAvgX[idx] = meanAcc[0];
  movingAvgY[idx] = meanAcc[1];

  idx = (idx + 1) % MOVING_AVG;

  ax = ay = 0;
  for (int i = 0; i < MOVING_AVG; i++) 
  {
    ax += movingAvgX[i];
    ay += movingAvgY[i];
  }
  ax /= MOVING_AVG;
  ay /= MOVING_AVG;
	drawSpot((int)((ax-offsetArr[0])*1000), (int)((ay-offsetArr[1])*1000));
	delay(100); 
}
