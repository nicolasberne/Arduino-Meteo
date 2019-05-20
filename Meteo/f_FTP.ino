bool doFTP() {

#ifdef DEBUG
  Serial.println(F("DO FTP"));
#endif

  myFile = SD.open(FILENAME, FILE_READ);

  if (!myFile) {
    Serial.println(F("SD open fail"));
    return false;
  }

  Serial.println(F("SD opened"));

  if (client.connect(server, 21)) {
    Serial.println(F("Command connected"));
  } else {
    myFile.close();
    Serial.println(F("Command connection failed"));
    return false;
  }
  if (!eRcv()) {
    return false;
  }

  client.println(F("USER arduino"));
  if (!eRcv()) {
    return false;
  }

  client.println(F("PASS arduino"));
  if (!eRcv()) {
    return false;
  }

  //client.println(F("CWD upload"));
  //if (!eRcv()) {
  //  return false;
  //}

  /* INUTILE...
  client.println(F("SYST"));
  if (!eRcv()) {
    return false;
  }
  */

  client.println(F("Type I"));
  if (!eRcv()) {
    return false;
  }

  client.println(F("PASV"));
  if (!eRcv()) {
    return false;
  }

  char *tStr = strtok(outBuf, "(,");
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

  if (dclient.connect(server, hiPort)) {
    Serial.println(F("Data connected"));
  } else {
    Serial.println(F("Data connection failed"));
    client.stop();
    myFile.close();
    return false;
  }

  client.print(F("STOR "));
  client.println(FILENAME);
  if (!eRcv()) {
    dclient.stop();
    return false;
  }

  byte clientBuf[64];
  int clientCount = 0;

#ifdef DEBUG
  Serial.println(F("Writing"));
  int nbBuf = 0;
#endif

  while (myFile.available()) {
    clientBuf[clientCount] = myFile.read();
    clientCount++;

    if (clientCount > 63) {
      dclient.write(clientBuf, 64);
      clientCount = 0;

#ifdef DEBUG
      nbBuf++;
      Serial.println(nbBuf);
#endif

    }
  }

  if (clientCount > 0) {
    dclient.write(clientBuf, clientCount);
  }

  dclient.stop();
  Serial.println(F("Data disconnected"));
  if (!eRcv()) {
    return false;
  }

  client.println(F("QUIT"));

  if (!eRcv()) {
    return false;
  }

  client.stop();
  Serial.println(F("Command disconnected"));

  myFile.close();
  Serial.println(F("Close"));

  return true;
}

bool eRcv() {
  byte respCode;
  byte thisByte;

  while (!client.available()) {
    delay(1);
  }

  respCode = client.peek();

  outCount = 0;

  while (client.available()) {
    thisByte = client.read();
    Serial.write(thisByte);

    if (outCount < 127) {
      outBuf[outCount] = thisByte;
      outCount++;
      outBuf[outCount] = 0;
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

  client.println(F("QUIT"));

  while (!client.available()) {
    delay(1);
  }

  while (client.available()) {
    thisByte = client.read();
    Serial.write(thisByte);
  }

  client.stop();
  Serial.println(F("FAIL - Command disconnected"));
  
  myFile.close();
  Serial.println(F("Close"));
}
