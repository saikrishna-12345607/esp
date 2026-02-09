#include <Arduino.h>
#include<Adafruit_Sensor.h>
#include<DHT.h>
#define DHTPIN 19    
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(921600);
  Serial.println("dht11 test");
  dht.begin();
}

void loop() {
  delay(1000);
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  float f=dht.readTemperature( true);
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("failed to read the from the sensor ");
    return;
    }
    float hif=dht.computeHeatIndex(f,h);
    float hic=dht.computeHeatIndex(t,h,false);
    Serial.print("humidity:");
    Serial.print(h);
    Serial.print("temperature:");
    Serial.print(t);
    Serial.print("c");
    Serial.print(f);
    Serial.print("heat index ");
    Serial.print(hif);
    Serial.print("oC");
    Serial.print(hic);
    Serial.println("oF");

}

