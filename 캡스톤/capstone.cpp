
/*
 * @ 2021.12.01 by KIM
 * @ finally we made it
 */
#include "FirebaseESP8266.h" 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ESP8266Wifi.h>
#include <string>
#include <ESP8266HTTPClient.h> 
#include <ArduinoJson.h>
#include "time.h"
#include <math.h>
#include <pitches.h>

#define NX 149 /* X�� ������ �� */
#define NY 253 /* Y�� ������ �� */

 //��ǥ��ȯ�� ����ü
struct lamc_parameter {
    float Re; /* ����� �����ݰ� [ km ] */
    float grid; /* ���ڰ��� [ km ] */
    float slat1; /* ǥ������ [degree] */
    float slat2; /* ǥ������ [degree] */
    float olon; /* �������� �浵 [degree] */
    float olat; /* �������� ���� [degree] */
    float xo; /* �������� X��ǥ [���ڰŸ�] */
    float yo; /* �������� Y��ǥ [���ڰŸ�] */
    int first; /* ���ۿ��� (0 = ����) */
};

using namespace std;

// Arudino Pin number . if not work check your board pin map

int d6 = 12; //���
int d4 = 4;  //����
int d5 = 14; //��ü
int d7 = 13; // ����
int d3 = 5; // IR
int d8 = 0; // Speaker

//end

// begin ���� �ڵ�
unsigned long prevPlayTime = 0;
unsigned long playDuration = 0;
int currentMelody = 0;

int melodySize = 75;
int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0, NOTE_G7, 0, 0,  0, NOTE_G6, 0, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0,
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0,
  NOTE_C7, 0, 0, NOTE_G6, 0, 0, NOTE_E6, 0, 0, NOTE_A6, 0, NOTE_B6, 0, NOTE_AS6, NOTE_A6, 0,
  NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7, 0, NOTE_F7, NOTE_G7, 0, NOTE_E7, 0,NOTE_C7, NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
  9, 9, 9, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
};

// ¡�ۺ� ù ���� ���� : �˱� ���� �������� ����� ��ȯ�Ͽ� ó��
int song[] = { NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, 0, // �̷ֹ�����(��ǥ)
NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, 0, // �̷ֹ�����(��ǥ)
NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, 0, // ���Ĺ̷���(��ǥ)
NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_E5, NOTE_C5, 0 }; // �ּ��ķ��̵�(��ǥ)
// ¡�ۺ� ù ���� ���ֽð�
int times[] = { 250, 250, 250, 250, 750, 250, // 8����ǥ = 250ms
250, 250, 250, 250, 750, 250,
250, 250, 250, 250, 750, 250,
250, 250, 250, 250, 500, 250, 250 };

// end

//���� api �ּ�
const String server = "http://apis.data.go.kr/1360000/VilageFcstInfoService_2.0/getVilageFcst?serviceKey="; // korea weather api host url
const String key = "";  // data.go.kr �Ϲ�����Ű
const String pageno = "&pageNo=1"; //view data page num
const String row = "&numOfRows=50";
const String type = "&dataType=JSON";  // data type JSON / XML
const String date = "&base_date=";
String user_data = "";
String basetime = "&base_time=";
String fcsttime = "";
const String nx = "&nx=";
const String ny = "&ny=";
String x = "72";  //����
String y = "71";  //�浵

double latit;
double longi;

//���� ���� ���� ������� ��� ����, �Ҹ� ���, ��� ��뿩��, ��� ����
bool umbrella = false;
String Sound;
bool damp = false;
bool isHuman = false;

const String serial = "10001";

int counter = 60;   // in loop() delay * counter = loop time 

//Firebase �ּ� �� pwd ����
#define Firebase_host ""  //enter the your firebase project url
#define Firebase_pass ""    //enter the your firebase project pwd

//wifi name % pwd setting
String ssid = ""; //enter the your wifi ssid
String wifi_pass = "";   // enter the your wifi pwd

//Time Setting
const char* ntpserver = "pool.ntp.org";
const int  gmtOffset_sec = 32400;
const int summerTime = 0;

FirebaseData firebaseData;
FirebaseJson json;

WiFiClient wificlient;

String temper;  //����µ�
String Wind_Speed;  //ǳ��
String Wind_Direction;  //ǳ��
String Sky; //�ϴ� ����
String POP; //����Ȯ��
String PCP; //������
String PTY; //��������  0 ��X 1�� 2��/��  3��  4�ҳ���
String REH; //����
String Snow;  //������

//String fcsttime[] = {"0000","0100","0200","0300","0400","0500","0600","0700","0800","0900","1000","1100","1200","1300","1400","1500","1600","1700","1800","1900","2000","2100","2200","2300"};
int nowTime;

//���⿡ �۾�, �������� �� �����ͼ� �����Ұž�
void SetWifi()
{

}

//DB���� ��ġ ���� ��������
void GetLocationData()
{
    String db = "internet-of-umbrella-default-rtdb/DBConnect/Users/10001/location";
    String lang = "latitude";
    String lon = "longtitude";
    if (Firebase.getString(firebaseData, "DBConnect/Users/10001/location/latitude"))
    {
        latit = (firebaseData.stringData()).toDouble();
        Serial.println(latit);
    }
    if (Firebase.getString(firebaseData, "DBConnect/Users/10001/location/longitude"))
    {
        longi = (firebaseData.stringData()).toDouble();
        Serial.println(longi);
    }
}

//DB�� ���� ���� ����
void SendWeatherData()
{
    String tempe = String(temper);
    String POP1 = String(POP);
    String PCP1 = String(PCP);
    String REH1 = String(REH);
    String Snow1 = String(Snow);
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/temper", tempe);
    if (Sky == "1")
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/sky", "����");
    else if (Sky == "3")
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/sky", "��������");
    else
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/sky", "�帲");

    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/probability", POP1);

    if (PTY == "0")
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/precipitation", "0");
    else if (PTY == "1")
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/precipitation", PCP1);
    else if (PTY == "2")
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/precipitation", PCP1);
    else if (PTY == "3")
    {
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/snow", Snow1);
    }
    else
        Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/precipitation", PCP1);

    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/humidity", REH1);
}


//��ǥ ��ȯ �Լ�
void dfs_xy_conf(double v1, double v2)
{
    double RE = 6371.00877; // ���� �ݰ�(km)
    double GRID = 5.0; // ���� ����(km)
    double SLAT1 = 30.0; // ���� ����1(degree)
    double SLAT2 = 60.0; // ���� ����2(degree)
    double OLON = 126.0; // ������ �浵(degree)
    double OLAT = 38.0; // ������ ����(degree)
    double XO = 43.0; // ������ X��ǥ(GRID)
    double YO = 136.0; // ������ Y��ǥ(GRID)
    double DEGRAD = PI / 180.0;
    double RADDEG = 180.0 / PI;

    double re = RE / GRID;
    double slat1 = SLAT1 * DEGRAD;
    double slat2 = SLAT2 * DEGRAD;
    double olon = OLON * DEGRAD;
    double olat = OLAT * DEGRAD;

    double sn = tan(PI * 0.25f + slat2 * 0.5f) / tan(PI * 0.25f + slat1 * 0.5f);
    sn = log(cos(slat1) / cos(slat2)) / log(sn);
    double sf = tan(PI * 0.25f + slat1 * 0.5f);
    sf = pow(sf, sn) * cos(slat1) / sn;
    double ro = tan(PI * 0.25f + olat * 0.5f);
    ro = re * sf / pow(ro, sn);

    int dx;
    int dy;
    double ra, theta;

    ra = tan(PI * 0.25f + (v1)*DEGRAD * 0.5f);
    ra = re * sf / pow(ra, sn);
    theta = v2 * DEGRAD - olon;
    if (theta > PI)
        theta -= 2.0f * PI;
    if (theta < -PI)
        theta += 2.0f * PI;
    theta *= sn;
    dx = floor(ra * sin(theta) + XO + 0.5f);
    dy = floor(ro - ra * cos(theta) + YO + 0.5f);

    x = String(dx);
    y = String(dy);
}

void SetWeather(String category, String item)
{
    if (category == "TMP")
        temper = item;
    if (category == "VEC")
        Wind_Direction = item;
    if (category == "WSD")
        Wind_Speed = item;
    if (category == "SKY")
        Sky = item;
    if (category == "PTY")
        PTY = item;
    if (category == "POP")
        POP = item;
    if (category == "PCP")
        PCP = item;
    if (category == "REH")
        REH = item;
    if (category == "SNO")
        Snow = item;
}

void GetWeather()
{
    Serial.printf("���� ����� %s���Դϴ� \n", temper);
    Serial.printf("���� ǳ���� %s �Դϴ� \n", Wind_Direction);
    Serial.printf("���� ǳ���� %sm/s�Դϴ� \n", Wind_Speed);
    if (Sky == "1")
        Serial.printf("���� ������ �����Դϴ� \n");
    else if (Sky == "3")
        Serial.printf("���� ������ ���������Դϴ�. \n");
    else
        Serial.printf("���� ������ �帲�Դϴ� \n");
    Serial.printf("���� Ȯ���� %s%% �Դϴ� \n", POP);
    if (PTY == "0")
        Serial.printf("�� ���� �ʽ��ϴ� \n");
    else if (PTY == "1")
        Serial.printf("�ð��� �������� %s%% �Դϴ� \n", PCP);
    else if (PTY == "2")
        Serial.printf("�ð��� �������� %s%% �Դϴ� \n", PCP);
    else if (PTY == "3")
    {
        Serial.printf("���� ���� ���� �ֽ��ϴ� \n");
        Serial.printf("�ð��� �������� %s cm�Դϴ� \n", Snow);
    }
    else
        Serial.printf("�ð��� �������� %s%% �Դϴ� \n", PCP);
    Serial.printf("���� ������ %s%% �Դϴ� \n", REH);
}

//Printing to Serial monital by present time 
// Notice
// Do not use first if you using first, time data not Set
void printLocalTime()
{
    time_t now = time(NULL);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
    delay(1000);
    Serial.printf("����ð� %d��%d%��%d��\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

void GetTimeData()
{
    time_t now = time(NULL);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
    delay(1000);
    int year = (timeinfo->tm_year) + 1900;
    int mon = (timeinfo->tm_mon) + 1;
    int day = (timeinfo->tm_mday);

    int result = year * 10000 + mon * 100 + day;
    user_data = String(result);

    Serial.printf("���ó�¥ %s \n", user_data);
}

//���û apiȣ���� ���� ��ȸ�� �ð��� ����
void SetFcstTime()
{
    time_t now = time(NULL);
    struct tm* timeinfo;
    timeinfo = localtime(&now);
    delay(1000);
    int hour = (timeinfo->tm_hour);
    int minute = (timeinfo->tm_min);
    int result = hour * 100 + minute;
    nowTime = hour * 100;
    Serial.println(nowTime);
    Serial.println(result);
    if (200 <= result && result < 500)
    {
        fcsttime = "0200";
    }
    else if (500 <= result && result < 800)
    {
        fcsttime = "0500";
    }
    else if (800 <= result && result < 1100)
    {
        fcsttime = "0800";
    }
    else if (1100 <= result && result < 1400)
    {
        fcsttime = "1100";
    }
    else if (1400 <= result && result < 1700)
    {
        fcsttime = "1400";
    }
    else if (1700 <= result && result < 2000)
    {
        fcsttime = "1700";
    }
    else if (2000 <= result && result < 2300)
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
    String url = server + key + pageno + row + type + date + user_data + basetime + fcsttime + nx + x + ny + y;
    Serial.println(url);
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connecting to Server");
        HTTPClient http;
        http.begin(wificlient, url);
        int httpCode = http.GET();
        if (httpCode > 0)
        {
            DynamicJsonDocument doc(12288);

            String payload = http.getString();
            DeserializationError error = deserializeJson(doc, payload);
            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.f_str());
                return;
            }
            JsonObject response_body = doc["response"]["body"];
            for (JsonObject response_body_items_item_item : response_body["items"]["item"].as<JsonArray>()) {
                const char* response_body_items_item_item_fcstTime = response_body_items_item_item["fcstTime"];
                String result = String(nowTime);
                if (String(response_body_items_item_item_fcstTime) == result)
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

// ���� ��带 �����ϴ� �Լ�
void SoundMode()
{
    if (Firebase.getString(firebaseData, "DBConnect/Users/10001/sound"))
    {
        Serial.println((firebaseData.stringData()));
        Sound = firebaseData.stringData();
    }
    Serial.printf("�Ҹ���� %d \n", Sound);
    if ((Sound.substring(2, 3)).equals("0"))
    {
        //���ҰŸ��
    }
    else if ((Sound.substring(2, 3)).equals("1"))
    {
        //1�� ���
        if (millis() < prevPlayTime + playDuration)
        {
            // music is playing. do nothing
            return;
        }
        // stop the tone playing:
        noTone(8);

        if (currentMelody >= melodySize)
            currentMelody = 0;
        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / tempo[currentMelody];

        tone(d8, melody[currentMelody], noteDuration);
        prevPlayTime = millis();
        playDuration = noteDuration * 1.30;

        currentMelody++;
    }
    else if ((Sound.substring(2, 3)).equals("2"))
    {
        //2�� ���
        for (int i = 0; i < 25; i++) // ���� ���̸�ŭ ���� ����
        {
            tone(d8, song[i], times[i]); // �迭������ �뷡 ���
            delay(times[i]);
        }
    }
    else if ((Sound.substring(2, 3)).equals("3"))
    {
        //3�� ���
    }
    else
    {
        //�߸��� �� 0���� �ʱ�ȭ
        Sound = "";
    }
}


//���� �����̿� �Լ�
void RainySensor()
{
    if (analogRead(d6) < 400)
    {
        Serial.println(analogRead(d6));
        damp = false;
        delay(1000);
    }
    else
    {
        Serial.println(analogRead(d6));
        damp = true;
        delay(1000);
    }
}

//��´� ��갡����
void takeUmbrella()
{
    Serial.println("���� " + PTY);
    if (PTY == "1" && umbrella && isHuman)
    {
        //�� ���� ����� ��������
        Serial.println("��갡������");
        SoundMode();
        delay(1000);
        IRSensor(); //���������� Ȯ��
        delay(1000);
    }
}

// ���ܼ� ��ü��������
void HumanSensor()
{
    if (LOW == digitalRead(d5))
    {
        isHuman = true;
        takeUmbrella();
        Serial.println(digitalRead(d5));
    }
    else
    {
        isHuman = false;
        Serial.println(digitalRead(d5));
    }
}

//����� �����ϴ� �Լ�
void IRSensor()
{
    if (LOW == digitalRead(d3))
    {
        Serial.println("�������");
        umbrella = true;
        Serial.println(digitalRead(d3));
    }
    else
    {
        Serial.println("������");
        umbrella = false;
        Serial.println(digitalRead(d3));
    }
}

//���� ������
//���� 15�а� ���͸� ������
void motor()
{
    int fif = 1000 * 60 * 15;
    unsigned long activation_time = millis();
    //���� ��� ����
    //����� �����ϰ� ���Ǿ�����
    while (activation_time < fif && umbrella && damp)
    {
        digitalWrite(d7, HIGH);
        IRSensor();
        RainySensor();
    }
    digitalWrite(d7, LOW);
}
//�� �ȿµ��� �׷��� �׽�Ʈ��尡 �ʿ��ؿ�
void TestMode()
{
    PTY = "1";
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/temper", "5");
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/sky", "�帲");
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/probability", "100");
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/precipitation", "5");
    Firebase.setString(firebaseData, "/DBConnect/Users/10001/weather/humidity", "90");
}

//Arduino First Execution
void setup() {
    // put your setup code here, to run once:
    pinMode(d3, INPUT);
    pinMode(d6, INPUT);
    pinMode(d5, INPUT);
    pinMode(d4, OUTPUT);
    pinMode(d7, OUTPUT);
    pinMode(d8, OUTPUT);
    Serial.begin(9600);
    WiFi.begin(ssid, wifi_pass);
    Serial.println();
    Serial.println("Waiting for Wifi connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
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
    while (!time(nullptr))
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
}

void loop() {
    delay(1000);
    RainySensor();
    HumanSensor();
    IRSensor();

    // put your main code here, to run repeatedly:
    if (counter == 60) //Get new data every 10 minutes
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