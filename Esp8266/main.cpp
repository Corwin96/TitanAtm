#include <Arduino.h>
/*
  Secured Esp8266 Websockets Client

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS)
        3. Sends the websockets server a message ("Hello Server")
        4. Sends the websocket server a "ping"
        5. Prints all incoming messages while the connection is open

    NOTE:
    The sketch dosen't check or indicate about errors while connecting to 
    WiFi or to the websockets server. For full example you might want 
    to try the example named "Esp8266-Client" (And use the ssl methods).

  Hardware:
        For this sketch you only need an ESP8266 board.

  Created 15/02/2019
  By Gil Maimon
  https://github.com/gilmaimon/ArduinoWebsockets

*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password

const char* websockets_connection_string = "ws://192.168.178.22:8545"; //Enter server adress

// This fingerprint was updated 20.04.2019
const char echo_org_ssl_fingerprint[] PROGMEM = "E0 E7 13 AE F4 38 0F 7F 22 39 5C 32 B3 16 EC BB 95 F3 0B 5B";

String createdLog;
String messageFromServer;

uint32 msgcount = 0;

using namespace websockets;

void verifyMsgOnCreatedLog(string msg)
{
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    createdLog = doc["result"].as<String>();
    Serial.print("created log: "); Serial.println(createdLog);s
}

void onFirstMessage(String msg)   //First message is used for verification of subscription
{
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    createdLog = doc["result"].as<String>();
    Serial.print("created log: "); Serial.println(createdLog);
}

void onMessageCallback(WebsocketsMessage message)
{
    Serial.print("Got Message: ");
    Serial.println(message.data());
    messageFromServer = message.data();

    if(msgcount == 0)
    {
        onFirstMessage(message.data());
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

WebsocketsClient client;

void setup() {
    Serial.begin(115200);
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

    // run callback when messages are received
    client.onMessage(onMessageCallback);
    
    // run callback when events are occuring
    client.onEvent(onEventsCallback);

    // Before connecting, set the ssl fingerprint of the server
    //client.setFingerprint(echo_org_ssl_fingerprint);

    // Connect to server
    client.connect(websockets_connection_string);

    client.send("{\"jsonrpc\": \"2.0\", \"id\": 1, \"method\": \"eth_subscribe\", \"params\": [\"logs\", {\"address\": \"0xFAeE2fA3942030D7FDc5EF293f99968aefD634f4\", \"topics\": [\"0xa0a5a35754b3519ae614f00422ac62a6bc4608ccb722963aab00cb48c588ad72\"]}]}");

    // Send a ping
    //client.ping();
}

void loop() {
    client.poll();
    //client.send("hoi");
}
