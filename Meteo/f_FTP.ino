bool doFTP() {

#ifdef DEBUG
  Serial.println(F("DO FTP"));
#endif

  g_myFile = SD.open(g_dataFileName, FILE_READ);

  if (!g_myFile) {
    Serial.println(F("SD open fail"));
    return false;
  }
  
#ifdef DEBUG
  Serial.println(F("SD opened"));
#endif

  bool ret = doFTP_bis();

  g_myFile.close();

#ifdef DEBUG
  Serial.println(F("Close"));
#endif

  return ret;
}

bool doFTP_bis() {

  if (FTP_CLIENT.connect(FTP_SERVER, 21)) {
    Serial.println(F("Command connected"));
  } else {
    Serial.println(F("Command connection failed"));
    return false;
  }
  if (!eRcv()) {
    return false;
  }

  bool ret = doFTP_ter();

  FTP_CLIENT.stop();
  Serial.println(F("Command disconnected"));
  
  return ret;
}

bool doFTP_ter() {
  FTP_CLIENT.println(F("USER arduino"));
  if (!eRcv()) {
    return false;
  }

  FTP_CLIENT.println(F("PASS arduino"));
  if (!eRcv()) {
    return false;
  }

  FTP_CLIENT.println(F("Type I"));
  if (!eRcv()) {
    return false;
  }

  FTP_CLIENT.println(F("PASV"));
  if (!eRcv()) {
    return false;
  }

  char *tStr = strtok(g_outBuf, "(,");
  int array_pasv[6];
  for ( int i = 0; i < 6; i++) {
    tStr = strtok(NULL, "(,");
    array_pasv[i] = atoi(tStr);
    if (tStr == NULL) {
      Serial.println(F("Bad PASV Answer"));
      return false;
    }
  }

  unsigned int hiPort = (array_pasv[4] << 8) | (array_pasv[5] & 255);

#ifdef DEBUG
  Serial.print(F("Data port: "));
  Serial.println(hiPort);
#endif

  if (FTP_PASSIVE_CLIENT.connect(FTP_SERVER, hiPort)) {
    Serial.println(F("Data connected"));
  } else {
    Serial.println(F("Data connection failed"));
    return false;
  }

  FTP_CLIENT.print(F("STOR "));
  FTP_CLIENT.println(g_dataFileName);
  if (!eRcv()) {
    FTP_PASSIVE_CLIENT.stop();
    return false;
  }

  byte clientBuf[64];
  int clientCount = 0;

#ifdef DEBUG
  Serial.println(F("SEND DATA"));
  int nbBuf = 0;
#endif

  while (g_myFile.available()) {
    clientBuf[clientCount] = g_myFile.read();
    clientCount++;

    if (clientCount > 63) {
      FTP_PASSIVE_CLIENT.write(clientBuf, 64);
      clientCount = 0;

#ifdef DEBUG
      nbBuf++;
      Serial.println(nbBuf);
#endif

    }
  }

  if (clientCount > 0) {
    FTP_PASSIVE_CLIENT.write(clientBuf, clientCount);
  }

  FTP_PASSIVE_CLIENT.stop();
  Serial.println(F("Data disconnected"));
  if (!eRcv()) {
    return false;
  }

  FTP_CLIENT.println(F("QUIT"));
  if (!eRcv()) {
    return false;
  }
  
  return true;
}

bool eRcv() {
  byte respCode;
  byte thisByte;

  while (!FTP_CLIENT.available()) {
    delay(1);
  }

  respCode = FTP_CLIENT.peek();

  byte outCount = 0;

  while (FTP_CLIENT.available()) {
    thisByte = FTP_CLIENT.read();
    Serial.write(thisByte);

    if (outCount < 127) {
      g_outBuf[outCount] = thisByte;
      outCount++;
      g_outBuf[outCount] = 0;
    }
  }

  if (respCode >= '4') {
    efail();
    return false;
  }

  return true;
}


void efail() {
  byte thisByte = 0;

  FTP_CLIENT.println(F("QUIT"));

  while (!FTP_CLIENT.available()) {
    delay(1);
  }

  while (FTP_CLIENT.available()) {
    thisByte = FTP_CLIENT.read();
    Serial.write(thisByte);
  }

  FTP_CLIENT.stop();
  Serial.println(F("FAIL - Command disconnected"));

  g_myFile.close();
  Serial.println(F("Close"));
}
