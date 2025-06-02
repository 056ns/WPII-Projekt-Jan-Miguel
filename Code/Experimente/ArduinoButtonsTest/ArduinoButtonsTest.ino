const int buttonVolUp = 2;
const int buttonVolDown = 3;
const int buttonCatNext = 4;
const int buttonCatPrev = 5;

const int KAT_MAX = 2;
const int KAT_MIN = 1;

int spielKategorie = 1; // Kategorie Startwert

int volume = 20; // Startlautstärke


void setup() {
  Serial.begin(9600);

  pinMode(buttonVolUp, INPUT_PULLUP);       // INPUT_PULLUP --> Wie ein input aber mit 20-50 K Wiederstand und wenn gedrückt auf Ground --> LOW
  pinMode(buttonVolDown, INPUT_PULLUP);
  pinMode(buttonCatNext, INPUT_PULLUP);
  pinMode(buttonCatPrev, INPUT_PULLUP);

  Serial.println("Taster bereit.");
}

void loop() {
  
  if (digitalRead(buttonVolUp) == LOW) {
    volume = min(volume + 1, 30);
    Serial.print("Lauter: ");
    Serial.println(volume);
    delay(200); // Entprellung
  }

  if (digitalRead(buttonVolDown) == LOW) {
    volume = max(volume - 1, 0);
    Serial.print("Leiser: ");
    Serial.println(volume);
    delay(200);
  }

  if (digitalRead(buttonCatNext) == LOW) {
    spielKategorie++;
    if (spielKategorie > KAT_MAX) spielKategorie = KAT_MIN;
    Serial.println("Kategorie: ");
    printKategorie();
    delay(200);
  }

  if (digitalRead(buttonCatPrev) == LOW) {
    spielKategorie--;
    if (spielKategorie < KAT_MIN) spielKategorie = KAT_MAX;
    Serial.println("Kategorie ");
    printKategorie();
    delay(200);
  }

}


// Eigene Funktionen

void printKategorie() {
  if (spielKategorie == 1) {
    Serial.println("Land und Hauptstadt");
  } else if (spielKategorie == 2) {
    Serial.println("Lehrer und Vornamen");
  }
}