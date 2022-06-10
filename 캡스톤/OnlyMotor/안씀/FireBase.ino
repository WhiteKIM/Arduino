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
//다음날 12시 날씨 전송
void SendData_Day2(String tempe, String POP1, String PCP1, String REH1, String Snow1, String sky, String PTY)
{
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/temper", tempe);
  if(sky=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/sky", "맑음");
  else if(sky=="3")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/sky", "구름많음");
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/sky", "흐림");
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/probability", POP1);
  
  if(PTY=="0")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/precipitation", "0");
  else if(PTY=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/precipitation", PCP1);
  else if(PTY=="2")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/precipitation", PCP1);
  else if(PTY=="3")
  {
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/snow", Snow1);
  }
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/precipitation", PCP1);
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day2/humidity", REH1);
}
//2일뒤 날씨 전송
void SendData_Day3(String tempe, String POP1, String PCP1, String REH1, String Snow1, String sky, String PTY)
{
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/temper", tempe);
  if(sky=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/sky", "맑음");
  else if(sky=="3")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/sky", "구름많음");
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/sky", "흐림");
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/probability", POP1);
  
  if(PTY=="0")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/precipitation", "0");
  else if(PTY=="1")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/precipitation", PCP1);
  else if(PTY=="2")
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/precipitation", PCP1);
  else if(PTY=="3")
  {
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/snow", Snow1);
  }
  else
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/precipitation", PCP1);
    
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/day3/humidity", REH1);
}
