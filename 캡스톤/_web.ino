#include <ESP8266WebServer.h>

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,255);
IPAddress subnet(255,255,255,0);
ESP8266WebServer server(80);
