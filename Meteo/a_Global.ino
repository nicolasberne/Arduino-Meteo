#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SD.h>
#include <dht.h>

#define DEBUG

dht DHT;
const int DHT_PIN = 2; // D2

byte mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x2F, 0x19};
const IPAddress ip(192, 168, 1, 30);

// For FTP
const IPAddress server(192, 168, 1, 20);
EthernetClient client;
EthernetClient dclient;
char outBuf[128];
byte outCount;

// FOR NTP
const int LOCAL_PORT = 8888;
const char timeServer[] = "192.168.1.20";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
EthernetUDP Udp;

unsigned long lastTime;

//const unsigned long SECONDS_BETWEEN_1900_2019 = 3755289600UL; // (119*365 + 29) * 86400

const unsigned long DELAY_SAVE_DATA = 2ul * 60ul * 1000ul;
const int NB_DATA = 10;
const unsigned long NTP_SECONDS_DIVISOR = 3600; 
int nbData = 1;  // 1 car une premiere ecriture au lancement !!

const int DELAY_NTP = 500;
const int DELAY_RETRY_NTP = 2500;
const int NB_RETRY_NTP = 5;

const int SD_CHIP_SELECT = 4;

//const String FILENAME = "DATA.CSV";
String FILENAME;
File myFile;
