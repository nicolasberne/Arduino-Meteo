#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SD.h>
#include <dht.h>

#define DEBUG

dht g_DHT;
const int DHT_PIN = 2; // D2

byte g_mac[] = {0xA8, 0x61, 0x0A, 0xAE, 0x2F, 0x19};
const IPAddress MY_IP(192, 168, 1, 30);

// For FTP
const IPAddress FTP_SERVER(192, 168, 1, 20);
EthernetClient FTP_CLIENT;
EthernetClient FTP_PASSIVE_CLIENT;
char g_outBuf[128];

// FOR NTP
const int NTP_LOCAL_PORT = 8888;
const char TIMESERVER[] = "192.168.1.20";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte g_packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
EthernetUDP NTP_UDP;

unsigned long g_lastTime;

const unsigned long DELAY_SAVE_DATA = 2ul * 60ul * 1000ul;
const int NB_DATA = 30;
int g_nbData;

const int DELAY_NTP = 500;
const int DELAY_RETRY_NTP = 2500;
const int NB_RETRY_NTP = 5;

const int SD_CHIP_SELECT = 4;

char g_dataFileName[] = "00000000.CSV";
const char FILE_INDEX[] = "INDEX.TXT";
const int BYTES_LINE = 24;
int g_index;

File g_myFile;
