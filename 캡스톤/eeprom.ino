void eeprom_setup()
{
  EEPROM.begin(4096);
}

void eeprom_clear()
{
  for(int i=0; i<EEPROM.length(); i++)
  {
    EEPROM.write(i,0);
  }
}

void eeprom_write(int address, String data)
{
  for(int i=0; i< data.length(); i++)
  {
    EEPROM.write(address+i, data[i]);
  }
}

void eeprom_read(int address, int len_address, char * memory)
{
  uint8_t dataSize = EEPROM.read(len_address);
  if(dataSize < 21)
  {
    for(int i=0; i< dataSize; i++)
    {
      memory[i] = EEPROM.read(address+i);
      memory[dataSize]='\0';
    }
  }
}


void eeprom_wifi_write(String ssidTemp, String passTemp)
{
  EEPROM.write(ssid_length_address,ssidTemp.length());
  eeprom_write(ssid_address,ssidTemp);
  EEPROM.write(ssid_length_password, passTemp.length());
  eeprom_write(ssid_password, passTemp);
  EEPROM.commit();
  Serial.println("WIFI 설정 완료");
}

void eeprom_wifi_read()
{
  eeprom_read(ssid_address, ssid_length_address, ssid);
  eeprom_read(ssid_password, ssid_length_password, pass);
}
