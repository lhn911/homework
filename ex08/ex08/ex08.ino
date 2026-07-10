#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "Redmi K70 321";
const char* password = "123456789n";

WebServer server(80);
const int ledPin = 2;
const int touchChannel = T0;
const int touchThreshold = 40;

bool systemArm = false;
bool isAlarm = false;

void handleRoot() {
  String page = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>安防报警系统</title>
<style>
button{font-size:26px;padding:15px 40px;margin:20px;}
</style>
</head>
<body style="text-align:center;margin-top:100px;">
<h2>安防主机控制面板</h2>
<button onclick="fetch('/arm')">布防 ARM</button>
<button onclick="fetch('/disarm')">撤防 DISARM</button>
</body>
</html>
)HTML";
  server.send(200, "text/html", page);
}

void handleArm(){
  systemArm = true;
  server.send(200, "text/plain", "已布防");
}

void handleDisarm(){
  systemArm = false;
  isAlarm = false;
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "已撤防，报警解除");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);

  Serial.print("正在连接WiFi：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功，IP地址：");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(touchChannel);
  bool touchNow = (touchVal < touchThreshold);

  if(systemArm && touchNow && !isAlarm){
    isAlarm = true;
  }

  if(isAlarm){
    digitalWrite(ledPin, HIGH);
    delay(80);
    digitalWrite(ledPin, LOW);
    delay(80);
  }
}
