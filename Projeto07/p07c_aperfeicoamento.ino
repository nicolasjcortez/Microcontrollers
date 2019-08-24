#include <GFButton.h>
#include <ShiftDisplay.h>
#include <RotaryEncoder.h>
#include <TimerOne.h>

char* nomeDasNotas[] = {"DO ", "REb", "RE ", "MIb", "MI ", "FA ", "SOb", "SOL", "LAb", "LA ", "SIb", "SI "};
int frequencias[] = {131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247};
int oitavaAtual = 0;
unsigned long temposDeEstalos[100];


int bpm = 0;
bool beep = false;
bool bot2 = false;
int sensorDeSom = 19;
int notaAtual = 0;
int campainhaPassiva = 5;
int terra = A5;
GFButton botao1(A1);
GFButton botao2(A2);
unsigned long instanteAnterior = 0;
unsigned long t1 = 0;
unsigned long tIni = 0;
unsigned long tFin = 0;
int tAtual = 0;

ShiftDisplay display(4, 7, 8, COMMON_CATHODE, 4, true);
RotaryEncoder encoder(20, 21);
int posicaoAnterior = 0;
int leds[4] = {13,12,11,10};

void setup() {
    zeraVet();
    Serial.begin(9600); 
    pinMode(campainhaPassiva, OUTPUT);
    pinMode(terra, OUTPUT);
    digitalWrite(terra,LOW);
    botao1.setPressHandler(afinador);
    botao1.setReleaseHandler(para_campainha);
    botao2.setPressHandler(metronomo1);
    botao2.setReleaseHandler(metronomo2);
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

void zeraVet(){
  for(int i =0;i<100; i++)
  {
    temposDeEstalos[i]=0; 
  }
}

void tickDoEncoder() {
  encoder.tick();
}

void somDetectado()
{
  if(bot2 == true)
  {
    unsigned long instanteAtual = millis();
    if (instanteAtual > instanteAnterior + 10) {
      if(t1 == 0)
      {
        t1 = millis();
        tIni = t1 - tIni;
      }
      else
      {
        unsigned long diff = millis() - t1;
        //numeroDeEstalos++;
        //Serial.println(numeroDeEstalos);
        int i;
        for(i=0; temposDeEstalos[i]!=0; i++){}
        temposDeEstalos[i]=diff;
        t1 = millis();
      }
      instanteAnterior = instanteAtual;
    }
  }
}

void funcBeep(){
  tone(campainhaPassiva, 440, 100);
}

void metronomo1(){
  zeraVet();
  tIni = millis();
  t1 =0;
  tAtual = 0;
  bot2 = true;
}
void metronomo2(){
  bot2 = false;
  tFin = millis() - t1;
  for(int i=0; temposDeEstalos[i]!=0; i++)
  {
    Serial.println(temposDeEstalos[i]);
  }
  Serial.println("---");
  Timer1.attachInterrupt(updateTimer);
  if(temposDeEstalos[0]!=0)
  {
    Timer1.initialize(tIni*1000);
  }
}

void updateTimer()
{
  tone(campainhaPassiva, 220, 50);
  if(temposDeEstalos[tAtual]!= 0)
  {
    Timer1.initialize(temposDeEstalos[tAtual]*1000);
    tAtual++;
  }
  else{
    Timer1.initialize((tIni+tFin)*1000);
    tAtual = 0;
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

void afinador(){
  tone(campainhaPassiva, frequencias[notaAtual]);
  beep = false;
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
      String string1 = String(nomeDasNotas[notaAtual]);
      String string2 = String(oitavaAtual);
      display.set(string1 + string2);
      int posicao = encoder.getPosition();
      if (posicao > posicaoAnterior) {
        Serial.println(posicao);        
        if (notaAtual < 11)
        {
          notaAtual++;
          tone(campainhaPassiva, frequencias[notaAtual]*pow(2,oitavaAtual), 200);
        }
        else if(oitavaAtual < 3)
        {
          oitavaAtual++;
          notaAtual = 0;
          tone(campainhaPassiva, frequencias[notaAtual]*pow(2,oitavaAtual), 200);
        }
        posicaoAnterior = posicao;
      }
      else if (posicao < posicaoAnterior) {
        Serial.println(posicao);
        if (notaAtual > 0)
        {
          notaAtual--;
          tone(campainhaPassiva, frequencias[notaAtual]*pow(2,oitavaAtual), 200);
        }
        else if(oitavaAtual > 0)
        {
          oitavaAtual--;
          notaAtual = 11;
          tone(campainhaPassiva, frequencias[notaAtual]*pow(2,oitavaAtual), 200);
        }
        posicaoAnterior = posicao;
      }
   }
   display.update();
    
}

