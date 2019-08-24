#include <GFButton.h>
#include <ShiftDisplay.h>
#include <TimerOne.h>

GFButton botao1(A1);
GFButton botao2(A2);
GFButton botao3(A3);

ShiftDisplay display(4, 7, 8, COMMON_ANODE, 4, true);
//ShiftDisplay display(4, 7, 8, COMMON_CATHODE, 4, true);

int timer[] = {0, 0, 0, 0};
int buzzer = 3;

int leds[] = {13,12,11,10};

bool tocou[] = {false, false, false, false};
bool andamento[] = {false, false, false, false};
int indiceTimer = 0;

bool mais_1 = false;

void aumenta_timer(){
  botao1.setHoldTime(1000);
  timer[indiceTimer]+=5;
  tocou[indiceTimer] = false;
}

void aumenta_hold(){
  botao1.setHoldTime(200);
  timer[indiceTimer]+=5;
  tocou[indiceTimer] = false;
}

void diminui_timer(){
  botao2.setHoldTime(1000);
  if (timer[indiceTimer]>=5)
    timer[indiceTimer]-=5;
  else
    timer[indiceTimer]=0;
}

void diminui_hold(){
  botao2.setHoldTime(200);
  if (timer[indiceTimer]>=5)
    timer[indiceTimer]-=5;
  else
    timer[indiceTimer]=0;
}

void tocar_bz(int indice){
  int i;
  if (!tocou[indice])
    for(i=0;i<4;i++){
      digitalWrite(buzzer, LOW);
      delay(10000);
      digitalWrite(buzzer, HIGH);
      delay(10000);
      tocou[indice] = true;
      andamento[indice] = false;
    }
}

void decresce1(){
  for(int i = 0; i<4;i++)
  {
    if (andamento[i])
      if (timer[i]>0)
        timer[i]--;
      else
        tocar_bz(i);
  }
}

void comeca(){
  mais_1 = false;
  botao3.setHoldHandler(comeca_pause); 
  botao3.setHoldTime(1000);
}

void comeca_pause(){
  botao3.setHoldHandler(comeca_zera); 
  botao3.setHoldTime(2000);
  mais_1 = true;
  andamento[indiceTimer] = !andamento[indiceTimer];
}

void comeca_zera(){
  timer[indiceTimer] = 0;
  andamento[indiceTimer] = false;
}

void proximo(){
  if (!mais_1)
    indiceTimer = (indiceTimer+1)%4;
}

float converte(int tempo){
  float m,s;
  
  m=(float)(tempo/60);
  s=(float)(tempo%60);
  return  m+s/100;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(leds[0],OUTPUT);
  pinMode(leds[1],OUTPUT);
  pinMode(leds[2],OUTPUT);
  pinMode(leds[3],OUTPUT);
  botao1.setPressHandler(aumenta_timer);
  botao2.setPressHandler(diminui_timer);
  botao3.setPressHandler(comeca); 
  botao1.setHoldHandler(aumenta_hold);
  botao2.setHoldHandler(diminui_hold);
  botao3.setHoldHandler(comeca_pause); 
  botao3.setReleaseHandler(proximo); 
  botao1.setHoldTime(1000);
  botao2.setHoldTime(1000);
  botao3.setHoldTime(1000);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(decresce1); 
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  botao1.process();
  botao2.process();
  botao3.process();
  display.set(converte(timer[indiceTimer]),2,1);
  display.update();
  for(int i;i<4;i++)
  {
    if( i !=indiceTimer )
      digitalWrite(leds[i],HIGH);
    else
      digitalWrite(leds[i],LOW);
  }
}
