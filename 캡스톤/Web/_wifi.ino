#include <ESP8266WiFi.h>

char ssid[21] = "NO SSID";
char pass[21] = "";

String ssid_AP = "Arduino_IOT";
String pass_AP ="arduino123";

uint8_t mac_array[6];
char _mac[17];
String mac;
