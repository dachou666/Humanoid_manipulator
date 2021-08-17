#include <Servo.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//----舵机部分---
#define THUMB_MAX 120
#define THUMB_MIN 10
#define THUMB_INIT 10

#define INDEX_FINGER_MAX 125
#define INDEX_FINGER_MIN 20
#define INDEX_FINGER_INIT 20

#define MIDDLE_FINGER_MAX 125
#define MIDDLE_FINGER_MIN 0
#define MIDDLE_FINGER_INIT 0

#define RING_FINGER_MAX 117
#define RING_FINGER_MIN 20
#define RING_FINGER_INIT 20

#define LITTLE_FINGER_MAX 117
#define LITTLE_FINGER_MIN 30
#define LITTLE_FINGER_INIT 30

const int numberOfServos = 8; // Number of servos
Servo servo[numberOfServos];  // 定义Servo对象来控制
int choose_thumb; //选择拇指
int thumb_pos; //控制拇指的舵机值

//----网络部分---
unsigned int localPort = 6666;      // 可以自定义端口号
int packetSize = 0;
int n;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];   
WiFiUDP Udp;

int standard_state=0;
int first_flag=0;
int second_flag=0;

void setup() {

  Serial.begin(115200);
  Serial.println("Robot Starts Initialization");
//---------------
  WiFi.mode(WIFI_AP);
  IPAddress softLocal(192,168,1,1);                      // 设置内网WIFI IP地址
  IPAddress softGateway(192,168,1,1);
  IPAddress softSubnet(255,255,255,0);
  WiFi.softAPConfig(softLocal, softGateway, softSubnet);
   
  String apName = ("dachou_Humanoid_manipulator");  // 设置WIFI名称 //+(String)ESP.getChipId()
  const char *softAPName = apName.c_str();
   
  WiFi.softAP(softAPName, "55588888");      //  创建wifi  名称 + 密码 
   
  IPAddress myIP = WiFi.softAPIP(); 
  Serial.println("");
  Serial.print("AP直链模式 网关IP: ");     
  Serial.println(myIP);
   
  Serial.print("AP SSID: ");   //串口打印WIFI 名称
  Serial.println(apName);
  Serial.print("AP 密码: ");        
  Serial.println("55588888");  //串口打印WIFI 密码
//---------
  Udp.begin(localPort);   //UDP服务初始化
  
//  servo[0].attach(5);servo[0].write(THUMB_INIT);
//  servo[1].attach(4);servo[1].write(INDEX_FINGER_INIT);
//  servo[2].attach(0);servo[2].write(MIDDLE_FINGER_INIT);
//  servo[3].attach(2);servo[3].write(RING_FINGER_INIT);
//  servo[4].attach(14);servo[4].write(LITTLE_FINGER_INIT);
}

void loop() { 
  
  if(standard_state == 0 && first_flag == 0)
  {
    first_flag = 1;
    servo[1].attach(4);servo[1].write(INDEX_FINGER_INIT);
    servo[2].attach(0);servo[2].write(MIDDLE_FINGER_INIT);
    servo[3].attach(2);servo[3].write(RING_FINGER_INIT);
    servo[4].attach(14);servo[4].write(LITTLE_FINGER_INIT);
    delay(500);
    servo[0].attach(5);servo[0].write(THUMB_INIT);
  }

  if(standard_state == 1 && second_flag == 0)
  {
    second_flag = 1;
    servo[0].attach(5);servo[0].write(THUMB_MAX);
    servo[1].attach(4);servo[1].write(INDEX_FINGER_MAX);
    servo[2].attach(0);servo[2].write(MIDDLE_FINGER_MAX);
    servo[3].attach(2);servo[3].write(RING_FINGER_MAX);
    servo[4].attach(14);servo[4].write(LITTLE_FINGER_MAX);
  }
  
  packetSize = Udp.parsePacket();
  if (packetSize)  //等待接受UDP数据，一旦收到就进入处理
  {          
    n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);  
    packetBuffer[n] = 0;
    Serial.printf("Receive data:");
    Serial.println(packetBuffer);

    if(!(strcmp(packetBuffer,"MAX_finish")))
    {
      standard_state = 1;
    }
    else if(!(strcmp(packetBuffer,"MIN_finish")))
    {
      standard_state = 2;
    }
    else if(standard_state == 2)
    {
      choose_thumb = packetBuffer[0]-48;
      if(packetBuffer[2] == ' ')
      {
        packetBuffer[2] = '0';
      }
      thumb_pos = (packetBuffer[2]-48)*100+(packetBuffer[3]-48)*10+(packetBuffer[4]-48);
      Serial.printf("%d,%d\r\n",choose_thumb,thumb_pos);
  
      switch(choose_thumb)
      {
        case 0:{servo[choose_thumb].attach(5);servo[choose_thumb].write(thumb_pos);}break;
        case 1:{servo[choose_thumb].attach(4);servo[choose_thumb].write(thumb_pos);}break;
        case 2:{servo[choose_thumb].attach(0);servo[choose_thumb].write(thumb_pos);}break;
        case 3:{servo[choose_thumb].attach(2);servo[choose_thumb].write(thumb_pos);}break;
        case 4:{servo[choose_thumb].attach(14);servo[choose_thumb].write(thumb_pos);}break;
        default:break;
       }
    }

  }
}
