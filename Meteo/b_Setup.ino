
void setup() {
  Serial.begin(9600);
  
#ifdef DEBUG
  Serial.print(F("DELAY_SAVE_DATA "));
  Serial.println(DELAY_SAVE_DATA);
#endif

  Ethernet.begin(mac, ip);
  
#ifdef DEBUG
  Serial.print(F("server is at "));
  Serial.println(Ethernet.localIP());
#endif

  Udp.begin(LOCAL_PORT);

#ifdef DEBUG
  Serial.print(F("Initializing SD card..."));
#endif

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CHIP_SELECT)) {
    Serial.println(F("Card failed, or not present"));
    while (1);
  }
  
#ifdef DEBUG
  Serial.println(F("card initialized."));
#endif

  newFilename();
  writeData();
  lastTime = millis();
}
