#include "Adafruit_CCS811.h"
#include <Wire.h>  // driver I2C
#include "WiFi.h"
#include "ThingSpeak.h"

Adafruit_CCS811 ccs;

// Connection à un réseau précis
char ssid[] = "OnePlus 8T"; // your network SSID (name)
char pass[] = "floflo8t"; // your network passw

// Thingspeak
unsigned long myChannelNumber = 1; 
const char * myWriteAPIKey="NTQY5AKGV1X76854" ; //API Key
WiFiClient client;


void setup()
{

  //Setup WiFi
  
  Serial.begin(9600);
  WiFi.disconnect(true); // effacer de l’EEPROM WiFi credentials

  delay(1000);
  WiFi.begin(ssid, pass);
  delay(1000);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println("WiFi setup ok");
  
  delay(1000);
  ThingSpeak.begin(client); // connexion (TCP) du client au serveur
  delay(1000);
  
  Serial.println("ThingSpeak begin");
  
  delay(100);
  Wire.begin(); 
  Serial.begin(9600);


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
}

float etemp=20.0;
int co2,tvoc; char dbuff[24];

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


      ThingSpeak.setField(4, co2);
      ThingSpeak.setField(5, tvoc);
      
      
      // Envoie des données
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
      delay(10000);

      
    }
    else {
      Serial.println("ERROR!");
      delay(5000);
    }
  }
}
