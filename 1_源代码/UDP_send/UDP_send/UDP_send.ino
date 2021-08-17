#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
 
const char* ssid = "dachou_Humanoid_manipulator";                 //你的AP的名字
const char* password = "55588888";         //AP的密码
const char* IP = "192.168.1.1";             //创建AP的模块的IP地址
const int PORT = 6666;                      //模块的端口号

WiFiUDP Udp;
unsigned int localUdpPort = 6666;  //本地的端口号，就是ESP8266-NodeMCU的端口号
char incomingPacket[255];  // buffer for incoming packets
String comdata="";
char send_data[] = {};
 
void setup() {
  
  Serial.begin(115200);
  Serial.println();
  //开始连接
  Serial.printf("Connecting to %s ", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  //开启本地端口
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  Udp.parsePacket();
}
 
void loop() {
    /******************串口接受数据************************/
    while (Serial.available() > 0) // 串口收到字符数大于零。
    {
        comdata += char(Serial.read());
        delay(2);
    }
    /******************串口打印数据************************/ 
    if(comdata!="")//如果接受到数据
    {
      //开始发送数据
      Udp.parsePacket();
      //开始打包
      Udp.beginPacket(IP,PORT);
      strcpy(send_data,comdata.c_str());
      //写入数据
      Udp.write(send_data);
      //发送
      Udp.endPacket();
      Serial.printf("UDP send once \r\n");
    }
    comdata = "";//清空数据
    
}
