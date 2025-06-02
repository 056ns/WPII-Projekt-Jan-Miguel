#include <SoftwareSerial.h> //Standartmäßig enthalten
#include <DFRobotDFPlayerMini.h> // 1.0.6
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // RST → D9 Reset
#define SS_PIN 10   // SDA → D10 Slave/Chip Select = SPI Protokoll, Aktiviert Kommunikation zum Leser

SoftwareSerial mySoftwareSerial(7, 6); // D6 = Arduino-TX → DFPlayer RX, D7 = Arduino-RX ← DFPlayer TX
DFRobotDFPlayerMini myDFPlayer;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instanz des MFRC522 Objekts erzeugen

int indexPaar;
String uidKarteA = "";
String uidKarteB = "";


// Get UID Anfang

/*
* Funktion um die UID aus der Karte auszulesen.
* Die Funktion gibt die UID in Hex Wert zurück (String).
*/
String getUID() {
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX); // Umwandlung von Dezimal nach Hex
  }
  uidString.trim(); // entfernt Leerzeichen vor und nach der UID
  uidString.toUpperCase();  // Optional: alles in Großbuchstaben
  return uidString;
}

// Leds

  const int LedGruen = 8; 
  const int LedRot = 5;

// Buttons

  const int buttonVolUp = 2;
  const int buttonVolDown = 3;
  const int buttonCatNext = 4;
  const int KAT_MAX = 6;
  const int KAT_MIN = 1;

  int spielKategorie = 1; // Kategorie Startwert (Folder)
  int currentVolume = 20; // Startlautstärke

// Beispiel-Array mit bekannten RFID-UIDs als Strings
String KartenListe[] = { // Liste fängt bei 0 an
    "B31ABB16",
    "63C06616",
    "33E36C16",
    "13986916",
    "43D56016",
    "C3A45D16",
    "331E7416",
    "C3B2BA16",
    "93536E16",
    "93836E16",
    "B3347616",
    "C3B05D16",
    "33B67616",
    "D3407716",
    "93E56716",
    "930AD327",
    "738F6416",
    "035C7916",
    "E33D5C16",
    "E3B9B816",
    "B3EAB116",
    "331A7716",
    "D33C8816",
    "939A6D16",
    "93D36716",
    "D3876316",
    "232DB316",
    "53E66716",
    "B397AE16",
    "43526016",
    "13F0B916",
    "33CD6016",
    "B3BE6A16",
    "D33B6B16",
    "53A57416",
    "4329BF16"
};



void setup() {
  // Anfang hier DFPlayer
  mySoftwareSerial.begin(9600);  // DFPlayer arbeitet mit 9600 Baud
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

   if (!myDFPlayer.begin(mySoftwareSerial)) { // Check ob der Player da ist, wenn nicht dann gebe einen Fehler aus 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true);
  } 

  Serial.println("DFPlayer Mini erfolgreich verbunden.");
  myDFPlayer.volume(20);         // Lautstärke (0 bis 30)

  // RFID Setup
  SPI.begin();            // SPI starten  Notwendig für die Kommunikation zum Leser
  mfrc522.PCD_Init();     // RFID starten
  Serial.println("RFID-Reader bereit. Bitte Karte vorhalten.");

  // Leds Setup

  pinMode(LedGruen, OUTPUT);
  pinMode(LedRot, OUTPUT);
  
  // Buttons Setup

  pinMode(buttonVolUp, INPUT_PULLUP);       // INPUT_PULLUP --> Wie ein input aber mit 20-50 K Wiederstand und wenn gedrückt auf Ground --> LOW
  pinMode(buttonVolDown, INPUT_PULLUP);
  pinMode(buttonCatNext, INPUT_PULLUP);
  Serial.println("Taster bereit.");
  myDFPlayer.volume(currentVolume);

}

void loop() {

  // Buttons Druckaktion
  // Button Lautstärke lauter
  if (digitalRead(buttonVolUp) == LOW) {
   if (currentVolume < 30) {
      currentVolume++;
      myDFPlayer.volume(currentVolume);
      Serial.print("Lautstärke erhöht: ");
      Serial.println(currentVolume);
      myDFPlayer.playFolder(9, currentVolume);  // Spielt 0001.mp3 bis 0030.mp3
      delay(500);  // Entprellen
  }}

  // Button Lautstärke leiser
  if (digitalRead(buttonVolDown) == LOW) {
   if (currentVolume > 1) {
      currentVolume--;
      myDFPlayer.volume(currentVolume);
      Serial.print("Lautstärke leiser: ");
      Serial.println(currentVolume);
      myDFPlayer.playFolder(9, currentVolume);  // Spielt 0001.mp3 bis 0030.mp3
      delay(500);  // Entprellen
  }}

  // Button Kategorie wechseln
  if (digitalRead(buttonCatNext) == LOW) {
    spielKategorie++;
    if (spielKategorie > KAT_MAX) spielKategorie = KAT_MIN;
    Serial.print("Kategorie: ");
    Serial.println(spielKategorie);
    myDFPlayer.playFolder(8, spielKategorie);  // Spielt 0001.mp3 bis 0030.mp3
    delay(200);
  }

  // RFID Kartenleseraktion

  // Warten auf neue Karte
  if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() ) {
    return;
  }

  String uid = getUID();
  Serial.print("Erkannte UID: ");
  Serial.println(uid);

    // Gehe durch die Kartenliste und überprüfe jede Karte mit deiner neu eingelesenen
  for (int indexKarte = 0; indexKarte < 36; indexKarte++) {
      Serial.print(indexKarte);
    if (uid == KartenListe[indexKarte]) {
         myDFPlayer.playFolder(spielKategorie, indexKarte + 1);
         delay(2500);
         Serial.println("Spiele Audio aus ");
         Serial.print("Kategorie: ");
         Serial.println(spielKategorie);
         delay(300);

        // Überprüfe ob die Zahl gerade oder ungerade ist, dabei wird Modulo verwendet (Rest nach Divsion)
        if (indexKarte % 2 == 0) {
          indexPaar =  indexKarte + 1;   // gerade indexZahl + 1 nehmen um das indexPaar zu der UID zu haben
        }
        else {
          indexPaar =  indexKarte - 1;   // ungerade indexZahl - 1 nehmen um das indexPaar zu der UID zu haben 
        } 
    }
  }

  // Von Index in UID umwandeln
  if (uidKarteA == "") {
    uidKarteA = uid;
    uidKarteB = KartenListe[indexPaar];  
    Serial.print("UID von der ersten Karte: ");
    Serial.println(uidKarteA);
    Serial.print("UID vom Paar: ");
    Serial.println(uidKarteB);
  }
  else {
    if (uidKarteA != uid) {
      Serial.print("uid von der zweiten Karte: ");
      Serial.println(uid);

      Serial.print("paar von der ersten Karte: ");
      Serial.println(uidKarteB);

        if(uid == uidKarteB) {
          Serial.println("Herzlichen Glückwunsch!!! Richtiges Pärchen gefunden!");
          digitalWrite(LedGruen, HIGH);
          myDFPlayer.playFolder(8, 9);  // Gewinner Musik
          delay(3000);
          digitalWrite(LedGruen, LOW); 

          // Reset für das nächste Paar 
          uidKarteA = "";
          uidKarteB = ""; 
        }
        else {
          Serial.println("Verkackt!");
          digitalWrite(LedRot, HIGH);
          myDFPlayer.playFolder(8, 8); // Verlierer Musik
          delay(3000);
          digitalWrite(LedRot, LOW);

          // Reset für das nächste Paar 
          uidKarteA = "";
          uidKarteB = ""; 
        }
    }
  }
}
/*
  if (uid == "B31ABB16") {
  Serial.println("Spiele MP3 1: Becker");
  myDFPlayer.playFolder(1, 1);  // Datei 0001.mp3
  }
  else if (uid == "63C06616") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 2);  // Datei 0002.mp3
  }
  else if (uid == "33E36C16") {
  Serial.println("Spiele MP3 3: Drei");
  myDFPlayer.playFolder(1, 3);  // Datei 0003.mp3
  }
  else if (uid == "13986916") {
  Serial.println("Spiele MP3 4: Vier");
  myDFPlayer.playFolder(1, 4); // Datei 0004.mp3
  }
  else if (uid == "43D56016") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 5);
  }
  else if (uid == "C3A45D16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 6);  // Datei 0002.mp3
  }
  else if (uid == "331E7416") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 7);  // Datei 0002.mp3
  }
  else if (uid == "C3B2BA16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 8);  // Datei 0002.mp3
  }
  else if (uid == "93536E16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 9);  // Datei 0002.mp3
  }
  else if (uid == "93836E16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 10);  // Datei 0002.mp3
  }
  else if (uid == "B3347616") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 11);  // Datei 0002.mp3
  }
  else if (uid == "C3B05D16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 12);  // Datei 0002.mp3
  }
  else if (uid == "33B67616") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 13);  // Datei 0002.mp3
  }
  else if (uid == "D3407716") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 14);  // Datei 0002.mp3
  }
  else if (uid == "93E56716") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 15);  // Datei 0002.mp3
  }
  else if (uid == "930AD327") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 16);  // Datei 0002.mp3
  }
  else if (uid == "738F6416") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 17);  // Datei 0002.mp3
  }
  else if (uid == "035C7916") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 18);  // Datei 0002.mp3
  }
  else if (uid == "E33D5C16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 19);  // Datei 0002.mp3
  }
  else if (uid == "E3B9B816") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 20);  // Datei 0002.mp3
  }
  else if (uid == "B3EAB116") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 21);  // Datei 0002.mp3
  }
  else if (uid == "331A7716") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 22);  // Datei 0002.mp3
  }
  else if (uid == "D33C8816") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 23);  // Datei 0002.mp3
  }
  else if (uid == "939A6D16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 24);  // Datei 0002.mp3
  }
  else if (uid == "93D36716") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 25);  // Datei 0002.mp3
  }
  else if (uid == "D3876316") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 26);  // Datei 0002.mp3
  }
  else if (uid == "232DB316") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 27);  // Datei 0002.mp3
  }
  else if (uid == "53E66716") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 28);  // Datei 0002.mp3
  }
  else if (uid == "B397AE16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 29);  // Datei 0002.mp3
  }
  else if (uid == "43526016") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 30);  // Datei 0002.mp3
  }
  else if (uid == "13F0B916") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 31);  // Datei 0002.mp3
  }
  else if (uid == "33CD6016") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 32);  // Datei 0002.mp3
  }
  else if (uid == "B3BE6A16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 33);  // Datei 0002.mp3
  }
  else if (uid == "D33B6B16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 34);  // Datei 0002.mp3
  }
  else if (uid == "53A57416") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 35);  // Datei 0002.mp3
  }
  else if (uid == "4329BF16") {
  Serial.println("Spiele MP3 2: Zwei");
  myDFPlayer.playFolder(1, 36);  // Datei 0002.mp3
  }
  else {
  Serial.println("Unbekannte Karte.");
  }
   delay(3000); // Kurze Pause, um Mehrfachlesungen zu vermeiden
}

*/


