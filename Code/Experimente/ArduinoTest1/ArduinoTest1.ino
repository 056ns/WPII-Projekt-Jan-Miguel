#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10   // SDA → D10 Slave/Chip Select = SPI Protokoll, Kommunikation
#define RST_PIN 9   // RST → D9 Reset

MFRC522 mfrc522(SS_PIN, RST_PIN);


void setup() {
  Serial.begin(9600);
  SPI.begin();            // SPI starten  Notwendig für die Kommunikation zum Leser
  mfrc522.PCD_Init();     // RFID initialisieren
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

  delay(1000); // Kurze Pause, um Mehrfachlesungen zu vermeiden

}
