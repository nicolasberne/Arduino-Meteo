bool writeData() {
  unsigned long ntpTime = getNTPTime();
  bool ok = false;

  if (ntpTime) {

#ifdef DEBUG
    Serial.print(ntpTime);
    Serial.print(F(" "));
#endif

    myFile = SD.open(FILENAME, FILE_WRITE);
    if (myFile) {

#ifdef DEBUG
      Serial.print(F("File open "));
      Serial.println(FILENAME);
#endif

      int chk = DHT.read11(DHT_PIN);
#ifdef DEBUG
      Serial.print(F("chk "));
      Serial.println(chk);
#endif

      if (DHT.temperature != -999 && DHT.humidity != -999) {
        myFile.print(ntpTime);
        myFile.print(';');
        myFile.print(DHT.temperature);
        myFile.print(';');
        myFile.println(DHT.humidity);
        ok = true;
      }
      myFile.close();
    } else {
      Serial.println(F("ERROR FILE"));
    }
  } else {
    Serial.println(F("ERROR NTP"));
  }

  return ok;
}

/*
void deleteDataFile() {

#ifdef DEBUG
  Serial.println(F("Remove file"));
#endif

  SD.remove(FILENAME);
}
*/

unsigned long getDataFileSize() {

#ifdef DEBUG
  Serial.println(F("Get file size"));
#endif

  unsigned long size = 0;
  myFile = SD.open(FILENAME);
  if (myFile) {
    size = myFile.size();
    myFile.close();
  }
  return size;
}

void printData() {

#ifdef DEBUG
  Serial.print(F("Print data "));
  Serial.println(FILENAME);
  int lineNb = 1;
  Serial.print(lineNb);
  Serial.print(F(". "));
#endif

  myFile = SD.open(FILENAME);
  if (myFile) {
    while (myFile.available()) {
      int r = myFile.read();
      Serial.write(r);

#ifdef DEBUG
      if (r == 10) {
        ++lineNb;
        Serial.print(lineNb);
        Serial.print(F(". "));
      }
#endif

    }

#ifdef DEBUG
    Serial.println();
#endif
    
    myFile.close();
  }
}

void newFilename() {
  
#ifdef DEBUG
  Serial.print(F("newFilename : "));
#endif

  unsigned long ntpTime = getNTPTime() / NTP_SECONDS_DIVISOR;
  if (ntpTime) {
    FILENAME = String(ntpTime) + F(".CSV");

#ifdef DEBUG
    Serial.print(ntpTime);
    Serial.print(F(" "));
    Serial.println(FILENAME);
#endif

  } else {
    Serial.println(F("ERROR NTP"));
  }
}
