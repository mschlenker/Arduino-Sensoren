#include <Fat16.h>
#include <Fat16util.h> 

SdCard card;
Fat16 file;

unsigned long uptime;
unsigned long loopstart; 
unsigned long loopend; 
unsigned long delta;

/* Zahl in Datei schreiben */ 
void writeNumber(uint32_t n) {
  uint8_t buf[10];
  uint8_t i = 0;
  do {
    i++;
    buf[sizeof(buf) - i] = n%10 + '0';
    n /= 10;
  } while (n);
  file.write(&buf[sizeof(buf) - i], i);
}

void setup(void) {
  pinMode(8, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  // Karte und Laufwerk initialisieren
  if (!card.init()) Serial.println("Initializing card or reader failed.");
  if (!Fat16::init(&card)) Serial.println("Initializing volume failed.");
  // create a new file
  char name[] = "TEST_000.CSV";
  for (uint8_t i = 0; i < 1000; i++) {
    name[5] = i/100 + '0'; 
    name[6] = i/10 + '0';
    name[7] = i%10 + '0';
    if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
  }
}

void loop(void) {
  loopstart = millis();
  writeNumber(loopstart);
  Serial.println(loopstart);
  file.write("\n");
  digitalWrite(8, HIGH);   // LED an
  file.sync();
  digitalWrite(8, LOW);    // LED aus
  loopend = millis();
  delta = loopend - loopstart; 
  if (delta < 1000) { delay(1000 - delta); } 
}
