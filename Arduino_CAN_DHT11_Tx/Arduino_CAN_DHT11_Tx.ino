#include "DHT.h"
#include <CAN.h>
/*********************/

#define DHTPIN 9     
#define DHTTYPE DHT11   // DHT 11
/*********************/
#define debug 1   
//Make this to 0 if you dont want to print temp and hum value
/*********************/
void ReadDHT11_Temp_Humidity( );
/*********************/
DHT dht(DHTPIN, DHTTYPE);
float h,t;
unsigned char data[8];
/*********************/
void setup() {
  Serial.begin(9600);
  dht.begin();
  //set up CAN Bus 
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
  Serial.print("Set Up complete");
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  ReadDHT11_Temp_Humidity();
  #if debug
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println();
  #endif
  //copy the temperature and humdity values to buffer
  memcpy(data, &t, 4);
  memcpy(data + 4, &h, 4);

  CAN.beginPacket(0x12);
  CAN.write(data,8);
  CAN.endPacket();
  Serial.println("Tx Completed");
}

void ReadDHT11_Temp_Humidity()
{
   h = dht.readHumidity();
  // Read temperature as Celsius (the default)
   t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
