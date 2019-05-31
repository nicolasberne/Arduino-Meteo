void doWriteData() {
  if (writeData()) {
    ++g_nbData;

#ifdef DEBUG
    Serial.print(F("nbDATA "));
    Serial.println(g_nbData);
#endif

    if (g_nbData >= NB_DATA) {

#ifdef DEBUG
      Serial.print(F("*** NB_DATA "));
      Serial.println(g_nbData);
#endif

      if (doFTP()) {
        newIndex();
        g_nbData = 0;
      } else {
        Serial.println(F("FTP ERROR NB_DATA"));
      }
    }
  }
}

bool writeData() {
  unsigned long ntpTime = getNTPTime();
  bool ok = false;

  if (ntpTime) {

#ifdef DEBUG
    Serial.print(ntpTime);
    Serial.print(F(" "));
#endif

    g_myFile = SD.open(g_dataFileName, FILE_WRITE);
    if (g_myFile) {

#ifdef DEBUG
      Serial.print(F("File open "));
      Serial.println(g_dataFileName);
#endif

      int chk = g_DHT.read11(DHT_PIN);
#ifdef DEBUG
      Serial.print(F("chk "));
      Serial.println(chk);
#endif

      if (g_DHT.temperature != -999 && g_DHT.humidity != -999) {
#ifdef DEBUG
        Serial.println(F("OK"));
#endif
        g_myFile.print(ntpTime);
        g_myFile.print(';');
        g_myFile.print(g_DHT.temperature);
        g_myFile.print(';');
        g_myFile.println(g_DHT.humidity);
        ok = true;
      } else {
#ifdef DEBUG
        Serial.println(F("KO"));
#endif
      }
      g_myFile.close();
    } else {
      Serial.println(F("ERROR FILE"));
    }
  } else {
    Serial.println(F("ERROR NTP"));
  }

  return ok;
}

unsigned long getDataFileSize() {

#ifdef DEBUG
  Serial.println(F("Get file size"));
#endif

  unsigned long size = 0;
  g_myFile = SD.open(g_dataFileName);
  if (g_myFile) {
    size = g_myFile.size();
    g_myFile.close();
  }
  return size;
}

void printData() {

#ifdef DEBUG
  Serial.print(F("Print data "));
  Serial.println(g_dataFileName);
  int lineNb = 1;
  Serial.print(lineNb);
  Serial.print(F(". "));
#endif

  g_myFile = SD.open(g_dataFileName);
  if (g_myFile) {
    while (g_myFile.available()) {
      int r = g_myFile.read();
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

    g_myFile.close();
  }
}

void readIndex() {

#ifdef DEBUG
  Serial.print(F("readIndex "));
#endif

  g_myFile = SD.open(FILE_INDEX);
  if (g_myFile) {
    byte msb = g_myFile.read();
    byte lsb = g_myFile.read();

    g_index = (msb << 8) + lsb;
    g_myFile.close();
  } else {
    g_index = 0;
  }

#ifdef DEBUG
  Serial.println(g_index);
#endif
  newFilenameByIndex();
}

void newIndex() {
  ++g_index;
#ifdef DEBUG
  Serial.print(F("newIndex "));
  Serial.println(g_index);
#endif

  if (SD.exists(FILE_INDEX)) {
    SD.remove(FILE_INDEX);
  }
  g_myFile = SD.open(FILE_INDEX, FILE_WRITE);
  if (g_myFile) {
    g_myFile.write(g_index >> 8);
    g_myFile.write(g_index & 0xFF);
    g_myFile.close();
  }
  newFilenameByIndex();
}

void newFilenameByIndex() {
#ifdef DEBUG
  Serial.print(F("newFilenameByIndex "));
#endif
  int index = g_index;

  for (int i = 7; i >= 0; --i) {
    g_dataFileName[i] = (char)(index % 10 + 48);
    index /= 10;
  }

#ifdef DEBUG
  Serial.println(g_dataFileName);
#endif

}
