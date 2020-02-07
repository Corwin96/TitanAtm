#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_wpa2.h"
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

/*
  Websockets By Gil Maimon is used
  https://github.com/gilmaimon/ArduinoWebsockets

    IPAddress gateway(192,168,178,1);
    IPAddress subnet(255,255,255,0);
    IPAddress ip(192,168,178,102);
    IPAddress dnsip(8,8,8,8);
    IPAddress dns2ip(8,8,4,4);
    To use normal wifi
*/

#define EAP_ANONYMOUS_IDENTITY ""                               //Anonymous identity (nothing in case of eduroam)
#define EAP_IDENTITY ""                                         //User identity (student mail)
#define EAP_PASSWORD ""                                         //User password
#define BACKLIGHT_PIN 13

const char* ssid = "eduroam";                                   //SSID
//const char* password = "";                                    //WPA2 NON ENTERPRISE

const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n" \
                           "MIIEsTCCA5mgAwIBAgIQCKWiRs1LXIyD1wK0u6tTSTANBgkqhkiG9w0BAQsFADBh\n" \
                           "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
                           "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
                           "QTAeFw0xNzExMDYxMjIzMzNaFw0yNzExMDYxMjIzMzNaMF4xCzAJBgNVBAYTAlVT\n" \
                           "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
                           "b20xHTAbBgNVBAMTFFJhcGlkU1NMIFJTQSBDQSAyMDE4MIIBIjANBgkqhkiG9w0B\n" \
                           "AQEFAAOCAQ8AMIIBCgKCAQEA5S2oihEo9nnpezoziDtx4WWLLCll/e0t1EYemE5n\n" \
                           "+MgP5viaHLy+VpHP+ndX5D18INIuuAV8wFq26KF5U0WNIZiQp6mLtIWjUeWDPA28\n" \
                           "OeyhTlj9TLk2beytbtFU6ypbpWUltmvY5V8ngspC7nFRNCjpfnDED2kRyJzO8yoK\n" \
                           "MFz4J4JE8N7NA1uJwUEFMUvHLs0scLoPZkKcewIRm1RV2AxmFQxJkdf7YN9Pckki\n" \
                           "f2Xgm3b48BZn0zf0qXsSeGu84ua9gwzjzI7tbTBjayTpT+/XpWuBVv6fvarI6bik\n" \
                           "KB859OSGQuw73XXgeuFwEPHTIRoUtkzu3/EQ+LtwznkkdQIDAQABo4IBZjCCAWIw\n" \
                           "HQYDVR0OBBYEFFPKF1n8a8ADIS8aruSqqByCVtp1MB8GA1UdIwQYMBaAFAPeUDVW\n" \
                           "0Uy7ZvCj4hsbw5eyPdFVMA4GA1UdDwEB/wQEAwIBhjAdBgNVHSUEFjAUBggrBgEF\n" \
                           "BQcDAQYIKwYBBQUHAwIwEgYDVR0TAQH/BAgwBgEB/wIBADA0BggrBgEFBQcBAQQo\n" \
                           "MCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBCBgNVHR8E\n" \
                           "OzA5MDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRHbG9i\n" \
                           "YWxSb290Q0EuY3JsMGMGA1UdIARcMFowNwYJYIZIAYb9bAECMCowKAYIKwYBBQUH\n" \
                           "AgEWHGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMwCwYJYIZIAYb9bAEBMAgG\n" \
                           "BmeBDAECATAIBgZngQwBAgIwDQYJKoZIhvcNAQELBQADggEBAH4jx/LKNW5ZklFc\n" \
                           "YWs8Ejbm0nyzKeZC2KOVYR7P8gevKyslWm4Xo4BSzKr235FsJ4aFt6yAiv1eY0tZ\n" \
                           "/ZN18bOGSGStoEc/JE4ocIzr8P5Mg11kRYHbmgYnr1Rxeki5mSeb39DGxTpJD4kG\n" \
                           "hs5lXNoo4conUiiJwKaqH7vh2baryd8pMISag83JUqyVGc2tWPpO0329/CWq2kry\n" \
                           "qv66OSMjwulUz0dXf4OHQasR7CNfIr+4KScc6ABlQ5RDF86PGeE6kdwSQkFiB/cQ\n" \
                           "ysNyq0jEDQTkfa2pjmuWtMCNbBnhFXBYejfubIhaUbEv2FOQB3dCav+FPg5eEveX\n" \
                           "TVyMnGo=\n" \
                           "-----END CERTIFICATE-----\n";

// Creat a set of new characters
const uint8_t charBitmap[][8] =
{
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
int period = 3000;                  //3000 ms to wait

const char* websockets_connection_string = "wss://rinkeby.infura.io/ws";  //Websocketserver adress "wss://echo.websocket.org"; for testing
const char echo_org_ssl_fingerprint[] PROGMEM = "E0 E7 13 AE F4 38 0F 7F 22 39 5C 32 B3 16 EC BB 95 F3 0B 5B";  // This fingerprint was updated 20.04.2019

String subscriptionLog;                  //SubscriptionLogNumber for verification
uint32_t msgcount = 0;

using namespace websockets;
WebsocketsClient client;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void tokenReceived();

void verifyMsgOnSubscriptionLog(String msg)
{
    Serial.println("Verifying");
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    if(subscriptionLog == doc["params"]["subscription"].as<String>())
    {
        tokenReceived();
    }    
}


void onFirstMessage(String msg)   //First message is used for verification of subscription
{
    StaticJsonDocument<1024>doc;
    deserializeJson(doc, msg);
    subscriptionLog = doc["result"].as<String>();
    Serial.print("created log: "); Serial.println(subscriptionLog);
}


void onMessageCallback(WebsocketsMessage message)
{
    Serial.print("Got Message: ");
    Serial.print(message.data());
    Serial.println(" Message number: ");
    Serial.print(msgcount);
    Serial.println();

    if(msgcount == 0)
    {
        onFirstMessage(message.data());
    }
    else
    {
        verifyMsgOnSubscriptionLog(message.data());
    }
    msgcount++;
}


void onEventsCallback(WebsocketsEvent event, String data)
{
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


void lcdSendToken()
{
    lcd.clear();
    lcd.home ();
    lcd.print("Titan Arcade");
    lcd.setCursor(0, 1);
    lcd.print("Send Titan token");
}


void lcdReceivedToken()
{
    lcd.clear();
    lcd.home ();
    lcd.print("Titan Arcade");
    lcd.setCursor(0, 1);
    lcd.print(" Token received!");
    while(millis() < currentTime + period){}
}


void tokenReceived()
{
    Serial.println("token received");
    lcdReceivedToken();
    digitalWrite(27, LOW);
    delay(1000);
    digitalWrite(27, HIGH);
    lcdSendToken();
}


void initLcd()
{
    int charBitmapSize = (sizeof(charBitmap) / sizeof(charBitmap[0]));
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_PIN, HIGH);
    lcd.begin(16,2);

    for(int i = 0; i < charBitmapSize; i++)
    {
       lcd.createChar ( i, (uint8_t *)charBitmap[i] );
    }

    lcd.home();                   //go to first line
    lcd.print(" Setup");
    lcd.setCursor( 0, 1 );        //go to the next line
    lcd.print("  Starting   ");
}


void initWiFi()
{
    WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
    WiFi.mode(WIFI_STA); //init wifi mode
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY)); //provide identity
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);
    //WiFi.config(ip, gateway, subnet, dnsip, dns2ip);

    //Connect to wifi
    WiFi.begin(ssid);
    Serial.print("Connecting to "); Serial.println(ssid);

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        counter++;
        if (counter >= 60) { //after 30 seconds timeout - reset board (on unsucessful connection)
            ESP.restart();
        }
    }

    Serial.print("Connected to WiFi. IP: ");
    Serial.println(WiFi.localIP());
}


void setup()
{
    Serial.begin(115200);       //Serial for debugging purposes
    initLcd();
    initWiFi();

    //client.setCACert(test_root_ca);
    //Run callback when messages are received
    client.onMessage(onMessageCallback);
    
    //Run callback when events are occuring
    client.onEvent(onEventsCallback);

    //Before connecting, set the ssl fingerprint of the server
    //client.setFingerprint(echo_org_ssl_fingerprint);
    client.connect(websockets_connection_string);               // Connect to server
    client.send("{\"jsonrpc\": \"2.0\", \"id\": 1, \"method\": \"eth_subscribe\", \"params\": [\"logs\", {\"address\": \"0x335a1f5634A440Ae4ca59adddd42AcB4E8dDbeF3\", \"topics\": [\"0x324633c1240a599ae948c2245fead42ace06b9f0e120c7d74045f79b248f59f3\"]}]}");

    lcdSendToken();
}


void loop()
{
    currentTime = millis();
    client.poll();
}
