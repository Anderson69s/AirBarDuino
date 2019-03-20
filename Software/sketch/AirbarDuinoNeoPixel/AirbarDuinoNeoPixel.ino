#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <AirBarDuino.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//wiring:
// MISO->12
// MOSI->11
// CS->10
// SCK->13


#define PIN 6
#define NLEDS 51
Adafruit_NeoPixel strip = Adafruit_NeoPixel(51, PIN, NEO_GRB + NEO_KHZ800);
//https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library
int Inhib = 0;


void setup()
{
  Serial.begin( 115200 );
  while (!Serial);
  Serial.println("AirBar Start");
  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");
  delay( 200 );
  if (!Hid.SetReportParser(0, &AirBar))
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );
  strip.begin();
  Blank();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
  if (Inhib) Inhib--;

  Usb.Task();
  AirBarUpdate(5);
  if(AirBarValue.NTouch>0)
  {
    Serial.print(AirBarValue.NTouch);
    Serial.print("\t");
    Serial.print(AirBarValue.Touch1X);
    Serial.print("\t");
    Serial.print(AirBarValue.Touch1Y);
    Serial.print("\t");
    Serial.print(AirBarValue.Touch2X);
    Serial.print("\t");
    Serial.print(AirBarValue.Touch2Y);
    Serial.print("\t");
    Serial.print(3700);
    Serial.print("\t");
    Serial.print(0);
    Serial.println();
    LedStripAnime();
  }
}
//***********************************************************************
void LedStripAnime(void)
{
  int LedA, LedB;
  int TeinteA, TeinteB;

  LedA = map(AirBarValue.Touch1X, 26, 3400, 50, 0);
  TeinteA = map(AirBarValue.Touch1Y, 0, 3900, 0, 255);

  if (AirBarValue.NTouch == 2)
  {
    LedB = map(AirBarValue.Touch2X, 26, 3400, 50, 0);
    TeinteB = map(AirBarValue.Touch2Y, 0, 3900, 0, 255);
  }
  Blank();
  strip.setPixelColor(LedA, 255 - TeinteA, 0, TeinteA);
  if (AirBarValue.NTouch == 2) strip.setPixelColor(LedB, 255 - TeinteB, 0, TeinteB);
  strip.show();
  //if (abs(LedA - LedB) <= 4)
  /*
  if (AirBarValue.NTouch == 2)
  {
    if (abs(AirBarValue.Touch2X - AirBarValue.Touch1X) <= 500)
    {
      if (!Inhib) explode(LedA);
      Inhib = 5;
    }
  }*/
  strip.show();
}
//*************************************************************************
void explode(int StartPos)
{
  int Fin = 0;
  int LedLow, LedHigh;
  LedLow = StartPos;
  LedHigh = StartPos;

  do
  {
    strip.setPixelColor(LedHigh, 50, 50, 50);
    strip.setPixelColor(LedLow, 50, 50, 50);
    if (LedHigh < NLEDS - 1)LedHigh++;
    if (LedHigh == NLEDS - 1) Fin++;
    if (LedLow > 0) LedLow--;
    if (LedLow == 0) Fin++;
    strip.show();
    delay(5);
  } while (Fin < 2);
}
//***********************************************************
void Blank()
{
  for (int i = 0; i < NLEDS; i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
}
//***********************************************************
