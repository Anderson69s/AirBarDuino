/*
 *******************************************************************************
 * USB-MIDI dump utility
 * Copyright (C) 2013-2016 Yuuichi Akagawa
 *
 * for use with USB Host Shield 2.0 from Circuitsathome.com
 * https://github.com/felis/USB_Host_Shield_2.0
 *
 * This is sample program. Do not expect perfect behavior.
 *******************************************************************************
 */

#include <usbh_midi.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);

void MIDI_poll();
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime);

boolean bFirst;
uint16_t pid, vid;

void setup()
{
  bFirst = true;
  vid = pid = 0;
  Serial.begin(115200);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  //unsigned long t1;

  Usb.Task();
  //t1 = micros();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
  }
  //delay(1ms)
  //doDelay(t1, micros(), 1000);
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t  rcvd;

  if (Midi.vid != vid || Midi.pid != pid) {
    sprintf(buf, "VID:%04X, PID:%04X", Midi.vid, Midi.pid);
    Serial.println(buf);
    vid = Midi.vid;
    pid = Midi.pid;
  }
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {
    sprintf(buf, "%08lX: ", millis());
    Serial.print(buf);
    Serial.print(rcvd);
    Serial.print(':');
    for (int i = 0; i < 64; i++) {
      sprintf(buf, " %02X", bufMidi[i]);
      Serial.print(buf);
    }
    Serial.println("");
  }
}

// Delay time (max 16383 us)
void doDelay(unsigned long t1, unsigned long t2, unsigned long delayTime)
{
  unsigned long t3;

  if ( t1 > t2 ) {
    t3 = (0xFFFFFFFF - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
