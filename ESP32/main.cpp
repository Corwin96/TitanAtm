#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ESP32Ping.h>
#include <ArduinoJson.h>

/*
  Secured Esp32 Websockets Client

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS)
        3. Sends the websockets server a message ("Hello Server")
        4. Sends the websocket server a "ping"
        5. Prints all incoming messages while the connection is open

    NOTE:
    The sketch dosen't check or indicate about errors while connecting to 
    WiFi or to the websockets server. For full example you might want 

  Websockets By Gil Maimon is used
  https://github.com/gilmaimon/ArduinoWebsockets

*/

#define BACKLIGHT_PIN 13

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Creat a set of new characters
const uint8_t charBitmap[][8] = {
   { 0xc, 0x12, 0x12, 0xc, 0, 0, 0, 0 },
   { 0x6, 0x9, 0x9, 0x6, 0, 0, 0, 0 },
   { 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0, 0x0 },
   { 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0, 0x0 },
   { 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0, 0x0 },
   { 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0, 0x0 },
   { 0x0, 0x0, 0x0, 0x6, 0x9, 0x9, 0x6, 0x0 },
   { 0x0, 0x0, 0x0, 0xc, 0x12, 0x12, 0xc, 0x0 }
   
};

unsigned long currentTime;
int period = 4000;
unsigned long lastTime;
bool tokenNotify = 1;       //to set up in the loop

const char* ssid = "NoWiresAttached"; //Enter SSID
const char* password = "D1ck&m4T_W*6"; //Enter Password

const char* websockets_connection_string = "wss://rinkeby.infura.io/ws"; //"wss://echo.websocket.org"; //Enter server adress

// This fingerprint was updated 20.04.2019
const char echo_org_ssl_fingerprint[] PROGMEM = "E0 E7 13 AE F4 38 0F 7F 22 39 5C 32 B3 16 EC BB 95 F3 0B 5B";

String subscribtionLog;                  //Subscribtion number
String messageFromServer;

uint32_t msgcount = 0;

using namespace websockets;

void tokenReceived();

void verifyMsgOnSubscribtionLog(String msg)
{
    Serial.println("Verifying");
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    if(subscribtionLog == doc["params"]["subscription"].as<String>())
    {
        tokenReceived();
    }   
}

void onFirstMessage(String msg)   //First message is used for verification of subscription
{
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    subscribtionLog = doc["result"].as<String>();
    Serial.print("created log: "); Serial.println(subscribtionLog);
}

void onMessageCallback(WebsocketsMessage message)
{
    Serial.print("Got Message: ");
    Serial.print(message.data());
    Serial.print(" Message number ");
    Serial.println(msgcount);
    messageFromServer = message.data();

    if(msgcount == 0)
    {
        onFirstMessage(message.data());
    }
    else
    {
        verifyMsgOnSubscribtionLog(message.data());
    }
    msgcount++;
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setupSubscribe(WebsocketsClient webSockClient, String jsonToSend)
{
    webSockClient.send(jsonToSend);
}

void pingSomething()
{
    bool success = Ping.ping("192.168.4.21", 3);
 
    if(!success){
        Serial.println("Ping failed");
        return;
    }
 
    Serial.println("Ping succesful.");
}

void tokenReceived()
{
    Serial.println("token received");
    //lastTime = millis();
    //tokenNotify = 1;
    lcd.clear();
    lcd.home ();
    lcd.print("Titan Arcade");
    lcd.setCursor(0, 1);
    lcd.print(" Token received!");
    while(millis() < currentTime + period){}
    lcd.clear();
    lcd.home ();
    lcd.print("Titan Arcade");
    lcd.setCursor(0, 1);
    lcd.print("Send Titan token");
}

WebsocketsClient client;
IPAddress gateway(192,168,178,1);
IPAddress subnet(255,255,255,0);
IPAddress ip(192,168,178,102);
IPAddress dnsip(8,8,8,8);
IPAddress dns2ip(8,8,4,4);

void setup()
{
    int charBitmapSize = (sizeof(charBitmap) / sizeof(charBitmap[0]));

    // Switch on the backlight
    pinMode(BACKLIGHT_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_PIN, HIGH);
  
    lcd.begin(16,2);               // initialize the lcd 

    for(int i = 0; i < charBitmapSize; i++)
    {
       lcd.createChar ( i, (uint8_t *)charBitmap[i] );
    }

    lcd.home();                   // go home
    lcd.print(" Setup");  
    lcd.setCursor( 0, 1 );        // go to the next line
    lcd.print("  Starting   ");
    
    Serial.begin(115200);

    WiFi.config(ip, gateway, subnet, dnsip, dns2ip);
    // Connect to wifi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to "); Serial.println(ssid);
    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }
    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP());
    pingSomething();
    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);

    // Before connecting, set the ssl fingerprint of the server
    //client.setFingerprint(echo_org_ssl_fingerprint);

    // Connect to server
    client.connect(websockets_connection_string);

    // RPC call to check if a token has been transferred, change the address to the recipient contract address and the topic to the corresponding emit hash
    client.send("{\"jsonrpc\": \"2.0\", \"id\": 1, \"method\": \"eth_subscribe\", \"params\": [\"logs\", {\"address\": \"0x335a1f5634A440Ae4ca59adddd42AcB4E8dDbeF3\", \"topics\": [\"0x324633c1240a599ae948c2245fead42ace06b9f0e120c7d74045f79b248f59f3\"]}]}");

    lcd.clear();
    lcd.home ();
    lcd.print("Titan Arcade");
    lcd.setCursor(0, 1);
    lcd.print("Send Titan token");
    // Send a ping
    //client.ping();
}

void loop()
{
    currentTime = millis();
    client.poll();
    
    /*
    if(tokenNotify == 1)
    {
        if((currentTime-4000)>=lastTime)
        {
            lcd.clear();
            lcd.home ();
            lcd.print("Titan Arcade");
            lcd.setCursor(0, 1);
            lcd.print(" Token received!");
        }
        else
        {
            tokenNotify = 0;
            lcd.clear();
            lcd.home ();
            lcd.print("Titan Arcade");
            lcd.setCursor(0, 1);
            lcd.print(" Send Titan token");
        }
    }
    */
    //client.send("hoi");
}
