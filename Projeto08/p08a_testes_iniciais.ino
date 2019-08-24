#include<Servo.h>
#include<EEPROM.h>
#include<GFButton.h>

int poten = A5;
int joy[] = {A1, A2};
int base = 12, ombro = 11, cotovelo = 10, garra = 9;
GFButton botao[] = {2, 3, 4, 5};
int cont;
Servo servo_b, servo_o;
int ang_serv = 90;

void aperta_botao1 (GFButton& botaoDoEvento)
{
  EEPROM.get(0, cont);
  cont++;
  Serial.println(cont);
  EEPROM.put(0, cont);
}

void setup()
{
  Serial.begin(9600);
  botao[1].setPressHandler(aperta_botao1);
  pinMode(poten, INPUT);
  servo_b.attach(base);
  servo_o.attach(ombro);
}

void loop()
{
  for (int i = 0; i < 4; i++) botao[i].process();
  int vA = analogRead(poten);
  int vM = map(vA, 0 , 1023, 0, 180);
  servo_b.write(vM);
  int pres = digitalRead(2);
  if (pres == LOW)
  {
    if (ang_serv > 45) ang_serv--;
    servo_o.write(ang_serv);
    delay(15);
  }
  pres = digitalRead(4);
  if (pres == LOW)
  {
    if (ang_serv < 135) ang_serv++;
    servo_o.write(ang_serv);
    delay(15);
  }
}

