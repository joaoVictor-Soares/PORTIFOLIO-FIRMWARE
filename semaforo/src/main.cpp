#include <Arduino.h>

#define LED_VERMELHO 12
#define LED_AMARELO 13
#define LED_VERDE 14
#define BUTTON 25

unsigned long tempoAtual = 0;
unsigned long tempoReferencia = 0;
unsigned long tempoDebounce = 0;
const unsigned long intervaloDebounce = 250;

volatile bool botaoPressionado = false;

// a IRAM guarda instruções na parte da memória executável do firmware
// funções marcadas com IRAM_ATTR são copiadas para a IRAM na inicialização, permitindo a execução direta e rápida pelo processador
// uso comum em tarefas de tempo real, como interrupções ou controle de periféricos
void IRAM_ATTR trataBotao(); 

void setup() {
  Serial.begin(115200);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // a sintaxe recebe o pino da interrupção, ebglobado por digitalPinToInterrupt(pino), que converte o número do pino digital para um número específico dentro da interrupção
  // além da função que será acionada e o tipo de mudança de estado que aciona o botão
  // FALLING é usado para acionar a interrupção quando o estado do botão mudar de HIGH para LOW
  attachInterrupt(digitalPinToInterrupt(BUTTON), trataBotao, FALLING); // uso importante para avisar o código que ele deve fazer alguma coisa, evitando polling e travamentosvia delay no código, além de capturar eventos muio curtos com maior precisão
  // importante para sleep mode
}

void loop() {
  tempoAtual = millis();
  
  // LÓGICA DO PEDESTRE
  if (botaoPressionado) {
    Serial.println("Botao acionado! Iniciando ciclo de seguranca...");
    
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    delay(2000); 

    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(5000);

    botaoPressionado = false;
    tempoReferencia = millis(); 
  }

  // LÓGICA DO SEMÁFORO NORMAL
  unsigned long tempoDecorrido = tempoAtual - tempoReferencia;

  if (tempoDecorrido < 4000) { 
    digitalWrite(LED_VERMELHO, HIGH);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, LOW);
  } 
  else if (tempoDecorrido < 8000) {
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERDE, HIGH);
  } 
  else if (tempoDecorrido < 10000) { 
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
  } 
  else {
    tempoReferencia = tempoAtual; 
  }
}

void IRAM_ATTR trataBotao() {
  // verifica se o tempo desde o último clique é maior que o debounce
  if ((millis() - tempoDebounce) > intervaloDebounce) {
    botaoPressionado = true;
    tempoDebounce = millis();
  }
}