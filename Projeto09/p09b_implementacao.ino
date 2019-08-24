#include <AFMotor.h>
#include <TimerOne.h>

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
int sensorOtico = A11;
int sensorOtico2 = A12;
void setup() {
    pinMode(sensorOtico, INPUT);
    pinMode(sensorOtico2, INPUT);
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial1.setTimeout(10);
    Timer1.initialize(100000);
    Timer1.attachInterrupt(dizValorSensor);
}
void dizValorSensor(){
  Serial.println(String(digitalRead(sensorOtico)) + " " +
  String(digitalRead(sensorOtico2)));
  Serial1.println(String(digitalRead(sensorOtico)) + " " +
  String(digitalRead(sensorOtico2)));
}
void loop() {
    String comando = Serial1.readString();
    comando.trim();
    if(comando != "")
    {      
      if(comando.startsWith("frente")){
        motor3.setSpeed(155);
        motor3.run(FORWARD);
        motor4.setSpeed(160);
        motor4.run(FORWARD);
      }
      else if(comando.startsWith("tras")){
        motor3.setSpeed(155);
        motor3.run(BACKWARD);
        motor4.setSpeed(160);
        motor4.run(BACKWARD);
      }
      else if(comando.startsWith("esquerda")){
        motor3.setSpeed(155);
        motor3.run(BACKWARD);
        motor4.setSpeed(160);
        motor4.run(FORWARD);
      }
      else if(comando.startsWith("direita")){
        motor3.setSpeed(155);
        motor3.run(FORWARD);
        motor4.setSpeed(160);
        motor4.run(BACKWARD);
      }
      else if(comando.startsWith("parar")){
        motor3.run(RELEASE);
        motor4.run(RELEASE);
      }
    }
}

