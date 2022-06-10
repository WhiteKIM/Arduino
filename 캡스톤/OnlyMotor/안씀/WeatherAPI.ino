#include "time.h"
#include "FirebaseESP8266.h" 
#include <ArduinoJson.h>

String day2_temp;
String day2_pop;
String day2_pcp;
String day2_reh;
String day2_snow;
String day2_sky;
String day2_pty;

String day3_temp;
String day3_pop;
String day3_pcp;
String day3_reh;
String day3_snow;
String day3_sky;
String day3_pty;

void SetWeather(String category,String fcstDate, String fcstTime,String item)
{
    int usertime = user_data.toInt();
    int day2 = usertime + (1*100);
    int day3 = usertime + (2*100);
    String day2_str = (String)day2;
    String day3_str = (String)day3;

    if(fcstDate == day2_str)
    {
      if(fcstTime=="1200")
      {
        if(category=="TMP")
          day2_temp = item;
        if(category=="SKY")
          day2_sky = item;
        if(category=="PTY")
          day2_pty = item;
        if(category=="POP")
          day2_pop = item;
        if(category=="PCP")
          day2_pcp = item;
         if(category=="REH")
          day2_reh = item;
        if(category=="SNO")
          day2_snow = item;
      }
      else
      {
        //안씀
      }
    }
    else if(fcstDate == day3_str)
    {
      if(fcstTime=="1200")
      {
        if(category=="TMP")
          day3_temp = item;
        if(category=="SKY")
          day3_sky = item;
        if(category=="PTY")
          day3_pty = item;
        if(category=="POP")
          day3_pop = item;
        if(category=="PCP")
          day3_pcp = item;
         if(category=="REH")
          day3_reh = item;
        if(category=="SNO")
          day3_snow = item;
      }
      else
      {
        //안씀
      }
    }
    else
    {
      //안씀
    }
}

void sendWeather()
{
  SendData_Day2(day2_temp, day2_pop, day2_pcp, day2_reh, day2_snow, day2_sky, day2_pty);
  SendData_Day2(day3_temp, day3_pop, day3_pcp, day3_reh, day3_snow, day3_sky, day3_pty);
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
      DynamicJsonDocument doc(145516);
      Serial.println(http.getString());
      DeserializationError error = deserializeJson(doc, http.getString());
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
          const char* response_body_items_item_item_fcstDate = response_body_items_item_item["fcstDate"];
          const char* response_body_items_item_item_fcstTime = response_body_items_item_item["fcstTime"];
          const char* response_body_items_item_item_fcstValue = response_body_items_item_item["fcstValue"];
        
          SetWeather(String(response_body_items_item_item_category),String(response_body_items_item_item_fcstDate),String(response_body_items_item_item_fcstTime), String(response_body_items_item_item_fcstValue));
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
