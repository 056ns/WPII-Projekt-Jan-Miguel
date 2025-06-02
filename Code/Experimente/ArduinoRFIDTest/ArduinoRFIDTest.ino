/*
* Miguel's WP II Project:
* Datum: 17.05.2025
* RFID Audio Memory Game with category selection & volume control
* Components: MFRC522 RFID Reader, DFPlayer Mini, Buttons, Speaker, Arduino Uno
*
* Pin layout for RFID reader
* -------------------------------------------------------------------------------------------------------
*               MFRC522          Arduino       WP II
*               RFID Reader      Uno R3        Miguel
* Signal        Pin              Pin           Kommentare    
* -------------------------------------------------------------------------------------------------------
* RST/Reset     RST              9             Reset Pin
* SPI SS        SDA(SS)          10            Slave Select Pin
* SPI MOSI      MOSI             11            Datenfluss: Arduino --> RFID Leser (Master Out Slave In)
* SPI MISO      MISO             12            Datenfluss: RFID Leser --> Arduino (Master In Slave Out)    
* SPI SCK       SCK              13            Arduino --> RFID Leser Taktgeber         
*/


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9   // RST → D9 Reset
#define SS_PIN 10   // SDA → D10 Slave/Chip Select = SPI Protokoll, Aktiviert Kommunikation zum Leser

MFRC522 mfrc522(SS_PIN, RST_PIN); // Instanz des MFRC522 Objekts erzeugen


void setup() { 
  // Einmalige Ausführung
  Serial.begin(9600);     // Startet den Seriellen Monitor
  SPI.begin();            // SPI starten  Notwendig für die Kommunikation zum Leser
  mfrc522.PCD_Init();     // RFID starten
  Serial.println("RFID-Reader bereit. Bitte Karte vorhalten.");
}

void loop() {
  // Warten auf neue Karte
  if ( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() ) {
    return;
  }

  Serial.print("Karten UID: ");
  // Schleife wird 4 mal durchlaufen, wenn size = 4
  for (byte i = 0; i < mfrc522.uid.size; i++) { 
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  delay(3000); // Kurze Pause, um Mehrfachlesungen zu vermeiden

}
