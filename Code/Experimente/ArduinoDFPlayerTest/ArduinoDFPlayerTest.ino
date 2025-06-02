
#include <SoftwareSerial.h> //Standartmäßig enthalten
#include <DFRobotDFPlayerMini.h> // 1.0.6

SoftwareSerial mySoftwareSerial(7, 6); // D6 = Arduino-TX → DFPlayer RX, D7 = Arduino-RX ← DFPlayer TX
DFRobotDFPlayerMini myDFPlayer;


void setup() {
  // put your setup code here, to run once:
  mySoftwareSerial.begin(9600);  // DFPlayer arbeitet mit 9600 Baud
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

 
  if (!myDFPlayer.begin(mySoftwareSerial)) { // Check ob der Player da ist, wenn nicht dann gebe einen Fehler aus 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  } 

  Serial.println("DFPlayer Mini erfolgreich verbunden.");
  myDFPlayer.volume(20);         // Lautstärke (0 bis 30)
  myDFPlayer.play(6);            // Spiele Datei /mp3/0001.mp3
  
  delay(2000);
  myDFPlayer.play(5);            // Spiele Datei /mp3/0002.mp3

  delay(2000);
  myDFPlayer.play(2);            // Spiele Datei /mp3/0003.mp3

}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() >=0) {
    int myVar = Serial.read();
  
    switch (myVar) {
    
        case '1':
        myDFPlayer.play(1);
        break;

        case '2':
        myDFPlayer.play(2);
        break;

        case '3':
        myDFPlayer.playFolder(1, 1);  // Spiele bestimmte mp3 Datei aus einem Ordner; Ordner Name (1-99), Song name (1-255);
        break;


    }  
  
  
  }


}
