#include <ESP8266WiFi.h>

void wifi_setup()
{
  eeprom_wifi_read();
  Serial.print("Saved Wifi :");
  Serial.println(ssid);
  if((String) ssid == "NO SSID")
  {
    Serial.println("Wifi not saved");
    get_mac();
    softAp_setup();
    web_setup();
    wifi_connection = false;
  }
  else
  {
    Serial.println("Wifi exist");
    if(!wifi_connect())
    {
      Serial.println("but not connect");
      get_mac();
      softAp_setup();
      web_setup();
      wifi_connection = false;
    }
  }
}

// wifi 클라이언트
boolean wifi_connect()
{
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  int count = 0;
  int retry = 0;
  boolean connection = true;
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    if(count > 30)
    {
      retry++;
      if(retry == 3)
      {
        connection = false;
        break;
      }
      Serial.println();
      check_wifi();
      Serial.println("retry in 3 seconds");
      count = 0;
      delay(3000);
    }
    else
    {
      Serial.print(".");
      delay(250);
      delay(250);
      count++;  
    }
  }
  if(connection)
  {
    Serial.println();
    Serial.println("Connected");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
  {
    Serial.println();
    Serial.println("Failed");
    return false;
  }
}

void check_wifi()
{
  int c = WiFi.status();
  switch(c)
  {
    case 0:
      Serial.println("Wi-Fi is in process of changing between statuses");
    break;
    case 1:
      Serial.println("configured SSID cannot be reached");
    break;
    case 3:
      Serial.println("WiFi connected");
    break;
    case 6:
      Serial.println("WiFi password is incorrect");
    break;
  }
}

// wifi AP 모드

void softAp_setup()
{
  delay(10);
  Serial.println();
  Serial.print("AP mode start....");
  if(WiFi.mode(WIFI_AP)){
    Serial.println("HEllo AP");
    Serial.println("Succeed");
  }
  else
  {        
    Serial.println("Failed");
  }
  Serial.print("Setting softAP....");
  if(WiFi.softAP(ssid_AP,pass_AP))
  {
    Serial.println("Succeed");
    Serial.println();
    Serial.print("AP SSID: ");
    Serial.println(ssid_AP);
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println("Failed");
    Serial.println("Password should be at least 8 character long");
  }
  Serial.print("Web Server Address: ");
  Serial.println(local_IP);
  Serial.println("MAC address: ");
  Serial.println(mac);
}

void get_mac()
{
  WiFi.macAddress(mac_array);
  for (int i = 0; i < sizeof(mac_array); i++)
  {
    if(i == sizeof(mac_array)-1)
    {
      sprintf(_mac, "%s%02x", _mac, mac_array[i]);
    }
    else
    {
      sprintf(_mac, "%s%02x-", _mac, mac_array[i]);
    }
  }
  String result(_mac);
  mac = result;
}
