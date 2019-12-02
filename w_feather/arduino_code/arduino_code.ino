

#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
#include "projection.h"

char ssid[] = "feater_test";        // your network SSID (name)
char pass[] = "123456";
int status = WL_IDLE_STATUS;
WiFiServer server(80);




void setup() {
  // put your setup code here, to run once:
  WiFi.setPins(8,7,4,2);
  pinMode(motor1_pin, OUTPUT);
  pinMode(motor2_pin, OUTPUT);
  pinMode(motor3_pin, OUTPUT);
  pinMode(motor4_pin, OUTPUT);
  pinMode(motor5_pin, OUTPUT);
  pinMode(motor6_pin, OUTPUT);

  
  //with USB connected
//  while (!Serial) {; }
  Serial.println("Access Point Web Server");
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.print("Creating access point named: ");
  Serial.println(ssid);
  
  status = WiFi.beginAP(ssid);
  
  if (status != WL_AP_LISTENING) {
  Serial.println("Creating access point failed");
  // don't continue
  while (true);
  }

  delay(1000);
  
  // start the web server on port 80
  server.begin();
  printWiFiStatus();
  
}

void loop() {
  StaticJsonDocument<100> doc;
  // put your main code here, to run repeatedly:
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      printMacAddress(remoteMac);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

   WiFiClient client = server.available();
   while (client) {                             // if you get a client,
    
//    Serial.println("new client");           // print a message out the serial port
    String bufferd = "";                // make a String to hold incoming data from the client
    int flag=0;
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();
//        Serial.write(c);      
        if (c == '['){
          flag = 1;
        }

        if (flag==1){
          bufferd+=c;
          if (c==']'){
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println();
            break;
          }
         }
    
    }
   }
    Serial.println(bufferd);
    client.stop();
//    Serial.println("client disconnected");
//    float x=bufferd[1];
//    float y=bufferd[3];
//    Serial.print(x);
//    Serial.print(y); 
      deserializeJson(doc, bufferd);
      JsonArray array = doc.as<JsonArray>();
      JsonVariant x_=array[0], y_=array[1];
      float x=x_.as<float>(), y=y_.as<float>();
      set_vibration(x,y);
      
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
