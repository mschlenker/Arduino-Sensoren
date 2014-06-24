#include <Fat16.h>
#include <Fat16util.h> 
#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>

SdCard card;
Fat16 file;
// DS3231 RTC;

unsigned long uptime;
unsigned long loopstart; 
unsigned long loopend; 
unsigned long delta;
int temp; 

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

void writeDate(tmElements_t tm) {
   writeNumber(tm.Hour);
   file.write(":");
   writeNumber(tm.Minute);
   file.write(":"); 
   writeNumber(tm.Second);
   file.write(";");
   writeNumber(tmYearToCalendar(tm.Year));
   file.write("-");
   writeNumber(tm.Month);
   file.write("-");
   writeNumber(tm.Day);
   file.write(";");
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
  tmElements_t tm;
  writeNumber(loopstart);
  // Serial.println(loopstart);
  file.write(";");
  if (RTC.read(tm)) {
    writeDate(tm); 
    /* temp = RTC.getTemperature() + 128;
    writeNumber(temp);
    file.write(";"); */
  } else {
     file.write(";");
     file.write(";");
  }
  temp = analogRead(3);
  Serial.println(temp);
  writeNumber(temp);
  file.write(";");
  file.write("\n"); 
  file.sync();
  loopend = millis();
  delta = loopend - loopstart; 
  if (delta < 1000) { delay(1000 - delta); } 
}
