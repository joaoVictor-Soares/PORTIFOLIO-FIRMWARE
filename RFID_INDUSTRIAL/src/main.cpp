#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

#define SS_PIN 21
#define RST_PIN 22
#define LED_VERMELHO 25
#define LED_VERDE 26
#define PINO_SERVO 14
#define CANAL_PWM 0
#define FREQUENCIA 50
#define RESOLUCAO 16

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* url_servidor = "http://10.110.12.82:5432/api/rfid";

bool permitido = false;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
MFRC522 rfid(SS_PIN, RST_PIN);

void conectarWifi() {
  Serial.print("Conectando ao Wifi: ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWifi conectado");
}

void atualizarDisplay(String status, String mensagem) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.println(status);
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.println(mensagem);
  display.display();
}

void moverServo(int graus) {
  int dutyCycle = map(graus, 0, 180, 1638, 8192);
  ledcWrite(CANAL_PWM, dutyCycle);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, LOW);

  SPI.begin(18, 19, 23, 21);
  rfid.PCD_Init();

  Wire.begin(4, 5);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for (;;);
  display.setTextColor(SSD1306_WHITE);
  atualizarDisplay("AGUARDANDO", "Aproxime a tag...");

  conectarWifi();

  ledcSetup(CANAL_PWM, FREQUENCIA, RESOLUCAO);
  ledcAttachPin(PINO_SERVO, CANAL_PWM);
  moverServo(0);

  Serial.println("--- Sistema HTTP Pronto ---");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String tagLida = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) tagLida += "0";
    tagLida += String(rfid.uid.uidByte[i], HEX);
  }
  tagLida.toUpperCase();

  String payload = "{\"TagResponsavel\":\"" + tagLida + "\",\"sensor\":\"RFID_01\",\"timestamp\":\"\"}";

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url_servidor);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Resposta: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro HTTP: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  if (tagLida == "55667788") {
    permitido = !permitido;
    if (permitido) {
      atualizarDisplay("PERMITIDO", "Salvando acao...");
      digitalWrite(LED_VERDE, HIGH);
      moverServo(90); delay(2000);
      moverServo(180); delay(2000);
      moverServo(0); delay(2000);
    } else {
      atualizarDisplay("DESLIGANDO", "Salvando acao...");
      delay(1000);
    }
  } else {
    atualizarDisplay("RECUSADO", "Tag invalida...");
    digitalWrite(LED_VERMELHO, HIGH);
    delay(3000);
  }

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  atualizarDisplay("AGUARDANDO", "Aproxime a tag...");
  rfid.PICC_HaltA();
}