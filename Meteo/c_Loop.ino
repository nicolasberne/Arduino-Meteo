void loop() {
  unsigned long currentTime = millis();

  if (currentTime - g_lastTime >= DELAY_SAVE_DATA) {

#ifdef DEBUG
    Serial.println(currentTime - g_lastTime);
#endif
    g_lastTime = currentTime;
    doWriteData();
  }

  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();

#ifdef DEBUG
    if (incomingByte > 31) {
      Serial.print(F("I received: "));
      Serial.println((char) incomingByte);
    }
#endif

    switch (incomingByte)  {
      case 'g':
      case 'G':
        doWriteData();
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
          
#ifdef DEBUG
          Serial.println(F("FTP OK"));
#endif
        } else {
          Serial.println(F("FTP ERROR"));
        }
        break;

      case 'z':
      case 'Z':
        delay(1200);
        break;

      case 'w':
      case 'W':
        newIndex();
        break;

      case 'r':
      case 'R':
        readIndex();
        break;
    }
  }
}
