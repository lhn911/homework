#include <WiFi.h>
#include <WebServer.h>

// 已配置你的WiFi
const char* ssid     = "Redmi K70 321";
const char* password = "123456789n";

WebServer server(80);
const int ledPin = 2;   // D2引脚
int brightness = 0;

// 主页页面，包含滑动条
void handleRoot() {
  String html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>网页无极调光</title>
<style>
body{text-align:center;margin-top:100px;font-size:24px;}
#slider{width:80%;height:40px;margin-top:30px;}
</style>
</head>
<body>
<h2>LED亮度调节滑块 0~255</h2>
<input type="range" id="slider" min="0" max="255" value="0">
<script>
const slider = document.getElementById('slider');
slider.addEventListener('input',function(){
  let val = this.value;
  fetch('/set?bri='+val);
})
</script>
</body>
</html>
)HTML";
  server.send(200, "text/html", html);
}

// 接收网页亮度参数
void handleSetBright() {
  if(server.hasArg("bri")){
    brightness = server.arg("bri").toInt();
    analogWrite(ledPin, brightness);
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // 连接手机热点
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功，设备局域网IP地址：");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
}

void loop() {
  server.handleClient();
}
