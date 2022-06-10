/*
* @ Author by Whitekim
* @ 2022.06.08
* @ 모터와 스피커를 통한 기능 구현
* @ 날씨 변수와 위치 변수는 
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "FirebaseESP8266.h" 
#include "time.h"

#define FIREBASE_HOST "internet-of-umbrella-default-rtdb.firebaseio.com/" //enter your firebase project url
#define FIREBASE_AUTH "lTEDV27cNLqMUsdwWOQY4vX6LxajTNWH46N4RDuJ" //use to firebase access key
#define NX 149 /* X축 격자점 수 */
#define NY 253 /* Y축 격자점 수 */

using namespace std;

int d4 = 4;  //모터1
int d5 = 14; //모터1
int d6 = 12; //모터2
int d7 = 13; // 모터2
int d8 = 0; //스피커

const char * ssid = "Simpletest";
const char * pass = "simpletest";

//날씨 api 주소
const String server1 ="http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey="; // korea weather api host url
const String key = "qZCP9%2BU9bBXMk8BDFspW8MdaxGUGL0Vt4nvgA6%2FLdVb9XD6Sa8X05NLpnx60iOisK2WMN7C7nIFlvUzScJYQ1g%3D%3D";  // data.go.kr 일반인증키
const String pageno ="&pageNo=1"; //view data page num
const String row = "&numOfRows=50";
const String type ="&dataType=JSON";  // data type JSON / XML
const String date = "&base_date=";
String user_data="";
String basetime ="&base_time=";
String fcsttime="";
const String nx ="&nx=";
const String ny ="&ny=";
String x="72";  //위도
String y="71";  //경도

//DB에서 가져온 위치정보 저장
static double latit;
static double longi;

//음악 재생시간
unsigned long prevPlayTime = 0;
unsigned long playDuration = 0;
int currentMelody = 0;

int melodySize = 75;

//Time Setting
const char * ntpserver= "pool.ntp.org";
const int  gmtOffset_sec = 32400; 
const int summerTime = 0;

FirebaseData firebaseData;
FirebaseJson json;

int counter =60;

int nowTime;

WiFiClient wificlient;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//마스터와 맞출것
  pinMode(d4, OUTPUT);
  pinMode(d5, OUTPUT);
  pinMode(d6, OUTPUT);
  pinMode(d7, OUTPUT);
  pinMode(d8,OUTPUT); 

  WiFi.begin(ssid, pass);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);
  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  configTime(gmtOffset_sec, summerTime, ntpserver);
  while(!time(nullptr))
  {
    Serial.print(".");
    delay(1000);
  }
  delay(10000); //오작동 방지 10초 대기
}

void loop() {
  // put your main code here, to run repeatedly:
   Firebase.getInt(firebaseData, "DBConnect/Users/10001/take_state");
   int getUmbrella = firebaseData.intData();

   Firebase.getInt(firebaseData, "DBConnect/Users/10001/run_motor");
   int run_motor = firebaseData.intData();
   if(run_motor==1)
   {
      Serial.println("motor on");
      digitalWrite(d4, HIGH);
      digitalWrite(d6, HIGH);
   }
   else
   {
     Serial.println("motor off");
     digitalWrite(d4, LOW);
     digitalWrite(d6, LOW);
   }
   
    if(getUmbrella ==1)
    {
      SoundMode();
    }
    else
    {
     //안가져감 
    }
}
