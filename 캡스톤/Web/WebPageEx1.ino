boolean wifi_connection = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello Main1");
  eeprom_setup();
  wifi_setup();
  Serial.println("Hello Main2");
}

void loop() 
{
  if(!wifi_connection)
  {
    web_start();
  }
}
