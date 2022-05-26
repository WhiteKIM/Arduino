void handleNotFound() 
{
  String message = "Page Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleRoot() 
{
  String page = Common_page();
  page+= (const __FlashStringHelper *)HTTP_PORTAL_OPTIONS;
  page+= (const __FlashStringHelper *)HTTP_REBOOT;
  page+= (const __FlashStringHelper *)HTTP_END;
  server.send(200, "text/html", page);
}

void handleWifi() 
{
  String page = Common_page();
  page+= (const __FlashStringHelper *)HTTP_MAIN;
  page+= (const __FlashStringHelper *)HTTP_FORM_SAVE;
  page+= (const __FlashStringHelper *)HTTP_FORM_SSID;
  page+= (const __FlashStringHelper *)HTTP_FORM_PASS;
  page+= (const __FlashStringHelper *)HTTP_FORM_END;
  page+= (const __FlashStringHelper *)HTTP_END;
  server.send(200, "text/html", page);
}

void handleSave() 
{
  String ssidTemp = server.arg("ssid");
  String passTemp = server.arg("pass");

  String page;
  
  if(ssidTemp == "")
  {
    page = Common_page();
    page+= (const __FlashStringHelper *)HTTP_SSID_ERROR;
    page+= (const __FlashStringHelper *)HTTP_WIFI_CONFIG;
  }
  else if(passTemp.length() < 8)
  {
    page = Common_page();
    page+= (const __FlashStringHelper *)HTTP_PASS_ERROR;
    page+= (const __FlashStringHelper *)HTTP_WIFI_CONFIG;
  }
  else{
    eeprom_wifi_write(urldecode(ssidTemp),urldecode(passTemp));
    eeprom_wifi_read();
    
    page = Common_page();
    page+= (const __FlashStringHelper *)HTTP_SAVED;
    page+= (const __FlashStringHelper *)HTTP_MAIN;
    page+= (const __FlashStringHelper *)HTTP_REBOOT;
  }
  
  page+= (const __FlashStringHelper *)HTTP_END;

  server.send(200, "text/html", page);
}

void handleRst() 
{
  String page = Common_page();
  String tem = "<p> Device will be reboot.....</p>";
  page += tem;
  page+= (const __FlashStringHelper *)HTTP_END;
  server.send(200, "text/html", page);
  delay(100);
  ESP.restart();
}
