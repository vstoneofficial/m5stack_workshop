/**
 * This example turns the ESP32 into a Bluetooth LE keyboard 
 * that you could use to control your video conference software audio and video toggles
 * 
 * Full video: https://youtu.be/El_mGy546Pk
 * Created by Vstone on 2022/09/20.
 */
#include <BleKeyboard.h>
#include <Arduino.h>
#include <M5Stack.h>

BleKeyboard bleKeyboard("SUBSCRIBE", "VS", 100);
bool printFlag = false;
bool muteFlag = false;
bool videoFlag = false;

void setup() {
  // initialize M5Stack
  M5.begin();
  M5.Lcd.setTextSize(5);
  M5.Lcd.print("Connecting...");
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  // initialize bleKeyboard
  bleKeyboard.begin();
}

void loop() {
  M5.update(); // update to receive button input
  if(bleKeyboard.isConnected()) 
  {
    if (!printFlag)
    {
      M5.Lcd.clear();
      M5.Lcd.setCursor(30, 30);
      M5.Lcd.print("Connected!");
      printFlag = true;
    }
    // if btnC was pressed, mute
    if (M5.BtnC.wasPressed()) 
    {
      M5.Lcd.clear();
      M5.Lcd.setCursor(30, 30);
      muteFlag = !muteFlag;
      muteFlag ? M5.Lcd.print("Unmuted") : M5.Lcd.print("Muted");

      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press('a');
      delay(100);
      bleKeyboard.releaseAll();
    }
    // if btnB was pressed, on/off video
    if (M5.BtnB.wasPressed()) 
    {
      M5.Lcd.clear();
      M5.Lcd.setCursor(30, 30);
      videoFlag = !videoFlag;
      videoFlag ? M5.Lcd.print("Video ON") : M5.Lcd.print("Video OFF");

      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press('v');
      delay(100);
      bleKeyboard.releaseAll();
    }
    // if btnA was hold for 300ms (to prevent accidental trigger), quit
    if (M5.BtnA.pressedFor(300, 2000)) 
    {
      M5.Lcd.clear();
      M5.Lcd.setCursor(30, 30);
      M5.Lcd.print("Quit");

      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press('q');
      bleKeyboard.releaseAll(); // we need to release before pressing enter!
      bleKeyboard.press(KEY_RETURN);
      delay(100);
      bleKeyboard.releaseAll();
    }

  }

}