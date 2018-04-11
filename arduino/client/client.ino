// Ljus Firmware
// Author: Tyler

#include <Ethernet.h>
#include <SPI.h>

#define PIN_SWITCH 0
#define PIN_LIGHT 8

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 120 };

const unsigned long requestInterval = 5000;
unsigned long lastAttemptTime = 0;
String resultLine = "";
boolean isResultLine = false;

// Params needed in ping request
String secretKey = "bacon";
String id = "sekund";

EthernetClient client;

void setup() {
  Serial.println("ljus booting up...");

  Ethernet.begin(mac, ip);
  Serial.begin(9600);
  
  pinMode(PIN_SWITCH, INPUT);
  pinMode(PIN_LIGHT, OUTPUT);
  
  // Turn the light off
  turnLightOff();

  // Give ethernet time to boot
  delay(1000);

  pingServer();
}

void loop() {
  if (client.connected()) {
    if (client.available()) {
      char c = client.read();
      
      if (c == '{') {
        isResultLine = true;
      } else if (c == '}') {
        isResultLine = false;
        
        if (resultLine.equals("true")) {
          turnLightOn();
        } else{
          turnLightOff();
        }
        
        Serial.println("RESULT: " + resultLine);
      }
      
      if (isResultLine && c != '{') {
        resultLine += c;
      }
    }
  }

  else if (millis() - lastAttemptTime > requestInterval) {
    client.stop();
    pingServer();
  }
}

void pingServer() {
  resultLine = "";
  boolean on = isSwitchOn();

  if (client.connect("ljus.herokuapp.com", 80)) {    
    String request = "GET /ping?id="+id+"&key="+secretKey+"&on="+on+" HTTP/1.0";
    Serial.println("Making request: " + request);
    client.println(request);
    client.println("HOST: ljus.herokuapp.com");
    client.println();
  } else {
    Serial.println("Error: Unable to contact server!");
  }

  lastAttemptTime = millis();
}

boolean isSwitchOn() {
  return digitalRead(PIN_SWITCH) == HIGH;
}

void turnLightOn() {
  digitalWrite(PIN_LIGHT, HIGH);
}

void turnLightOff() {
  digitalWrite(PIN_LIGHT, LOW);
}
