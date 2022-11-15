#include <PWMServo.h>
#include "WiFiEsp.h"
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"

#define ENA 3
#define ENB 6
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

SoftwareSerial softserial(4, 5);

char ssid[] = "LAPTOP-CKEQAMAM";
char pass[] = "2824hhasdo";
int status = WL_IDLE_STATUS;

unsigned int local_port = 8888;
unsigned int remote_port = 8888;

const int UDP_PACKET_SIZE = 255;  // UDP timestamp is in the first 48 bytes of the message
const int UDP_TIMEOUT = 3000;    // timeout in miliseconds to wait for an UDP packet to arrive

char packetBuffer[255];
String packetBufferStr;
int controlArraySize = 6;
int controlArray[6];
long LX; //longs to do sq and sqrt ops
long LY;
long RX;
long RY;
int button;
int dpad;

PWMServo head;
WiFiEspUDP Udp;
IPAddress laptopIP(192,168,137,1);

void forward() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
void left() {
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void right() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}
void back() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}
void stop() {
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}
void speed(int L, int R) {
  analogWrite(ENB,L);
  analogWrite(ENA,R);
}

void setup() {
  Serial.begin(9600);
  softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(9600);
  WiFi.init(&softserial);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  /*int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    Serial.println(WiFi.SSID(i));
  }*/
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  Udp.begin(local_port);
  Udp.beginPacket(laptopIP, 8888);
  Udp.write("hi mom");
  Udp.endPacket();
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
      //Serial.print("Received packet of size ");
      //Serial.println(packetSize);
      Udp.read(packetBuffer, 255);
      Udp.flush();
      
      //Serial.println(packetBuffer);
      packetBufferStr = packetBuffer;
      for (int i = 0; i < controlArraySize; i++){
        controlArray[i] = packetBufferStr.substring(4*i, 4*i + 3).toInt();
      }
      LX = map(controlArray[0], 0, 255, -255, 255);
      LY = map(controlArray[1], 255, 0, -255, 255);
      RX = map(controlArray[2], 0, 255, -255, 255);
      RY = map(controlArray[3], 255, 0, -255, 255);
      button = controlArray[4];
      dpad = controlArray[5];
      
      int Lintensity = min(sqrt(sq(LX) + sq(LY)), 255); //LX and LY are longs so that no overflow occurs
      int Rintensity = min(sqrt(sq(RX) + sq(RY)), 255);
      speed(Lintensity, Lintensity);
      if (Lintensity < 40) { //deadzone with radius 40
        stop();
        Serial.println("Stop");
      } else if (LY > abs(LX)) {
        forward();
        Serial.println("Forward");
      } else if ((-1*LY) > abs(LX)) {
        back();
        Serial.println("Back");
      } else if (LX > abs(LY)) {
        right();
        Serial.println("Right");
      } else if ((-1*LX) > abs(LY)) {
        left();
        Serial.println("Left");
      }
      if (bitRead(button, 0)) {//L1
        Serial.println("Close claw");
      } else if (bitRead(button, 1)) { //R1
        Serial.println("Open claw");
      }
      if (dpad == 0) { //up
        Serial.println("Elbow up");
      } else if (dpad == 4) { //down
        Serial.println("Elbow down");
      }
      if (Rintensity < 40) { //deadzone with radius 40

      } else if (RY > abs(RX)) {
        Serial.println("Shoulder up");
      } else if ((-1*RY) > abs(RX)) {
        Serial.println("Shoulder down");
      } else if (RX > abs(RY)) {
        Serial.println("Rotate right");
      } else if ((-1*RX) > abs(RY)) {
        Serial.println("Rotate left");
      }
  } 
}
