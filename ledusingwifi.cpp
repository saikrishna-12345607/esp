#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "Sai";
const char* password = "12345678";

WiFiServer server(80);
const int led1 = 16;
const int led2 = 17;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/LED1ON") != -1) digitalWrite(led1, HIGH);
    if (request.indexOf("/LED1OFF") != -1) digitalWrite(led1, LOW);
    if (request.indexOf("/LED2ON") != -1) digitalWrite(led2, HIGH);
    if (request.indexOf("/LED2OFF") != -1) digitalWrite(led2, LOW);

    
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print("<!DOCTYPE html><html><body><h1>ESP32 LED Control</h1>");
    client.print("<p>LED 1: <a href='/LED1ON'><button>ON</button></a> <a href='/LED1OFF'><button>OFF</button></a></p>");
    client.print("<p>LED 2: <a href='/LED2ON'><button>ON</button></a> <a href='/LED2OFF'><button>OFF</button></a></p>");
    client.print("</body></html>");
    client.stop();
  }
}
