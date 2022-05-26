
/*
 * @ 2022.5.26
 * @ UPDATE INFO
 * @ Split the code to use Function and Define
 * @ Add input Wifi info use Web Document
 * @ Have fun enjoy my code thx :)
 * @ Author : KIM
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ESP8266WiFi.h>
#include <string>
#include <ESP8266HTTPClient.h> 
#include <math.h>
#include "FirebaseESP8266.h" 
#include "time.h"

using namespace std;

//Firebase 주소 및 pwd 설정
#define Firebase_host ""  //enter the your firebase project url
#define Firebase_pass ""    //enter the your firebase project pwd

#define NX 149 /* X축 격자점 수 */
#define NY 253 /* Y축 격자점 수 */

//센서 관련 변수 순서대로 우산 유무, 소리 출력, 우산 사용여부, 사람 존재
bool umbrella = false;
String Sound;
bool damp = false;
bool isHuman = false;

//날씨 api 주소
const String server1 ="http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey="; // korea weather api host url
const String key = "";  // data.go.kr 일반인증키
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

static double latit;
static double longi;

String temper;  //현재온도
String Wind_Speed;  //풍속
String Wind_Direction;  //풍향
String Sky; //하늘 상태
String POP; //강수확률
String PCP; //강수량
String PTY; //강수상태  0 비X 1비 2비/눈  3눈  4소나기
String REH; //습도
String Snow;  //적설량

//String fcsttime[] = {"0000","0100","0200","0300","0400","0500","0600","0700","0800","0900","1000","1100","1200","1300","1400","1500","1600","1700","1800","1900","2000","2100","2200","2300"};

//Time Setting
const char * ntpserver= "pool.ntp.org";
const int  gmtOffset_sec = 32400; 
const int summerTime = 0;

boolean wifi_connection = true;
FirebaseData firebaseData;
FirebaseJson json;

// begin 음악 코드
unsigned long prevPlayTime = 0;
unsigned long playDuration = 0;
int currentMelody = 0;

int melodySize = 75;

// Arudino Pin number . if not work check your board pin map
int d6 = 12; //우산
int d4 = 4;  //모터
int d5 = 14; //인체
int d7 = 13; // 모터
int d3 = 5; // IR
int d8 = 0; // Speaker

//end

const String serial = "10001";

int counter = 60;   // in loop() delay * counter = loop time 

//wifi name % pwd setting
//String ssid = "Simpletest"; //enter the your wifi ssid
//String wifi_pass ="simpletest";   // enter the your wifi pwd

WiFiClient wificlient;

int nowTime;

//Arduino First Execution
void setup() {
  // put your setup code here, to run once:
  pinMode(d3, INPUT);
  pinMode(d6, INPUT);
  pinMode(d5, INPUT);
  pinMode(d4,OUTPUT);
  pinMode(d7,OUTPUT);
  pinMode(d8,OUTPUT);  
  Serial.begin(9600);

  //repeat if user enter the wifi info, and connect wifi
  while(!wifi_connection)
  {
    web_start();
  }

  while(WiFi.status()!= WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(Firebase_host, Firebase_pass);
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
}

void loop() {
  delay(1000);
  RainySensor();
  HumanSensor();
  IRSensor();
  
  // put your main code here, to run repeatedly:
  if(counter == 60) //Get new data every 10 minutes
  {
    printLocalTime();
    GetTimeData();
    counter = 0;
    delay(1000);
    GetLocationData();
    dfs_xy_conf(latit, longi);
    getData();
    GetWeather();
    delay(1000);
    SendWeatherData();
    TestMode();
  }
  else
  {
    counter++;
    delay(5000);
  }
   motor();
}
