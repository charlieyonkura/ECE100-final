/*  ___   ___  ___  _   _  ___   ___   ____ ___  ____  
 * / _ \ /___)/ _ \| | | |/ _ \ / _ \ / ___) _ \|    \ 
 *| |_| |___ | |_| | |_| | |_| | |_| ( (__| |_| | | | |
 * \___/(___/ \___/ \__  |\___/ \___(_)____)___/|_|_|_|
 *                  (____/ 
 * Osoyoo Wifi IoT  lesson 6
 * Remote control Servo over UDP
 * tutorial url: https://osoyoo.com/?p=32612
 */
#include <PWMServo.h>
#include "WiFiEsp.h"
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"
#define SERVO_PIN 9
int angle=90;

SoftwareSerial softserial(4, 5); // D4 to ESP_TX, D5 to ESP_RX by default

char ssid[] = "LAPTOP-CKEQAMAM";           // replace *** with your wifi network SSID (name)
char pass[] = "2824hhasdo";        // replace *** with your wifi network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int local_port = 8888;        // local port to listen for UDP packets
unsigned int remote_port = 8888;        // remote port to listen for UDP packets

const int UDP_PACKET_SIZE = 255;  // UDP timestamp is in the first 48 bytes of the message
const int UDP_TIMEOUT = 3000;    // timeout in miliseconds to wait for an UDP packet to arrive

char packetBuffer[255];
String packetBufferStr;
int controlArraySize = 5;
int controlArray[5];
PWMServo head;

// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;

/*int parseString(char str[]) {
  int a[20]; //change size
  int j = 0;
  char temp[3];
  for (int i = 0; i < strlen(str); i++) {
    while (!str[i] == ',') {
      temp += str[i];
    }
    a[j] = atoi(temp);
    j++;
    temp = "";
  }
  return a;
}*/

/*int parseString(String str) {
  int a[5];
  for (int i = 0; i < 5; i++){
    a[i] = str.substring(4*i, 4*i + 3).toInt();
  }
  return a;
}*/

void setup()
{
  Serial.begin(9600);   // initialize serial for debugging
  softserial.begin(115200);
  softserial.write("AT+CIOBAUD=9600\r\n");
  softserial.write("AT+RST\r\n");
  softserial.begin(9600);    // initialize serial for ESP module
  WiFi.init(&softserial);    // initialize ESP module

  /*int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    Serial.println(WiFi.SSID(i));
  }*/

   
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network, move the top slider to control servo!");
  
  Udp.begin(local_port);

   //head.attach(SERVO_PIN);
   //head.write(0);
   //delay(500);
   //head.write(180);
   //delay(500);
   //head.write(90);
   
}

void loop()
{

  int packetSize = Udp.parsePacket();
  if (packetSize) {
      //Serial.print("Received packet of size ");
      //Serial.println(packetSize);
      Udp.read(packetBuffer, 255);
      Udp.flush();
      
      Serial.println(packetBuffer);
      packetBufferStr = packetBuffer;
      for (int i = 0; i < controlArraySize; i++){
        controlArray[i] = packetBufferStr.substring(4*i, 4*i + 3).toInt();
      }
      
      
  }
}
int get_value(char bf[])
{
  int value = bf[3]- '0';
  
  for(int i = 4; i < 6; i++)
  {
    if(bf[i] < '0' || bf[i] > '9')
      break;
    else
      value = value * 10 + bf[i] - '0';
  }
  
  return value;
}
 
