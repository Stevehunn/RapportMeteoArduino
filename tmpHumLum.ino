#include <U8x8lib.h>  // bibliothèque à charger a partir de 
#include <Wire.h>  // driver I2C
#include "SparkFunHTU21D.h" //Hum, temp
#include <BH1750.h> //lum
#include "WiFi.h"
#include "ThingSpeak.h"
#include "Adafruit_CCS811.h"


// Connection à un réseau précis
char ssid[] = "OnePlus 8T"; // your network SSID (name)
char pass[] = "floflo8t"; // your network passw

// Thingspeak
unsigned long myChannelNumber = 1; 
const char * myWriteAPIKey="NTQY5AKGV1X76854" ; //API Key
WiFiClient client;

//Capteurs de température, humidité et de luminosité
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(15, 4, 16);
BH1750 lightMeter;
HTU21D myTempHumi; 

//Affichage des données des capteurs
void displayData(double temperature, double humidity, uint16_t luminosity)
{ 
  char dbuf[32];
  u8x8.clear();
  Serial.print(" Temperature:"); 
  Serial.print(temperature, 1); 
  Serial.print("C"); 
  Serial.print(" Humidity:"); 
  Serial.print(humidity, 1); 
  Serial.print("%"); 
  Serial.println(); 
  Serial.print("Light: ");
  Serial.print(luminosity);
  Serial.println(" lx");
  delay(1000); 

  sprintf(dbuf,"Humi:%f %",humidity); u8x8.drawString(0,2,dbuf);
  sprintf(dbuf,"Temp:%f C",temperature); u8x8.drawString(0,3,dbuf);
  sprintf(dbuf,"Lum:%f lx",luminosity); u8x8.drawString(0,4,dbuf);
  delay(6000);
  
} 

//Setup
void setup() { 

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
  
  u8x8.begin();  // initialize OLED
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  myTempHumi.begin();
  lightMeter.begin();
  Serial.println("Running...");
}

//Loop
void loop()
{
  float humd = myTempHumi.readHumidity(); 
  float temp = myTempHumi.readTemperature();
  uint16_t lum = lightMeter.readLightLevel(); 

  // Association des Fields
  ThingSpeak.setField(1, humd);
  ThingSpeak.setField(2, temp);
  ThingSpeak.setField(3, lum);

  
  // Affichage des données des capteurs
  displayData(temp,humd,lum);
  
  // Envoie des données
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
  
  delay(10000);

}
