String Common_page()
{
  String page;
  page = (const __FlashStringHelper *)HTTP_HEADER;
  page += (const __FlashStringHelper *)HTTP_STYLE;
  page += (const __FlashStringHelper *)HTTP_HEAD_END;

  String tem[3];
  tem[0] = "<p>SSID: ";
  tem[1] = ssid;
  tem[2] = "</p>";

  for(int i=0; i<3; i++){
    page += tem[i];
  }
  
  return page;
}
