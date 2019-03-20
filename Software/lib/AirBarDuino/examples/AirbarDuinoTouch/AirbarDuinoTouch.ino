#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>
#include <AirBarDuino.h>

#define DEBOUNCE 5

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
}

void loop()
{
  Usb.Task();
  Serial.print(AirBarUpdate(DEBOUNCE));
  Serial.print(" ");
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
}


