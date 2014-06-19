/*

Simple web server that answers any incoming packet on port 80
with a plain text page presenting temperature and humidity
read from a DHT22 sensor.

Triple licensed unter GPL v3.0, LGPL v2.1, MIT - choose one
that suits your needs!

Uses DHTlib from Rob Tillaard:
https://github.com/RobTillaart/Arduino 
Tested revision: 1f2314cdbf18a09d847796862685fd5943708ea1

Uses UIPEthernet from Norbert Truchsess:
https://github.com/ntruchsess/arduino_uip
Git Cloned on: 2014-04-03

*/


#include <UIPEthernet.h>
#include <dht.h>

#define DHT22_PIN 6

const char header[] = "HTTP/1.0 200 OK\r\n" 
  "Content-Type: text/plain\r\n\r\nLuftfeuchte: ";
const byte myMac[6] = { 0x23, 0x23, 0xDE, 0xAD, 0xBE, 0xEF };
const IPAddress myIP(192,168,2,5);

EthernetServer server = EthernetServer(80);
dht DHT;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(myMac, myIP);
  server.begin();
  Serial.println("\nListening...");
}

void loop() {
  if (EthernetClient client = server.available()) {
    Serial.println("\nAnswering...");
    client.print(header);
    int chk = DHT.read22(DHT22_PIN);
    client.print(DHT.humidity, 1);
    client.print("\r\nTemperatur: ");
    client.print(DHT.temperature, 1);
    client.print("\r\n");
    client.stop();
    Serial.println("\nDisconnect...");
  }
}
