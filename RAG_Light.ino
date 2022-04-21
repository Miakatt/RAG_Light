#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
// Set WiFi credentials
#define WIFI_SSID "Kikidoodle_Network"
#define WIFI_PASS "Bottle$TORE101!"
// Set UDP Port
#define UDP_PORT 4210
// Set LED parameters
#define LED_PIN      2   
#define LED_COUNT    1   // Number of LEDs 
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//--------R, A, G Values.--------------------------  
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t amber = strip.Color(255, 188, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t off = strip.Color(0, 0, 0);
//--------------------------------------------------
// Create UDP Instance.
WiFiUDP UDP;
char udpPacket[4];       // UDP Packet
String usbPacket;    // Serial packet 
char reply[] = "X";   // Reply with an X to confirm message received. 
  
void setup() {
  // Setup serial port
  Serial.begin(115200);
  Serial.println();


  //Neopixel Strip Setup
  strip.begin();
  strip.clear();
  strip.fill(blue);
  strip.setBrightness(255);
  strip.show(); // Initialize all pixels to 'off'
 
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
   
  // Connecting to WiFi...
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    strip.fill(blue);
    strip.show();
    delay(100);
    Serial.print(".");  
    strip.fill(off);
    strip.show(); 
    delay(100);   
  }
//--------------------------------------------------
  // Connected to WiFi
  strip.fill(green);
  strip.show();
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());  
//--------------------------------------------------
  // Begin listenting to UDP Port
  UDP.begin(UDP_PORT);
  Serial.print("Listening on UDP Port ");
  Serial.println(UDP_PORT);
//--------------------------------------------------
  // 
}

//--------------------------------------------------

void setRAG(char packet){
  // packet = toupper(packet);
  // Set colour depending on the char received. 
  Serial.print("Received ");
  Serial.println(packet);
  switch (packet){
    case '1':
      strip.fill(green);
      strip.show();
    break;
    case '2':
      strip.fill(amber);
      strip.show();
      break;
    case '3':
      strip.fill(red);
      strip.show();
      break;
    case '0':
      strip.fill(off);
      strip.show();
      break;      
    default:
      strip.fill(green);
      strip.show();
      break;
  }
}

//--------------------------------------------------


void loop() {
  // put your main code here, to run repeatedly:
  // Check if packet has been received. 
  int packetSize = UDP.parsePacket(); // UDP Packet
  int serialSize = Serial.available();// USB Serial packet
  // ********** UDP **********  
  if (packetSize) {
    int len = UDP.read(udpPacket, 255);
    if (len > 0){
      udpPacket[len] = '\0';
    }
    Serial.print("Packet received : ");
    Serial.println(udpPacket);
   
   // Set Colours depending on Packet Character
   setRAG(udpPacket[3]);  // strip of the LS|. Only pass the integer part of the incoming message.
   
   // Send a return packet
   
   UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
   UDP.write(reply);
   UDP.endPacket();
  }
  // ********** USB ********** 
  if (serialSize>0) {
    usbPacket = Serial.readString();
    setRAG(usbPacket.charAt(3));    // strip of the LS|. Only pass the integer part of the incoming message.
    delay(100);
    Serial.write('X');
  }
  
}
