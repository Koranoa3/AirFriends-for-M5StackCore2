#include <Arduino.h>
#include <LittleFS.h>
#include <M5Core2.h>

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
}

void loop()
{
  static uint8_t fontIndex = 0;
  String fonts[] = {"font_Yusei24", "font_DotGoth16"};
  M5.update();
  if (M5.BtnA.wasPressed())
  {
    fontIndex = (fontIndex + 1) % 2;
    M5.Lcd.fillScreen(BLACK);
    try
    {
      M5.Lcd.loadFont(fonts[fontIndex], LittleFS);
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.print("適切な気温です。");
    }
    catch (...)
    {
      M5.Lcd.println("Font load error");
      M5.Lcd.fillScreen(0x2104);
      delay(2000);
    }
  }
}