#include <Arduino.h>
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
}

void loop()
{

}