#include "WiFi.h"
#include "TFT_eSPI.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Iniciacion Realizada :D");

  
  // Pantallita
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  // tft.drawPixel(x,y,TFT_COLOR);

}

void loop() {
  Serial.println("Iniciando la deteccion");

  int n = WiFi.scanNetworks();
  Serial.println("Deteccion lista :D");
  if (n == 0) {
      Serial.println("No se encontraron redes :c");
  } else {
      Serial.print(n);
      Serial.println(" redes encontradas");
      Serial.println("Nr | SSID                             | BSSID             | RSSI | CH | Encriptacion");
      for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.printf("%2d",i + 1);
          Serial.print(" | ");
          Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
          Serial.print(" | ");
          Serial.print(WiFi.BSSIDstr(i));
          Serial.print(" | ");            
          Serial.printf("%4d", WiFi.RSSI(i));
          Serial.print(" | ");
          Serial.printf("%2d", WiFi.channel(i));
          Serial.print(" | ");
          switch (WiFi.encryptionType(i))
          {
            case WIFI_AUTH_OPEN: Serial.print("open"); break;
            case WIFI_AUTH_WEP: Serial.print("WEP"); break;
            case WIFI_AUTH_WPA_PSK: Serial.print("WPA"); break;
            case WIFI_AUTH_WPA2_PSK: Serial.print("WPA2"); break;
            case WIFI_AUTH_WPA_WPA2_PSK: Serial.print("WPA+WPA2"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
            default: Serial.print("unknown");
          }
          Serial.println();
          delay(10);
      }
  }
  Serial.println("");


  // Mostrar en la pantalla la frecuencia (channel) y el RSSI (dBm) y encima el nombre
  
  // Fondo negro
  tft.fillScreen(TFT_BLACK);
  // Elipses
  for(int i = 0; i < n; ++i)
  {
    long color = random(0xFFFF);
    tft.drawEllipse(WiFi.channel(i)*20+25, 220, 40, (100+WiFi.RSSI(i))*2, color);
    tft.setTextColor(color);
    if(WiFi.encryptionType(i) == WIFI_AUTH_OPEN){
      tft.drawString(WiFi.SSID(i), WiFi.channel(i)*20+25, (220-(100+WiFi.RSSI(i))*2)-8);
    } else {
      tft.drawString(WiFi.SSID(i) + "*", WiFi.channel(i)*20+25, (220-(100+WiFi.RSSI(i))*2)-8);
    }
  }

  tft.setTextColor(TFT_WHITE);

  tft.drawLine(25,20,25,220, TFT_WHITE); // Eje vertical
  tft.drawLine(25,220,285,220, TFT_WHITE); // Eje horizontal
  tft.fillRect(0,221,320,240, TFT_BLACK); // Cubrir mitad de la elipse

  // Dibujar texto del eje vertical
  tft.drawString("dbm", 18, 10);
  tft.drawString("0", 17, 20);
  tft.drawString("-20", 5, 57);
  tft.drawString("-40", 5, 97);
  tft.drawString("-60", 5, 137);
  tft.drawString("-80", 5, 177);
  tft.drawString("-100", 0, 217);

  // Dibujar texto del eje horizontal
  tft.drawString("ch", 294, 216);
  tft.drawString("1", 43, 223);
  tft.drawString("2", 63, 223);
  tft.drawString("3", 83, 223);
  tft.drawString("4", 103, 223);
  tft.drawString("5", 123, 223);
  tft.drawString("6", 143, 223);
  tft.drawString("7", 163, 223);
  tft.drawString("8", 183, 223);
  tft.drawString("9", 203, 223);
  tft.drawString("10", 220, 223);
  tft.drawString("11", 240, 223);
  tft.drawString("12", 260, 223);
  tft.drawString("13", 280, 223);

  // Wait a bit before scanning again
  WiFi.scanDelete();
  delay(5000);
}
