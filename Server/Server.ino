#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "DoorControl";  // Enter SSID here
const char* password = "cteadmin";  //Enter Password here

const int BUTTON_PIN = 5;

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

int gotRequest = HIGH;
int locked = 1;
void loop() {
  gotRequest = HIGH;
  server.handleClient();
  digitalWrite(LED_BUILTIN, gotRequest);
  if (digitalRead(BUTTON_PIN) == LOW){
    locked = 0;
  }
}

void handle_OnConnect() {
  Serial.println("Got request");
  gotRequest = LOW;
  server.send(200, "text/plain", locked ? "locked" : "unlocked"); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}
