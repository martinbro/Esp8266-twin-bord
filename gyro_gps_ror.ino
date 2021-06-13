
#include <WebSockets2_Generic.h>
#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
#include <strings_en.h>
#include <WiFiManager.h>

/* Konstanter ************************************************ */
static const int RXPin = 15, TXPin = 13;// GPS: D8 (RX), D7(TX)
static const uint8_t RORpwm = 14; //D5 som udlæg output
static const uint16_t GPSBaud = 9600;

/* Globale variable ************************************************ */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 5;
uint16_t WiFi_DELAY_MS = 100;
static uint32_t t0 = 0 ;
static uint32_t t1,tWiFi = 0;
//bool AUTOMODE = false;
/* Initialiserer moduler ******************************************* */

using namespace websockets2_generic;
WebsocketsClient client;

void setup()
{
    Serial.begin(115200);
    while (!Serial) { ;}//venter til vi har kontakt til serielporten
    
    analogWriteResolution(10);
    int ip = initConnectToWifi();
    // kalder callback når message eller event modtages 
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);

    // connect to Deno-server
    if(ip==0){ client.connect("192.168.137.1", 8000, "/ws");}//Min laptop Hotspot
//  if(ip==1){ client.connect("192.168.87.155", 8000, "/ws");}//Hjemme
    if(ip==2){ client.connect("192.168.43.170", 8000, "/ws");}//HUAWEI?
    if(ip==1){ client.connect("192.168.8.220", 8000, "/ws");}//Marnav?
    // Send a ping
    client.ping("gps");

}

void loop()
{   
    t0 = millis(); //opdaterer timer ved nyt tik
    if (t0 - t1 >= 1000) {
        t1 = t0;
        int tal = rand()%250+1;
        Serial.printf("Tilfældigt tal: %d \n",100);
        analogWrite(RORpwm,100);
    }
}

/* ** FUNKTIONER ** FUNKTIONER ** FUNKTIONER ** FUNKTIONER ** */

int initConnectToWifi(){
  // ESP8266 Connect to wifi
  
  delay(5000);
  for (byte j = 0; j < 3; j++) {
    
    if(j==0){
      WiFi.begin("SKIB1","u530}8T9");
      Serial.println("Prøver at forbinde til LAPTOP-RJIJOOL9 4301");
      }
    if(j==1){
      WiFi.begin("Marnav","");
      Serial.println("Prøver at forbinde til Marnav");
      }
    if(j==2){
      WiFi.begin("HUAWEI","1q2w3e4r");
      Serial.println("Prøver at forbinde til mobiltelefon");
    }
  

    // Wait some time to connect to wifi
    for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) 
    {
      Serial.print(".");
      delay(1000);
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Succesfuld forbindelse til WiFi");
      return j;
      }
  }
  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("No Wifi!");
    return -1;
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  //Serial.print("Connected to Wifi, Connecting to WebSockets Server: ");
  //Serial.println(websockets_server_host);
}
void onMessageCallback(WebsocketsMessage message) 
{
  Serial.print("Got Message: ");
  Serial.println(message.data());
  
//   DynamicJsonDocument doc1(1024);
//   deserializeJson(doc1, message.data());
//   //int nr = doc1["nr"];
//   float wp_lg = doc1["lg"];
//   //float wp_br = doc1["br"];
//   if (wp_lg<0) {
//     int i= 0;
//     while(i <10){
//       lg[i] = -1,0;
//       br[i] = -1,0;
//       i++;

//     }
//     antalWP = 0;
//     activeWP = 0;
//   }else{
//      lg[antalWP]= doc1["lg"];
//      br[antalWP]= doc1["br"];
//       antalWP++;
//   }
 

//   if(doc1["mssg"]<90){
    
//   }

//   String n = doc1["name"];
//  // WiFi_DELAY_MS = doc1["rate"];
//   WiFi_DELAY_MS = 100;
//   K = doc1["k"];
//    K=0.99;
//   i= 0;
//   while(i < antalWP ){
//    Serial.print(br[i],6);
//    Serial.print(", "); 
//    Serial.println(lg[i],6); 
//    i++; 
//   }
//   Serial.println();

}
void onEventsCallback(WebsocketsEvent event, String data) 
{
  if (event == WebsocketsEvent::ConnectionOpened) 
  {
    Serial.println("Connnection Opened");
    Serial.println(data);
    
  } 
  else if (event == WebsocketsEvent::ConnectionClosed) 
  {
    Serial.println("Connnection Closed");
  } 
  else if (event == WebsocketsEvent::GotPing) 
  {
    Serial.println("Got a Ping!");
  } 
  else if (event == WebsocketsEvent::GotPong) 
  {
    Serial.println("Got a Pong!");
  }
}
