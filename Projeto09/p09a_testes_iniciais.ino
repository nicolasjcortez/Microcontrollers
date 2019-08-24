#include <AFMotor.h>

int sensorOtico = A11;
int sensorOtico2 = A12;
bool isLow;
AF_DCMotor motor3(3);
int x = 0;
bool is800 = false;
 
void setup() {
    pinMode(sensorOtico, INPUT);
    pinMode(sensorOtico2, INPUT);
    Serial.begin(9600);
    isLow = digitalRead(sensorOtico);
    Serial1.begin(9600);
    Serial1.setTimeout(10);
}

void loop() {
    if( digitalRead(sensorOtico) == LOW && isLow == HIGH ||
        digitalRead(sensorOtico) == HIGH && isLow == LOW){
      Serial.println("tocar");
      Serial1.println("tocar");
    }
    isLow = digitalRead(sensorOtico);

    if(analogRead(sensorOtico2) >= 800 && is800 == false)
    {
      x++;
      Serial1.println("contagem " + String(x));
      is800 = true;
    }
    else if(analogRead(sensorOtico2) < 800){
      is800 = false;
    }
    
    String comando = Serial1.readString();
    comando.trim();
    if(comando != "")
    {      
      if(comando.startsWith("frente")){
        int spd = comando.substring(7).toInt();
        motor3.setSpeed(spd);
        motor3.run(FORWARD);
      }
      else if(comando.startsWith("tras")){
        int spd = comando.substring(5).toInt();
        motor3.setSpeed(spd);
        motor3.run(BACKWARD);
      }
    }
    
}

