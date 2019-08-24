#include <GFButton.h>
#include <ShiftDisplay.h>
#include <RotaryEncoder.h>

int campainhaPassiva = 5;
int terra = A5;
GFButton botao1(A1);
GFButton botao2(A2);
unsigned long instanteAnterior = 0;
int sensorDeSom = 19;
int numeroDeEstalos = 0;
ShiftDisplay display(4, 7, 8, COMMON_CATHODE, 4, true);
RotaryEncoder encoder(20, 21);
int posicaoAnterior = 0;
int leds[4] = {13,12,11,10};

void setup() {
    Serial.begin(9600); 
    pinMode(campainhaPassiva, OUTPUT);
    pinMode(terra, OUTPUT);
    digitalWrite(terra,LOW);
    botao1.setPressHandler(toca_campainha);
    botao2.setPressHandler(toca_continua);
    botao2.setReleaseHandler(para_campainha);
    int origem = digitalPinToInterrupt(sensorDeSom);
    attachInterrupt(origem, somDetectado, RISING);
    int origem1 = digitalPinToInterrupt(20);
  attachInterrupt(origem1, tickDoEncoder, CHANGE);
  int origem2 = digitalPinToInterrupt(21);
  attachInterrupt(origem2, tickDoEncoder, CHANGE);
  pinMode(leds[0], OUTPUT);
  pinMode(leds[1], OUTPUT);
  pinMode(leds[2], OUTPUT);
  pinMode(leds[3], OUTPUT);
    
}

void tickDoEncoder() {
  encoder.tick();
}

void somDetectado()
{
  unsigned long instanteAtual = millis();
  if (instanteAtual > instanteAnterior + 10) {
    numeroDeEstalos++;
    Serial.println(numeroDeEstalos);
    instanteAnterior = instanteAtual;
  }
}

void updateLeds(int posicao)
{
   int resto = abs(posicao)%5;
   int i = 0;
   for(i = 0; i<resto; i++)
   {
      digitalWrite(leds[3-i], HIGH);
   }
   for(; i<4; i++)
   {
      digitalWrite(leds[3-i], LOW);
   }
}

void para_campainha()
{
  noTone(campainhaPassiva);
}

void toca_continua()
{
  tone(campainhaPassiva, 220);
}

void toca_campainha()
{
  Serial.println("Tocou");
  tone(campainhaPassiva, 440, 500);
}

void loop() {
   display.update();
   display.set(numeroDeEstalos);
    botao1.process();
    botao2.process();
    int posicao = encoder.getPosition();
    if (posicao != posicaoAnterior) {
      Serial.println(posicao);
      updateLeds(posicao);
      posicaoAnterior = posicao;
    }
}

