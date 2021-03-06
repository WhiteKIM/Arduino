#include "FirebaseESP8266.h"

//빗물 센서이용 함수
void RainySensor()
{
  if(analogRead(d6) < 400)
  {
    Serial.println("no water");
    damp = false;
    delay(1000);
  }
  else
  {
    Serial.println("water");
    damp = true;
    delay(1000);
  }
}

//비온다 우산가지고가
void takeUmbrella()
{
  if(PTY=="1" && umbrella && isHuman)
  {
    //비가 오면 우산을 가져가요
    Firebase.setInt(firebaseData, "DBConnect/Users/10001/take_state", 1);
    delay(1000);
    IRSensor(); //가져가는지 확인
    delay(1000);
  }
  Firebase.setInt(firebaseData, "DBConnect/Users/10001/take_state", 0);
}

// 적외선 인체감지센서
void HumanSensor()
{
  if(LOW==digitalRead(d5))
  {
    isHuman = true;
    takeUmbrella();
    Serial.println("human");
  }
  else
  {
    isHuman = false;
    Serial.println("no human");
  }
}

//우산을 감지하는 함수
void IRSensor()
{
  if(LOW== digitalRead(d3))
  {
    Serial.println("umb");
    umbrella = true;
  }
  else
  {
    Serial.println("no umb");
    umbrella = false;
  }
}

//모터 구동부
//사용시 15분간 모터를 구동함
void motor()
{
  int fif = 1000*60*15;
  unsigned long activation_time = millis();
  //모터 사용 조건
  //우산이 존재하고 사용되었을때
  while(activation_time < fif && umbrella && damp)
  {
    Firebase.setInt(firebaseData,"/DBConnect/Users/10001/run_motor", 1);
    Firebase.setString(firebaseData,"/DBConnect/Users/10001/state", "on");
    digitalWrite(d7 ,HIGH);
    IRSensor();
    RainySensor();      
  }
  Firebase.setInt(firebaseData,"/DBConnect/Users/10001/run_motor", 0);
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/state", "off");
  digitalWrite(d7 ,LOW);
}
//비가 안온데요 그래서 테스트모드가 필요해요
void TestMode()
{
  PTY = "1";
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/temper", "5");
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/sky", "흐림");   
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/probability", "100");
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/precipitation", "5");
  Firebase.setString(firebaseData,"/DBConnect/Users/10001/weather/humidity", "90");
}
