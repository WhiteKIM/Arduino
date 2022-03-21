// 공통 html
const char HTTP_HEADER[] PROGMEM = "<!DOCTYPE html><html lang=\"en\"><head>"
                                  "<meta charset=\"utf-8\" name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\"/><link rel=\"icon\" href=\"data:,\">"
                                  "<title> Test Web </title>"; 
const char HTTP_STYLE[] PROGMEM = "<style>" 
                                  ".c{text-align:center;}div," 
                                  "input{padding:5px;font-size:1em;}input{width:95%;}" 
                                  "body{text-align:center;font-family:verdana;}" // max-width:300px; 
                                  "button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%} " 
                                  "</style>"; 
const char HTTP_HEAD_END[] PROGMEM = "</head><body><div style=\"text-align:center;display:inline-block;min-width:260px;\">"; 
const char HTTP_END[] PROGMEM = "</div></body></html>\r\n";

// 메뉴 html
const char HTTP_PORTAL_OPTIONS[] PROGMEM = "<form action=\"/wifi\"method=\"get\"><button>Configure WiFi</button></form><br/>";

//기본 html
const char HTTP_MAIN[] PROGMEM = "<form action=\"/main\"method=\"get\"><button>Return to Main</button></form><br/>";
const char HTTP_REBOOT[] PROGMEM = "<form action=\"/r\"method=\"post\"><button>Reboot</button></form><br/>";

// wifi html
const char HTTP_FORM_SSID[] PROGMEM = "<input id='ssid'name='ssid'length=20 placeholder='SSID'><br/>"; 
const char HTTP_FORM_PASS[] PROGMEM = "<input id='pass'name='pass'length=20 type='password'placeholder='password'><br/>";
const char HTTP_SSID_ERROR[] PROGMEM = "<div>Please input SSID</div><br/>";
const char HTTP_PASS_ERROR[] PROGMEM = "<div>Password should be at least 8 characters</div><br/>";
const char HTTP_WIFI_CONFIG[] PROGMEM = "<form action=\"/wifi\"method=\"get\"><button>Return to WiFi Configuration</button></form><br/>";

// form html
const char HTTP_FORM_SAVE[] PROGMEM = "<form method='get'action='save'>";  
const char HTTP_FORM_END[] PROGMEM = "<br/><button type='submit'>save</button></form>";
const char HTTP_SAVED[] PROGMEM = "<div>Configuration Saved<br/>Please Reboot for completing setting</div><br/>";
