#include "mp3tf16p.h"


MP3Player mp3(7,6); // RX vom Arduino , TX vom Arduino



void setup() {
  Serial.begin(9600);
  mp3.initialize();
  mp3.playTrackNumber(6, 25);
}

void loop() {
  mp3.serialPrintStatus(MP3_ALL_MESSAGE);



}
