#if !defined(__AIRBAR_H__)
#define __AIRBAR_H__

#include <usbhid.h>
#define USBDATABUFF 24

struct AirBarValues 
{
  int NTouch;
  int Touch1X;
  int Touch1Y;
  int Touch2X;
  int Touch2Y;
  int MagicNumber;
  int LastMagicNumber;
  bool Change;
} AirBarValue;

struct AirBarEventData
{
   uint8_t data[USBDATABUFF];
};

int AirBarUpdate(int TimeOut)
{
  static int TO;
  
  if(AirBarValue.LastMagicNumber!=AirBarValue.MagicNumber)
  {
    TO=TimeOut;
  }
  else
  {
    if (TO >0) TO--;
    if(TO==0)
    {
      AirBarValue.NTouch=0;
      TO=-1;
    }
  }
  AirBarValue.LastMagicNumber=AirBarValue.MagicNumber;
  return(AirBarValue.NTouch);
}

class AirBarEvents
{
public:
	virtual void OnAirBarChanged(const AirBarEventData *evt);
};

#define RPT_AIRBAR_LEN	sizeof(AirBarEventData)/sizeof(uint8_t)
class AirBarReportParser : public HIDReportParser
{
	AirBarEvents *AirbarEvents;
  uint8_t oldAirBar[RPT_AIRBAR_LEN];
public:
	AirBarReportParser(AirBarEvents *evt);
	virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

AirBarReportParser::AirBarReportParser(AirBarEvents *evt) :
  AirbarEvents(evt)
{}

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
AirBarEvents AirBarEvents;
AirBarReportParser AirBar(&AirBarEvents);

void AirBarReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
  
  bool match = true;

  // Checking if there are changes in report since the method was last called
  for (uint8_t i=0; i<RPT_AIRBAR_LEN; i++) {
    if( buf[i] != oldAirBar[i] ) {
      match = false;
      break;
    }
  }
  // Calling Air Bar event handler
if (!match && AirbarEvents) 
  {
    AirbarEvents->OnAirBarChanged((const AirBarEventData*)buf);
    for (uint8_t i=0; i<RPT_AIRBAR_LEN; i++) oldAirBar[i] = buf[i];
  }
}

void AirBarEvents::OnAirBarChanged(const AirBarEventData *evt)
{
int tempY=0;
AirBarValue.MagicNumber=evt->data[2];
AirBarValue.NTouch=evt->data[1];

AirBarValue.Touch1X=evt->data[6]<<8; 
AirBarValue.Touch1X+=evt->data[5]; 

tempY=evt->data[8]<<8; 
tempY+=evt->data[7];
AirBarValue.Touch1Y=1.8*(2000-tempY+120);
if(AirBarValue.Touch1Y<0) AirBarValue.Touch1Y=0;

if(evt->data[1]==2)
{
  AirBarValue.Touch2X=evt->data[15]<<8; 
  AirBarValue.Touch2X+=evt->data[14]; 

  tempY=evt->data[17]<<8; 
  tempY+=evt->data[16]; 
  AirBarValue.Touch2Y=1.8*(2000-tempY+120);
  if(AirBarValue.Touch2Y<0) AirBarValue.Touch2Y=0;
}
else 
{
  AirBarValue.Touch2X=AirBarValue.Touch1X;
  AirBarValue.Touch2Y=AirBarValue.Touch1Y;
}

}
#endif 
