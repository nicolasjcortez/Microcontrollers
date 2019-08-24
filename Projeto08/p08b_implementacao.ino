#include<Servo.h>
#include<EEPROM.h>
#include<GFButton.h>
#include<meArm.h>

int poten = A5;
int joy[] = {A0, A1};
int base = 12, ombro = 11, cotovelo = 10, garra = 9;
GFButton botao[] = {2, 3, 4, 5};
meArm braco(
  180, 0, -pi / 2, pi / 2, // ângulos da base
  135, 45, pi / 4, 3 * pi / 4, // ângulos do ombro
  180, 90, 0, -pi / 2, // ângulos do cotovelo
  30, 0, pi / 2, 0 // ângulos da garra
);
bool f_modo = false;
typedef struct
{
  float x;
  float y;
  float z;
} Posicao;
Posicao p;

void aperta_botao1 (GFButton& botaoDoEvento)
{
  braco.openGripper();
}

void aperta_botao2 (GFButton& botaoDoEvento)
{
  f_modo = !f_modo;
}

void solta_botao1 (GFButton& botaoDoEvento)
{
  braco.closeGripper();
}

void setup()
{
  Serial.begin(9600);
  pinMode(joy[0], INPUT);
  pinMode(joy[1], INPUT);
  p.x = 0;
  p.y = 100;
  p.z = 0;
  braco.begin(base, ombro, cotovelo, garra);
  braco.closeGripper();
  pinMode(poten, INPUT);
  botao[0].setPressHandler(aperta_botao1);
  botao[0].setReleaseHandler(solta_botao1);
  botao[1].setPressHandler(aperta_botao2);
  braco.gotoPoint(p.x, p.y, p.z);
}

void loop()
{
  for (int i = 0; i < 4; i++) botao[i].process();
  int vA = analogRead(poten);
  int j_x = analogRead(joy[0]);
  int j_y = analogRead(joy[1]);
  float vM = map(vA, 0 , 1023, -30, 100);
  p.z = vM;
  if (f_modo == false)
  {
    float j_x_m = map(j_x, 0, 1023, -150, 150);
    float j_y_m = map(j_y, 0, 1023, 100, 200);
    p.x = j_x_m;
    p.y = j_y_m;
    braco.gotoPoint(p.x, p.y, p.z);
  }
  else
  {
    float j_x_m = map(j_x, 0, 1023, -10, 10) + 1;
    float j_y_m = map(j_y, 0, 1023, -10, 10) + 1;
    p.x += j_x_m;
    p.y += j_y_m;
    if (p.x < -150) p.x = -150;
    else if (p.x > 150) p.x = 150;
    if (p.y < 100) p.y = 100;
    else if (p.y > 200) p.y = 200;
    Serial.print(p.x);
    Serial.print("//");
    Serial.println(p.y);
    delay(50);
    braco.goDirectlyTo(p.x, p.y, p.z);
  }
}
