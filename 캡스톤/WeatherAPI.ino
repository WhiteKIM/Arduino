#include "time.h"
#include "FirebaseESP8266.h" 
#include <ArduinoJson.h>

void SetWeather(String category, String item)
{
    if(category=="TMP")
      temper = item;
    if(category=="VEC")
      Wind_Direction = item;
    if(category=="WSD")
      Wind_Speed= item;
   if(category=="SKY")
      Sky = item;
    if(category=="PTY")
      PTY=item;
    if(category=="POP")
      POP= item;
    if(category=="PCP")
      PCP= item;
    if(category=="REH")
      REH= item;
   if(category=="SNO")
      Snow = item;
}

void GetWeather()
{
  Serial.printf("현재 기온은 %s℃입니다 \n", temper);
  Serial.printf("현재 풍향은 %s 입니다 \n", Wind_Direction);
  Serial.printf("현재 풍속은 %sm/s입니다 \n", Wind_Speed);
  if(Sky=="1")
    Serial.printf("현재 날씨는 맑음입니다 \n");
   else if(Sky=="3")
    Serial.printf("현재 날씨는 구름많음입니다. \n");
   else
    Serial.printf("현재 날씨는 흐림입니다 \n");
  Serial.printf("강수 확률은 %s%% 입니다 \n", POP);
  if(PTY=="0")
    Serial.printf("비가 오지 않습니다 \n");
  else if(PTY=="1")
    Serial.printf("시간당 강수율은 %s%% 입니다 \n", PCP);
  else if(PTY=="2")
    Serial.printf("시간당 강수율은 %s%% 입니다 \n", PCP);
  else if(PTY=="3")
  {
    Serial.printf("현재 눈이 오고 있습니다 \n");
    Serial.printf("시간당 적설량은 %s cm입니다 \n", Snow);
  }
  else
    Serial.printf("시간당 강수율은 %s%% 입니다 \n", PCP);
  Serial.printf("현재 습도는 %s%% 입니다 \n", REH);
}

//Printing to Serial monital by present time 
// Notice
// Do not use first if you using first, time data not Set
void printLocalTime()
{
  time_t now = time(NULL);
  struct tm * timeinfo;
  timeinfo = localtime(&now);
  delay(1000);
  Serial.printf("현재시간 %d시%d%분%d초\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void GetTimeData()
{
  time_t now = time(NULL);
  struct tm * timeinfo;
  timeinfo = localtime(&now);
  delay(1000);
  int year =(timeinfo->tm_year)+1900;
  int mon = (timeinfo->tm_mon)+1;
  int day = (timeinfo->tm_mday);

  int result = year*10000+mon*100+day;
  user_data = String(result);
  
  Serial.printf("오늘날짜 %s \n",user_data);
}

//기상청 api호출을 위해 조회할 시간을 결정
void SetFcstTime()
{
  time_t now = time(NULL);
  struct tm * timeinfo;
  timeinfo = localtime(&now);
  delay(1000);
  int hour = (timeinfo->tm_hour);
  int minute = (timeinfo->tm_min);
  int result = hour*100+minute;
  nowTime = hour*100;
  Serial.println(nowTime);
  Serial.println(result);
  if(200<=result && result< 500)
  {
    fcsttime = "0200";
  }
  else if(500<= result&& result < 800)
  {
    fcsttime = "0500";
  }
  else if(800<= result&& result < 1100)
  {
    fcsttime = "0800";
  }
  else if(1100<= result&& result < 1400)
  {
    fcsttime = "1100";
  }
  else if(1400<= result&& result < 1700)
  {
    fcsttime = "1400";
  }
  else if(1700<= result && result< 2000)
  {
    fcsttime = "1700";
  }
  else if(2000<=result&& result < 2300)
  {
    fcsttime = "2000";
  }
  else
  {
    fcsttime = "2300";
  }
  Serial.println(fcsttime);
}

//Get Weather INFO From korea Meteorological Agency
//Only in 24hour can get date+user_data+x+y 
void getData()
{
  SetFcstTime();
  delay(1000);
  String url = server1+key+pageno+row+type+date+user_data+basetime+fcsttime+nx+x+ny+y;
  Serial.println(url);
  if(WiFi.status()==WL_CONNECTED)
  {
    Serial.println("Connecting to Server");
    HTTPClient http;
    http.begin(wificlient, url);
    int httpCode = http.GET();
    if(httpCode > 0)
    {
      DynamicJsonDocument doc(12288);
      
      String payload =http.getString();
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      JsonObject response_body = doc["response"]["body"];
      for (JsonObject response_body_items_item_item : response_body["items"]["item"].as<JsonArray>()) {
        const char* response_body_items_item_item_fcstTime = response_body_items_item_item["fcstTime"];
        String result =String(nowTime);
        if(String(response_body_items_item_item_fcstTime)==result)
        {
          const char* response_body_items_item_item_category = response_body_items_item_item["category"];
          const char* response_body_items_item_item_fcstValue = response_body_items_item_item["fcstValue"];
        
          SetWeather(String(response_body_items_item_item_category), String(response_body_items_item_item_fcstValue));
        }       
      }     
    }
    else
    {
      Serial.println("Error on HTTP request");
    }

     http.end();
  }
  //parsing();
}
