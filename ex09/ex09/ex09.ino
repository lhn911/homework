#include <WiFi.h>
#include <WebServer.h>

// 你的手机热点信息
const char* ssid     = "Redmi K70 321";
const char* password = "123456789n";

WebServer server(80);
const int touchChannel = T0;  // D4触摸引脚
int touchRawValue = 0;

// 网页主页
void handleIndex() {
  String htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<title>触摸数值实时仪表盘</title>
<style>
body { text-align:center; margin-top:120px; font-size:32px; }
#data { color:#ff4400; font-weight:bold; font-size:60px; }
</style>
</head>
<body>
<h3>触摸引脚实时检测数值</h3>
<div id="data">0</div>
<script>
// 定时刷新获取最新触摸值
function updateData(){
  fetch("/getTouch")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("data").innerText = val;
  });
}
setInterval(updateData, 100);
</script>
</body>
</html>
)HTML";
  server.send(200, "text/html", htmlPage);
}

// 接口返回触摸原始数值
void handleGetTouchData() {
  server.send(200, "text/plain", String(touchRawValue));
}

void setup() {
  Serial.begin(115200);

  // 连接WiFi热点
  Serial.print("正在连接热点：");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功");
  Serial.print("设备局域网IP：");
  Serial.println(WiFi.localIP());

  // 绑定网页路由
  server.on("/", handleIndex);
  server.on("/getTouch", handleGetTouchData);
  server.begin();
}

void loop() {
  server.handleClient();
  touchRawValue = touchRead(touchChannel);
  delay(100);
}
