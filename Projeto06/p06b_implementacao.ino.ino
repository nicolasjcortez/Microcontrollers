#include <GFButton.h>
#include <ShiftDisplay.h>
#include <TimerOne.h>

GFButton botao1(A1);
GFButton botao2(A2);
GFButton botao3(A3);

ShiftDisplay display(4, 7, 8, COMMON_ANODE, 4, true);
//ShiftDisplay display(4, 7, 8, COMMON_CATHODE, 4, true);

int timer = 0;
int buzzer = 3;

bool tocou = false;
bool andamento = false;

void aumenta_timer(){
  timer+=5;
  tocou = false;
}

void diminui_timer(){
  if (timer>=5)
    timer-=5;
  else
    timer=0;
}

void tocar_bz(){
  int i;
  if (!tocou)
    for(i=0;i<4;i++){
      digitalWrite(buzzer, LOW);
      delay(10000);
      digitalWrite(buzzer, HIGH);
      delay(10000);
      tocou = true;
      andamento = false;
    }
}

void decresce1(){
  if (andamento)
    if (timer>0)
      timer--;
    else
      tocar_bz();
}

void comeca(){
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(decresce1); 
  andamento = true;
}

float converte(int tempo){
  float m,s;
  
  m=(float)(tempo/60);
  s=(float)(tempo%60);
  return  m+s/100;
}

void setup() {
  // put your setup code here, to run once:
  botao1.setPressHandler(aumenta_timer);
  botao2.setPressHandler(diminui_timer);
  botao3.setPressHandler(comeca);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  botao1.process();
  botao2.process();
  botao3.process();
  display.set(converte(timer),2,1);
  display.update();
 
}
