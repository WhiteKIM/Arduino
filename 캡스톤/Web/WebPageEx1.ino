#include <ESP8266WiFi.h>
boolean wifi_connection = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  eeprom_setup();
  wifi_setup();
}

void loop() 
{
  
  if(!wifi_connection)
  {
    web_start();
  }
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  delay(1000);
}
