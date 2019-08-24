#include <GFButton.h>
#include <ShiftDisplay.h>
#include <RotaryEncoder.h>
#include <TimerOne.h>

char* nomeDasNotas[] = {"DO ", "REb", "RE ", "MIb", "MI ", "FA ", "SOb", "SOL", "LAb", "LA ", "SIb", "SI "};
int frequencias[] = {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247};

int bpm = 0;
bool beep = false;

int notaAtual = 0;
int campainhaPassiva = 5;
int terra = A5;
GFButton botao1(A1);
GFButton botao2(A2);
unsigned long instanteAnterior = 0;
unsigned long t1 = 0;
ShiftDisplay display(4, 7, 8, COMMON_CATHODE, 4, true);
RotaryEncoder encoder(20, 21);
int posicaoAnterior = 0;
int leds[4] = {13,12,11,10};

void setup() {
    Serial.begin(9600); 
    pinMode(campainhaPassiva, OUTPUT);
    pinMode(terra, OUTPUT);
    digitalWrite(terra,LOW);
    botao1.setPressHandler(afinador);
    botao1.setReleaseHandler(para_campainha);
    botao2.setPressHandler(metronomo1);
    botao2.setReleaseHandler(metronomo2);
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

void funcBeep(){
  tone(campainhaPassiva, 440, 100);
}

void metronomo1(){
  Timer1.stop();
  beep=false;
  t1 = millis();
}
void metronomo2(){
  beep = true;
  unsigned long diff = (millis()-t1);
  bpm = 60000 / diff;
  Timer1.attachInterrupt(funcBeep);
  Timer1.initialize(diff*1000);
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

void afinador(){
  tone(campainhaPassiva, frequencias[notaAtual]);
  beep = false;
  Timer1.stop();
}

void toca_campainha()
{
  Serial.println("Tocou");
  tone(campainhaPassiva, 440, 500);
}

void loop() {
   botao1.process();
   botao2.process();
   if (beep){
      display.set(bpm);
   }
   else{
      display.set(nomeDasNotas[notaAtual]);
      int posicao = encoder.getPosition();
      if (posicao > posicaoAnterior) {
        Serial.println(posicao);
        if (notaAtual < 11)
        {
          notaAtual++;
          tone(campainhaPassiva, frequencias[notaAtual], 200);
        }
        posicaoAnterior = posicao;
      }
      else if (posicao < posicaoAnterior) {
        Serial.println(posicao);
        if (notaAtual > 0)
        {
          notaAtual--;
          tone(campainhaPassiva, frequencias[notaAtual], 200);
        }
        posicaoAnterior = posicao;
      }
   }
   display.update();
    
}

