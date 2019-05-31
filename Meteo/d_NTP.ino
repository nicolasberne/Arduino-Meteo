unsigned long getNTPTime() {
  unsigned long seconds = 0;
  for (int i = 0; i < NB_RETRY_NTP; ++i) {
    sendNTPpacket(TIMESERVER);
    delay(DELAY_NTP);

    if (NTP_UDP.parsePacket()) {
      NTP_UDP.read(g_packetBuffer, NTP_PACKET_SIZE);
      unsigned long highWord = word(g_packetBuffer[40], g_packetBuffer[41]);
      unsigned long lowWord = word(g_packetBuffer[42], g_packetBuffer[43]);
      // combine the four bytes (two words) into a long integer
      // this is NTP time (seconds since Jan 1 1900):
      //  seconds = highWord << 16 | lowWord - SECONDS_BETWEEN_1900_2019;
      seconds = highWord << 16 | lowWord;
      break;
    }
    
#ifdef DEBUG
    Serial.print(F("NTP try "));
    Serial.println(i);
#endif
    delay(DELAY_RETRY_NTP);
  }

  return seconds;
}

void sendNTPpacket(const char * address) {
  // set all bytes in the buffer to 0
  memset(g_packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  g_packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  g_packetBuffer[1] = 0;     // Stratum, or type of clock
  g_packetBuffer[2] = 6;     // Polling Interval
  g_packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  g_packetBuffer[12]  = 49;
  g_packetBuffer[13]  = 0x4E;
  g_packetBuffer[14]  = 49;
  g_packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  NTP_UDP.beginPacket(address, 123); // NTP requests are to port 123
  NTP_UDP.write(g_packetBuffer, NTP_PACKET_SIZE);
  NTP_UDP.endPacket();
}
