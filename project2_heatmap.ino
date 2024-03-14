#include <WiFi.h>
#include <TinyGPSPlus.h>
#include "WiFi.h"
#include "TFT_eSPI.h"


#define VERDE_OSCURO 0x45A0
#define VERDE 0x6E00
#define VERDE_CLARO 0x9E80
#define AMARILLO_VERDE 0xE780
#define AMARILLO 0xF763
#define AMARILLO_NARANJA 0xF680
#define NARANJA_CLARO 0xF5C0
#define NARANJA 0xF500
#define NARANJA_OSCURO 0xEBC2
#define ROJO_NARANJA 0xEA83
#define ROJO 0xE963


TFT_eSPI tft = TFT_eSPI();
// The TinyGPSPlus object
TinyGPSPlus gps;

const char* ssid = "S22 chicha"; // Ingresar Nombre de la red
const char* password = "19032004"; // Ingresar Contrasenha 

double avr_lat = -12.08300; // Calibrar Latitud de referencia
double avr_lng = -76.96900; // Calibrar Longitud de referencia

double lat = 0;
double lng = 0;
int32_t rssi;
int x;
int y;
uint32_t color;

void setup()
{
  
  Serial.begin(9600);
  Serial2.begin(9600);
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  int i = 0;

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    i++;
    tft.drawPixel(i, 0, TFT_WHITE);
    delay(100);
  }
  tft.drawLine(0, 0, 320, 0, TFT_BLACK);

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  while (Serial2.available() > 0)
  {
    if (gps.encode(Serial2.read()))
    {
      if (gps.location.isValid()){
        calculateAndDisplayInfo();
        x = haciax(lat);
        y = haciay(lng);
        color = rssi_a_color(rssi);
        tft.fillRect(x-1, y-1, 3, 3, color);
        delay(500);
      }
    }

  }
      
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No Gps Detectado"));
    while (true);
  }

}

void calculateAndDisplayInfo()
{
  if (gps.location.isValid()){
    lat = gps.location.lat();
    lng = gps.location.lng();
    if(WiFi.status() == WL_CONNECTED){
      rssi = WiFi.RSSI();
    } else {
      rssi = -1000;
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      int k = 0;
      while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        k++;
        tft.drawPixel(k, 239, TFT_WHITE);
        delay(100);
      }
      tft.drawLine(0, 239, 319, 239, TFT_BLACK);
    }
    Serial.print("Lat: ");
    Serial.print(lat, 5);
    Serial.print(F(","));
    Serial.print(" Lng: ");
    Serial.print(lng, 5);
    Serial.print(", avr_lat: ");
    Serial.print(avr_lat, 5);
    Serial.print(", avr_lng: ");
    Serial.print(avr_lng, 5);
    Serial.print(" ---> x: ");
    Serial.print(x);
    Serial.print(", y: ");
    Serial.print(y);
    Serial.print(", RSSI:");
    Serial.print(rssi);
    Serial.println();
    tft.drawString('lat: ' + String(lat, 5), 0, 170);
    tft.drawString('lng: ' + String(lng, 5), 0, 180);
    tft.drawString('x: ' + String(x), 0, 190);
    tft.drawString('y: ' + String(y), 0, 200);
    tft.drawString('rssi: ' + String(rssi), 0, 210);
  }  
  else
  {
    Serial.print(F("Invalido "));
  }
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    Serial2.write(Serial.read());
  }
  while (Serial2.available())
  {
    Serial.write(Serial2.read());
  }
}

int haciax(double lat){
  return (int)((lat*-100000)-(avr_lat*-100000));
}

int haciay(double lng){
  return (int)((lng*-100000)-(avr_lng*-100000));
}

uint32_t rssi_a_color(int32_t rssi)
{
  if (rssi >= -40) { return VERDE_OSCURO; } 
  else if (rssi >= -50) { return AMARILLO; }
  else if (rssi >= -60) { return NARANJA_CLARO; }
  else if (rssi >= -70) { return NARANJA_OSCURO; }
  else if (rssi >= -80) { return ROJO_NARANJA; }
  else { return ROJO; }
}
