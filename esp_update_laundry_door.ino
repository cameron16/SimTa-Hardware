/*
 ESP8266 Blink by Simon Peter
 Blink the blue LED on the ESP-01 module
 This example code is in the public domain
 
 The blue LED on the ESP-01 module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "iPhone";
const char* password = "camcamcam";

const char* host = "smartapp-196617.appspot.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate

const char* fingerprint = "89 EF CB 66 89 6A 81 90 3C A6 47 21 E3 EF 62 EE 5C 8D 18 F5";

String state;


WiFiClientSecure client;

int counter = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(2, INPUT);
  pinMode(0, OUTPUT);

  pinMode(13, OUTPUT);  
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  while (!client.connect(host, httpsPort)) {
    delay(500);
  }

  state = "CLOSED";
  
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  String url = "/laundry";
   client.print(String("POST ") + url + " HTTP/1.1\r\n"+
                  "Host: " + host + "\r\n" +
                  "User-  Agent: BuildFailureDetectorESP8266\r\n" +
                  "Content-Type: application/json\r\n" +
                  "Content-Length: 60\r\n" +
                  "\r\n" +
                  "{\"laundry_number\":7,\"location\":\"Roosevelt\",\"door_status\":0}"+"\r\n"+
                  "Connection: close\r\n\r\n");
  
}

void open_laundry_door(){
  while (!client.connect(host, httpsPort)) {
          delay(500);
        }
        if (client.verify(fingerprint, host)) {
          Serial.println("certificate matches");
        } else {
          Serial.println("certificate doesn't match");
        }
        String url = "/laundry";
         client.print(String("POST ") + url + " HTTP/1.1\r\n"+
                        "Host: " + host + "\r\n" +
                        "User-  Agent: BuildFailureDetectorESP8266\r\n" +
                        "Content-Type: application/json\r\n" +
                        "Content-Length: 60\r\n" +
                        "\r\n" +
                        "{\"laundry_number\":7,\"location\":\"Roosevelt\",\"door_status\":1}"+"\r\n"+
                        "Connection: close\r\n\r\n");
  
}

void close_laundry_door(){
  while (!client.connect(host, httpsPort)) {
    delay(500);
  }
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  String url = "/laundry";
   client.print(String("POST ") + url + " HTTP/1.1\r\n"+
                  "Host: " + host + "\r\n" +
                  "User-  Agent: BuildFailureDetectorESP8266\r\n" +
                  "Content-Type: application/json\r\n" +  
                  "Content-Length: 60\r\n" +
                  "\r\n" +
                  "{\"laundry_number\":7,\"location\":\"Roosevelt\",\"door_status\":0}"+"\r\n"+
                  "Connection: close\r\n\r\n");

}


// the loop function runs over and over again forever
void loop() {
  if (digitalRead(16) == LOW){
   //BLINK SLOW. laundry machine door open
    digitalWrite(0, HIGH);
    delay(1000);                      // Wait for a second
    digitalWrite(0,LOW);
    delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
    if (state == "CLOSED"){
      state = "OPEN";
      open_laundry_door();

    }

  }
  else{
    //BLINK FAST. laundry machine door closed
    digitalWrite(0, HIGH);
    delay(100);
    digitalWrite(0, LOW);
    delay(100);
    if (state == "OPEN"){
      close_laundry_door();
      state = "CLOSED";
    }
    
  }

  
}
