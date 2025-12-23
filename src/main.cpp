#include <Arduino.h>
#include <Wire.h>
#include <LittleFS.h>
#include <M5Core2.h>
#include <MHZ19_uart.h>
#include "BME680Sensor.hpp"

const int rx_pin = 13; // Serial rx pin no
const int tx_pin = 14; // Serial tx pin no

MHZ19_uart mhz19;
BME680Sensor bme;

void setup()
{
  M5.begin();               // M5Core2の初期化
  M5.Lcd.fillScreen(BLACK); // 画面を黒で塗りつぶす

  // 横線を描画
  for (int y = 0; y < M5.Lcd.height(); y += 10)
  {
    uint16_t color = (y % 50 == 0) ? DARKGREY : 0x2104;
    int thickness = (y % 100 == 0) ? 2 : 1;
    for (int t = 0; t < thickness; t++)
    {
      M5.Lcd.drawLine(0, y + t, M5.Lcd.width(), y + t, color);
    }
  }

  // 縦線を描画
  for (int x = 0; x < M5.Lcd.width(); x += 10)
  {
    uint16_t color = (x % 50 == 0) ? DARKGREY : 0x2104;
    int thickness = (x % 100 == 0) ? 2 : 1;
    for (int t = 0; t < thickness; t++)
    {
      M5.Lcd.drawLine(x + t, 0, x + t, M5.Lcd.height(), color);
    }
  }

  // LittleFSのマウント
  if (!LittleFS.begin())
  {
    M5.Lcd.println("LittleFS mount failed");
    M5.Lcd.fillScreen(0x2104);
    delay(2000);
    return;
  }

  // I2C初期化
  M5.Lcd.setTextSize(1);
  M5.Lcd.println("I2C Check (GPIO21/22)...");

  Wire.begin(21, 22);
  Wire.setClock(100000);
  delay(100);

  // I2Cスキャン
  int deviceCount = 0;
  for (uint8_t addr = 1; addr < 127; addr++)
  {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0)
    {
      M5.Lcd.printf("Found: 0x%02X\n", addr);
      deviceCount++;
      delay(10);
    }
  }
  M5.Lcd.printf("Devices: %d\n", deviceCount);
  delay(1000);

  // BME680初期化 (0x77のみ)
  M5.Lcd.println("BME680 Init (0x77)...");
  if (bme.begin())
  {
    M5.Lcd.println("BME680 OK!");
  }
  else
  {
    M5.Lcd.println("BME680 FAILED!");
  }
  delay(2000);

  // MH-Z19 CO2センサーの初期化
  mhz19.begin(rx_pin, tx_pin);
  mhz19.setAutoCalibration(false);

  M5.Lcd.println("MH-Z19 is warming up now.");
  delay(10 * 1000);

  // フォントのロード
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.loadFont("font_Yusei24", LittleFS);
}

void loop()
{
  M5.update();

  M5.Lcd.fillScreen(BLACK);
  // BME680からデータを取得
  if (bme.read())
  {
    float temperature = bme.temperature();
    float humidity = bme.humidity();

    // 気温を表示
    M5.Lcd.setCursor(10, 50);
    M5.Lcd.print("温度 ");
    M5.Lcd.print(temperature, 1);
    M5.Lcd.print(" ℃");

    // 湿度を表示
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.print("湿度 ");
    M5.Lcd.print(humidity, 1);
    M5.Lcd.print(" ％");
  }
  // CO2濃度を取得
  int co2ppm = mhz19.getCO2PPM();
  M5.Lcd.setCursor(10, 150);
  M5.Lcd.print("CO2 ");
  M5.Lcd.print(co2ppm);
  M5.Lcd.print(" ppm");

  delay(5000);
}