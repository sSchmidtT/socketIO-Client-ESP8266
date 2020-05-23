#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SocketIoClient.h>
#define USE_SERIAL Serial

SocketIoClient socketIO;

//Correção nivel lógico invertido
#define ON LOW
#define OFF HIGH

const char* ssid = "TP-Link_4FEF"; //Enter SSID
const char* password = "22022019"; //Enter Password
const char* socketio_server_host = "10.1.1.110"; //Enter server adress
const uint16_t socketio_server_port = 3005; // Enter server port

const char* nameDevice = "arduQuarto";
const int GPIO_0 = 0;

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n", payload);
}

void eventDisconnect(const char * payload, size_t length) {
  USE_SERIAL.printf("got disconnect: %s\n", payload);
  
}

void eventConnect(const char * payload, size_t length) {
  USE_SERIAL.println("got connect");
  
}

void createEvent(const char * payload, size_t length) {
  USE_SERIAL.printf("event: %s \n", payload);
  
}

void eventCreate(const char * payload, size_t length){
  USE_SERIAL.printf("got create: %s\n", payload);
  StaticJsonDocument<200> doc;
  JsonObject array = doc.to<JsonObject>();

  StaticJsonDocument<200> docDev;
  JsonObject dev = docDev.to<JsonObject>();
  dev["name"] = nameDevice;
  dev["mac"] = WiFi.macAddress();

  StaticJsonDocument<200> docPin;
  JsonArray pinOuts = docPin.to<JsonArray>();
  JsonObject pin1 = pinOuts.createNestedObject();
  pin1["pintype"] = "D";
  pin1["name"] = "Lampada";
  pin1["number"] = GPIO_0;
  pin1["type"] = "output";
  pin1["active"] = true;

  dev["pinOuts"] = docPin;
  array["dev"] = docDev;
  char output[200];
  //String output;
  serializeJson(doc, output);

  USE_SERIAL.println(output);
  socketIO.emit("created", output);
}

void setup() {
  // put your setup code here, to run once:

  USE_SERIAL.begin(115200);

  USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  WiFi.begin(ssid, password);
  USE_SERIAL.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println();
  USE_SERIAL.print("Connected, IP address: ");
  USE_SERIAL.println(WiFi.localIP());

  pinMode(GPIO_0, OUTPUT);
  //digitalWrite(GPIO_0,OFF);

  socketIO.on("connect", eventConnect);
  socketIO.on("disconnect", eventDisconnect);
  socketIO.on("reply", event);
  socketIO.on("create", eventCreate);
  socketIO.on("create-event", createEvent);
  socketIO.begin(socketio_server_host, socketio_server_port);
}

uint64_t messageTimestamp;
void loop() {
  // put your main code here, to run repeatedly:

  socketIO.loop();
  //uint64_t now = millis();
  //if(now - messageTimestamp > 6000){
  // messageTimestamp = now;
  //socketIO.emit("message", "\"this is a message from a client\"");

  //}

}
