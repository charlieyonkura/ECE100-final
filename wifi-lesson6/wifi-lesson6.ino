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

char ssid[] = "LAPTOP-CKEQAMAM 6750";           // replace *** with your wifi network SSID (name)
char pass[] = "=33pM059";        // replace *** with your wifi network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned int local_port = 8888;        // local port to listen for UDP packets
unsigned int remote_port = 8888;        // remote port to listen for UDP packets

const int UDP_PACKET_SIZE = 255;  // UDP timestamp is in the first 48 bytes of the message
const int UDP_TIMEOUT = 3000;    // timeout in miliseconds to wait for an UDP packet to arrive

char packetBuffer[255];
PWMServo head;

// A UDP instance to let us send and receive packets over UDP
WiFiEspUDP Udp;

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
                               // if you get a client,
      //Serial.print("Received packet of size ");
      //Serial.println(packetSize);
      int len = Udp.read(packetBuffer, 255);

      Serial.println(packetBuffer);
      
      Udp.flush();
      if (len > 0)  packetBuffer[len] = 0;
  
      if(packetBuffer[0]=='S');
      {   
        if(packetBuffer[1] == 'T')
        {
            int angle = get_value(packetBuffer);
            angle = map(angle, 0, 99,  0,180);
            Serial.println(angle);
            head.write(angle);      
         }   
      } //END OF ACTION SWITCH
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
 
