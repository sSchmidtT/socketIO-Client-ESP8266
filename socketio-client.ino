#include <ESP8266WiFi.h>
#include <SocketIoClient.h>

SocketIoClient socketIO;

void setup() {
  // put your setup code here, to run once:

  WiFi.begin("TP-Link_4FEF", "22022019");
  Serial.print("Connecting");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  socketIO.begin("10.1.1.110", 3005);

  socketIO.on("reply", messageEventHandler);

}

void messageEventHandler(const char* payload, size_t length){
  Serial.printf("got message: %s \n", payload);
}

uint64_t messageTimestamp;
void loop() {
  // put your main code here, to run repeatedly:

  socketIO.loop();

  uint64_t now = millis();
  if(now - messageTimestamp > 6000){
    messageTimestamp = now;
    socketIO.emit("message", "\"this is a message from a client\"");
  }

}
