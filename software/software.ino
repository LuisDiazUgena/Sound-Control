
#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability

/*
Based on Teensy examples: encoder and USB Media buttons
*/


Encoder myEnc(5, 6);
int pinBtn = 7;
int pinledLong = 13;

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

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");

  pinMode(pinBtn, INPUT);
  pinMode(pinledLong, OUTPUT);

}


void loop() {

  nextTime = millis() + intervale;
  while (digitalRead(pinBtn)) {
    clicked = true;
    if (millis() > nextTime) {
      digitalWrite(pinledLong, HIGH);
    }
  }
  digitalWrite(pinledLong, LOW);
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
      Serial.print("diference = ");
      Serial.println((newPosition-oldPosition));
      oldPosition = newPosition;
    }
    if (newPosition < (oldPosition-rotation)) {
      decreaseVol();
      Serial.print("diference = ");
      Serial.println((oldPosition - newPosition));
      oldPosition = newPosition;
    }
  }
}
void nextSong() {
  Serial.println("next");
  Keyboard.set_media(KEY_MEDIA_NEXT_TRACK);
  send();
}
void playPause() {
  Serial.println("playPause");
  Keyboard.set_media(KEY_MEDIA_PLAY_PAUSE);
  send();
}
void increaseVol() {
  Serial.println("Vol+");
  Keyboard.set_media(KEY_MEDIA_VOLUME_INC);
  send();
}

void decreaseVol() {
  Serial.println("Vol-");
  Keyboard.set_media(KEY_MEDIA_VOLUME_DEC);
  send();
}

void send() {
  Keyboard.send_now();
  Keyboard.set_media(0);
  Keyboard.send_now();
}
