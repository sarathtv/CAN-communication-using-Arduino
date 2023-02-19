#include <CAN.h>
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

float temperature,humidity;
unsigned char buff[8];

void setup() {
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(4,0,"Embedded ");
  u8x8.drawString(3,1,"Enthusiast!");
  Serial.begin(9600);
  while (!Serial);
  Serial.println("CAN Receiver Callback");
  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
   // register the receive callback
  CAN.onReceive(onReceive);
}
void loop() {
  u8x8.drawString(0, 11, "Temperature:");
  u8x8.drawString(0, 12, (String(temperature) +"°C").c_str());
  u8x8.drawString(0, 13, "Humidity:");
  u8x8.drawString(0, 14, (String(humidity) + "%").c_str());
  delay(1000);
}
void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received ");
 long  id= CAN.packetId();
 if (id == 0x12)
 {
  Serial.print("ID 12");
  int availbyte=CAN.available();
  Serial.print("Available Bytes :: ");
  Serial.print(availbyte);
  for (int i=0;i<availbyte;i++){
    buff[i]=(unsigned char)CAN.read();
  }
  memcpy(&temperature, buff, 4);
  memcpy(&humidity, buff + 4, 4);
 }
 Serial.println();
 Serial.print(F("Humidity: "));
 Serial.print(humidity);
 Serial.print(F("%  Temperature: "));
 Serial.print(temperature);
 Serial.print(F("°C "));
 Serial.println();
}
