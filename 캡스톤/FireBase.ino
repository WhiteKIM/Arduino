#include "FirebaseESP8266.h" 

//DB에서 위치 정보 가져오기
void GetLocationData()
{
  String db = "internet-of-umbrella-default-rtdb/DBConnect/Users/10001/location";
  String lang = "latitude";
  String lon = "longtitude";
 if(Firebase.getString(firebaseData, "DBConnect/Users/10001/location/latitude"))
 {
   latit = (firebaseData.stringData()).toDouble();
   Serial.println(latit);
 }
 if(Firebase.getString(firebaseData, "DBConnect/Users/10001/location/longitude"))
 {
   longi = (firebaseData.stringData()).toDouble();
   Serial.println(longi);
 }
}

//DB에 날씨 정보 전송
void SendWeatherData()
{
  String tempe = String(temper);
  String POP1 = String(POP);
  String PCP1 = String(PCP);
  String REH1 = String(REH);
  String Snow1= String(Snow);
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/temper", tempe);
  if(Sky=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/sky", "맑음");
  else if(Sky=="3")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/sky", "구름많음");
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/sky", "흐림");
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/probability", POP1);
  
  if(PTY=="0")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/precipitation", "0");
  else if(PTY=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/precipitation", PCP1);
  else if(PTY=="2")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/precipitation", PCP1);
  else if(PTY=="3")
  {
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/snow", Snow1);
  }
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/precipitation", PCP1);
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/humidity", REH1);
}
