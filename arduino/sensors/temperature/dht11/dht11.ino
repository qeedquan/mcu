// https://wiki.keyestudio.com/Ks0068_keyestudio_37_in_1_Sensor_Kit_for_Arduino_Starters#Project_30:_DHT11_Temperature_and_Humidity_Sensor
// https://learn.adafruit.com/dht/using-a-dhtxx-sensor

#include <DHT.h>

enum {
  DHTPIN = 4,
  DHTTYPE = DHT11,
};

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}
  
void loop() {
  float h = dht.readHumidity();
  float c = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(c, h, false);
  
  if (isnan(h) || isnan(c) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(c);
  Serial.print("°C ");
  Serial.print(f);
  Serial.print("°F  Heat index: ");
  Serial.print(hic);
  Serial.print("°C ");
  Serial.print(hif);
  Serial.println("°F");

  delay(1000);
}
