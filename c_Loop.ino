void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= DELAY_SAVE_DATA) {

#ifdef DEBUG
    Serial.println(currentTime - lastTime);
#endif
    lastTime = currentTime;

    if (writeData()) {
      ++nbData;

#ifdef DEBUG
      Serial.print(F("nbDATA "));
      Serial.println(nbData);
#endif

      if (nbData >= NB_DATA) {

#ifdef DEBUG
        Serial.print(F("*** NB_DATA "));
        Serial.println(nbData);
#endif

        if (doFTP()) {
          newFilename();
          nbData = 0;
        } else {
          Serial.println(F("FTP ERROR NB_DATA"));
        }
      }
    }
  }

  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();

#ifdef DEBUG
    if (incomingByte > 31) {
      Serial.print(F("I received: "));
      Serial.println((char) incomingByte);
    }
#endif

    switch (incomingByte) {
      case 'D':
      case 'd':
        deleteDataFile();
        break;

      case 'g':
      case 'G':
        writeData();
        break;

      case 'p':
      case 'P':
        printData();
        break;

      case 's':
      case 'S':
#ifdef DEBUG
        Serial.print(F("File size :"));
#endif
        Serial.println(getDataFileSize());
        break;

      case 'f':
      case 'F':
        if (doFTP()) {
          newFilename();
        } else {
          Serial.println(F("FTP ERROR"));
        }
        break;

      case 'z':
      case 'Z':
        delay(1200);
        break;
    }
  }

}
