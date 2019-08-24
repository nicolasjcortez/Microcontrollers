#include <AFMotor.h>
#include <TimerOne.h>

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
int sensorOtico = A11;
int sensorOtico2 = A12;
bool isManual = true;
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
  //Serial.println(String(digitalRead(sensorOtico)) + " " +
 // String(digitalRead(sensorOtico2)));
  Serial1.println(String(digitalRead(sensorOtico)) + " " +
  String(digitalRead(sensorOtico2)));
}
void loop() {
    String comando = Serial1.readString();
    comando.trim();
    if(comando != "")
    {
       Serial.println(comando);
      if(comando == "automatico")
      {
        isManual = false;
        
      }
      else if(comando == "manual"){
        isManual = true;
        motor3.run(RELEASE);
        motor4.run(RELEASE);
      }
    }
      if(isManual)
      {
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
    else{
      if(digitalRead(sensorOtico) == HIGH && 
      digitalRead(sensorOtico2) == HIGH){
         motor3.setSpeed(155);
          motor3.run(FORWARD);
          motor4.setSpeed(160);
          motor4.run(FORWARD);
          Serial.println("frente");
      }
      else if(digitalRead(sensorOtico) == LOW && 
      digitalRead(sensorOtico2) == HIGH){
        motor3.setSpeed(155);
          motor3.run(FORWARD);
          motor4.setSpeed(160);
          motor4.run(BACKWARD);
          Serial.println("direita");
      }
      else if(digitalRead(sensorOtico) == HIGH && 
      digitalRead(sensorOtico2) == LOW){
        motor3.setSpeed(155);
          motor3.run(BACKWARD);
          motor4.setSpeed(160);
          motor4.run(FORWARD);
          Serial.println("esquerda");
      }
      else if(digitalRead(sensorOtico) == LOW && 
      digitalRead(sensorOtico2) == LOW){
         motor3.setSpeed(155);
          motor3.run(BACKWARD);
          motor4.setSpeed(160);
          motor4.run(BACKWARD);
          Serial.println("tras");
      }
    }
}

