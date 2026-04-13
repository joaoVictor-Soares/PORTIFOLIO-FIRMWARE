#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// define as linhas e as colunas usadas pela lcd
#define col 16
#define lin 2
#define ende 0x27

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

unsigned long tempo = 0;
unsigned long tempoReferencia = 0;


LiquidCrystal_I2C lcd(ende, 16, 2); // cria o objeto lcd com os parâmetros de endereço, nº de linhas e colunas

void setup() {
  // Serial.begin(115200);
  // inicialização do objeto lcd
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void loop() {
  //TODO: criar uma abstração usando wire.h e verificar o uso do Adafruit_SSD 1306
  tempo = millis();
  float umidade = 0;
  float temperatura = 0;
  
  unsigned long tempoDecorrido = tempo - tempoReferencia;

  if(tempoDecorrido < 5000){
    umidade = dht.readHumidity();
    temperatura = dht.readTemperature();
    // Serial.println(umidade);
    // Serial.println(temperatura);
    delay(10);
  }
  else{
    tempoReferencia = tempo;
  }

  lcd.setCursor(0,0);
  lcd.print("Temperatura:" + String(temperatura) + "C");
  lcd.setCursor(0,1);
  lcd.print("Umidade:" + String(umidade) + "%");
}
