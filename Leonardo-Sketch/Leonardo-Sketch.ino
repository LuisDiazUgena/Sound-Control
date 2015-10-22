#include "HID-Project.h"
#include <Encoder.h>

Encoder myEnc(5, 6);
int pinBtn = 7;
int pinLedLong = 13;

bool clicked = false;

long oldPosition  = -999;

int userDelay = 250;
unsigned long nextTime;
int intervale = 1000;

// Number of mechanichal clicks of the encoder to increase or decrease
//the volume of the computer.
int mechClicks = 2;
int pulsesPerClick = 4; // Electrical pulses per mechanichal pulse

int rotation = (pulsesPerClick*mechClicks) - 1;

void setup(){
  pinMode(pinBtn, INPUT);
  pinMode(pinLedLong, OUTPUT);

  // Sends a clean report to the host. This is important on any Arduino type.
  Consumer.begin();
}

void loop() {

    nextTime = millis() + intervale;
    while (digitalRead(pinBtn)) {
      clicked = true;
      if (millis() > nextTime) {
        digitalWrite(pinLedLong, HIGH);
      }
    }
    digitalWrite(pinLedLong, LOW);
    if (clicked) {
      if (millis() > nextTime) {
        //Long click
        nextSong();
      } else {
        //Short click
        playPause();
      }
      delay(userDelay);//Debouncing
      clicked = false;
    }

    //encoder rotation
    long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
      if (newPosition > (oldPosition + rotation)) {
        increaseVol();
        oldPosition = newPosition;
      }
      if (newPosition < (oldPosition - rotation)) {
        decreaseVol();
        oldPosition = newPosition;
      }
    }
  }
  void nextSong() {
    Consumer.write(MEDIA_NEXT);
  }
  void playPause() {
    Consumer.write(MEDIA_PLAY_PAUSE);
  }
  void increaseVol() {
    Consumer.write(MEDIA_VOLUME_UP);
  }

  void decreaseVol() {
    Consumer.write(MEDIA_VOLUME_DOWN);
  }
