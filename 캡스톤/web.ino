void web_setup()
{
  server.begin();
  Serial.println("Web Server started");
  server.onNotFound(handleNotFound);
  server.on("/", handleRoot);
  server.on("/main", handleRoot);
  server.on("/wifi", handleWifi);
  server.on("/save", handleSave);
  server.on("/r", handleRst);
}

void web_start()
{
  server.handleClient();
}

String urldecode(String str)
{
    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++)
    {
        c=str.charAt(i);
      if (c == '+')
      {
        encodedString+=' ';  
      }else if (c == '%') 
      {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } 
      else
      {
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}
