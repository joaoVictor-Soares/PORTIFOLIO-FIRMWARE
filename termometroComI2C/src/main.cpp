#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 17
#define DHTTYPE DHT22
// definindo dimensões do display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// definindo pino de reset do display (não usado)
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

unsigned long tempo = 0;
unsigned long tempoReferencia = 0;

void IRAM_ATTR showDHT();

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1.5);
   
}

void loop() {
   showDHT();
}
//   //TODO: criar uma abstração usando wire.h e verificar o uso do Adafruit_SSD 1306
//   tempo = millis();
//   float umidade = 0;
//   float temperatura = 0;
  
//   unsigned long tempoDecorrido = tempo - tempoReferencia;

//   if(tempoDecorrido < 5000){
//     umidade = dht.readHumidity();
//     temperatura = dht.readTemperature();
//     // Serial.println(umidade);
//     // Serial.println(temperatura);
//     delay(10);
//   }
//   else{
//     tempoReferencia = tempo;
//   }

//   if(isnan(temperatura) || isnan(umidade)){
//     display.setCursor(0,0);
//     display.clearDisplay();
//     display.print("Erro no Sensor!");
//     display.display();
//     return;
//   }

//   display.setCursor(0,0);
//   display.print("Temp:" + String(temperatura) + "C");
//   display.setCursor(0,1);
//   display.print("Umid:" + String(umidade) + "%");
//   display.display();
// }
void IRAM_ATTR showDHT(){
  float temperatura = 0;
  float umidade = 0;

  tempo = millis();

  unsigned long tempoDecorrido = tempo - tempoReferencia;

  if (tempoDecorrido < 2500){
    temperatura = dht.readTemperature();
    delay(10);
    display.clearDisplay();
    display.setCursor(0,20);
    display.print("Temp: " + String(temperatura));
    display.display();
  }
  else if(tempoDecorrido > 2500 && tempoDecorrido < 5000){
    umidade = dht.readHumidity();
    delay(10);
    display.clearDisplay();
    display.setCursor(0,20);
    display.print("Umid: " + String(umidade));
    display.display();
  }
  else{
    tempoReferencia = tempo;
  }

}