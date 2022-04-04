#include "Adafruit_CCS811.h"
#include <Wire.h>  // driver I2C
#include <SPI.h> // include libraries
#include <LoRa.h>
#define SCK 5 // GPIO5 SX127x's SCK
#define MISO 19 // GPIO19 SX127x's MISO
#define MOSI 27 // GPIO27 SX127x's MOSI
#define SS 18 // GPIO18 SX127x's CS
#define RST 14 // GPIO14 SX127x's RESET
#define DI0 26 // GPIO26 SX127x's IRQ(Interrupt Request)
#define freq 433E6
#define sf 8
#define sb 125E3


Adafruit_CCS811 ccs;

union pack
{
  uint8_t frame[16]; // trames avec octets
  float data[4]; // 4 valeurs en virgule flottante
} sdp ; // paquet d’émission


void setup()
{

  //Setup Sensor
  Serial.begin(9600);
  delay(200);
  Serial.println("CCS811 test");
  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }
  //calibrate temperature sensor
  while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);


  //Setup LoRa
  Serial.begin(9600);
  pinMode(DI0, INPUT); // signal interrupt
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(sf);
  LoRa.setSignalBandwidth(sb);


}

float etemp = 20.0;
int co2, tvoc; char dbuff[24];

void loop() {
  if (ccs.available()) {
    float temp = ccs.calculateTemperature();
    if (!ccs.readData()) {
      Serial.print("CO2: ");
      co2 = ccs.geteCO2();
      Serial.print(co2);
      tvoc = ccs.getTVOC();
      Serial.print("ppm, TVOC: ");
      Serial.print(tvoc);
      Serial.print("ppb Temp:");
      etemp = 0.02 * temp + 0.98 * etemp;
      Serial.println(temp);
      Serial.print("Est temp:"); Serial.println(etemp);

    }
    else {
      Serial.println("ERROR!");
      delay(5000);
    }
  }
  Serial.println("New Packet") ;
  LoRa.beginPacket(); // start packet
  sdp.data[0] = co2;
  sdp.data[1] = tvoc;
  LoRa.write(sdp.frame, 16);
  LoRa.endPacket();
  delay(9000);
}
