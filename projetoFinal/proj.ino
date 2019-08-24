#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <stdlib.h>

typedef struct
{
  byte io1;
  byte io2;
} Teste;

typedef struct
{
  byte nTestes;
  Teste *testes;
  byte nNome;
  char *nome;
} Chip;

LiquidCrystal lcd(50, 51, 5, 4, 3, 2);
boolean flagChipsSize = true;
boolean flagNameSize = false;
boolean flagName = false;
boolean flagNPinos = false;
boolean flagNTestes = false;
boolean flagTestes = false;
boolean buttonpressed = false;
boolean lcdon = false;
int currentChip;
int currentLetra;
int tamNome;
int nPinos;
int tamTestes;
int nChips;
int currentTeste;
int currentSide;
Chip *chips;
int endereco;
byte temp;

void readSerial(String texto)
{  
  if(flagChipsSize)
  {
    endereco = 0;
    temp = (byte)texto.toInt();
    nChips = temp;
    EEPROM.put(endereco,temp);
    endereco++;
    Serial.println("NChips: " + texto);
    flagChipsSize = false;
    flagNameSize = true;
    currentChip = 0;    
  }
  else if(flagNameSize)
  {
    temp = (byte)texto.toInt();
    tamNome = temp;
    EEPROM.put(endereco,temp);
    endereco++;
    Serial.println("TamNome: " + texto);
    currentLetra = 0;
    flagNameSize = false;
    flagName = true;
  }
  else if(flagName)
  {
    if(currentLetra != tamNome-1)
    {
      temp = (byte)texto.toInt();
      EEPROM.put(endereco,temp);
      endereco++;
      currentLetra ++;
      Serial.println("Letra: " + texto);
    }
    else
    {
      temp = (byte)texto.toInt();
      EEPROM.put(endereco,temp);
      endereco++;
      currentLetra = 0;
      flagName = false;
      flagNPinos = true;
      Serial.println("Letra: " + texto);
    }

  }
  else if(flagNPinos)
  {
    nPinos = texto.toInt();
    Serial.println("NPinos: " + texto);
    flagNTestes = true;
    flagNPinos = false;
  }
  else if(flagNTestes)
  {
    temp = (byte)texto.toInt();
    EEPROM.put(endereco,temp);
    endereco++;
    tamTestes = temp;
    Serial.println("NTestes: " + texto);
    currentTeste = 0;
    currentSide = 1;
    flagNTestes = false;
    flagTestes = true;
  }
  else if(flagTestes)
  {
    byte value = 0;
    for (int i=0; i< sizeof(texto)+2; i++)
    {
      value *= 2;
      if (texto[i] == '1') value++;
    }
    if(currentTeste != tamTestes-1)
    {
      if(currentSide == 1)
      {
        temp = value;
        EEPROM.put(endereco,temp);
        endereco++;
        currentSide = 2;
        Serial.println("Teste1: " + texto + " " +  value);
      }
      else
      {
        temp = value;
        EEPROM.put(endereco,temp);
        endereco++;
        currentSide = 1;
        currentTeste ++;
        Serial.println("Teste2: " + texto);
      }     
    }
    else
    {
      if(currentSide == 1)
      {
        temp = value;
        EEPROM.put(endereco,temp);
        endereco++;
        currentSide = 2;
        Serial.println("Teste1: " + texto );
      }
      else
      {
        temp = value;
        EEPROM.put(endereco,temp);
        endereco++;
        currentSide = 1;
        Serial.println("Teste2: " + texto);
        currentTeste = 0;
        currentChip++;
        flagTestes = false;
        if(currentChip == nChips)
        {
          flagChipsSize = true;
          preencheVetor();
        }
        else
        {
          flagNameSize = true;
        }
      } 
    }
  }
}

void escreverLcd(Chip ch, byte p)
{
  String ret = "";
  byte a;
  lcd.clear();
  if (p != 0)
  {
    for(a = 0;a<(ch.nNome-5);a++)  lcd.print(ch.nome[a]);
    lcd.setCursor(0,2);
    for(a = ch.nNome;ch.nome[a] != ' ';a--);
    for(byte y = a+1;y<ch.nNome;y++)
    {
      ret += ch.nome[y];
      lcd.print(ch.nome[y]);
    }
    Serial.print(ret);
  }
  else
  {
    lcd.print("CHIP MAL");
    lcd.setCursor(0,2);
    lcd.print("INSERIDO");
  }
}

void preencheVetor()
{
  endereco = 0;
  byte n_chip;
  char *nm;
  Teste *t;
  EEPROM.get(endereco,n_chip);
  chips = (Chip*) malloc(sizeof(Chip)*n_chip);
  for(int i = 0;i<n_chip;i++)
  {
    EEPROM.get(++endereco,chips[i].nNome);
    chips[i].nome = (char*)malloc(sizeof(char)*(chips[i].nNome+1));
    for(int j = 0;j<chips[i].nNome;j++)
    {
      EEPROM.get(++endereco,chips[i].nome[j]);
    }
    chips[i].nome[chips[i].nNome] = '\0';
    EEPROM.get(++endereco,chips[i].nTestes);
    chips[i].testes = (Teste*) malloc(sizeof(Teste)*chips[i].nTestes);
    for(int k = 0;k<chips[i].nTestes;k++)
    {
      EEPROM.get(++endereco,chips[i].testes[k].io1);
      EEPROM.get(++endereco,chips[i].testes[k].io2);
    }
  }  
}

void checar()
{
  byte n_chips;
  int y,u;
  EEPROM.get(0,n_chips);
  for(y = 0;y<n_chips;y++)
  {
    //Serial.println(chips[y].nome);
    for(u = 0;u<chips[y].nTestes;u++)
    {
      PORTA = chips[y].testes[u].io1;
      PORTF = chips[y].testes[u].io2;
      delay(10);
      /*Serial.println(chips[y].testes[u].io1,BIN);
      Serial.println(PINC,BIN);
      Serial.println(chips[y].testes[u].io2,BIN);
      Serial.println(PINL,BIN);
      */if(PINC != chips[y].testes[u].io1) break;
      if(PINL != chips[y].testes[u].io2) break;
    }
    if(u == chips[y].nTestes) 
    {
      escreverLcd(chips[y],y);
      break;
    }
  }
  if(y==n_chips)
  {
    lcd.clear();
    lcd.print("CHIP NAO");
    lcd.setCursor(0,2);
    lcd.print("IDENTIFICADO");
  }
}

void setup()
{
  DDRA = B11111111;
  DDRF = B11111111;
  DDRC = B00000000;
  DDRL = B00000000;
  Serial.begin(9600); 
  Serial.setTimeout(10);
  pinMode(7,INPUT);
  lcd.begin(16, 2);
  lcd.print("INSIRA UM");
  lcd.setCursor(0,2);
  lcd.print("CHIP");
  preencheVetor();
}

void loop()
{
  if(digitalRead(7) == 1) buttonpressed = true;
  else 
  {
    lcdon = false;
    buttonpressed = false;
  }
  if (buttonpressed == true && lcdon == false) 
  {
    checar();
    lcdon = true;
  }
  String texto = Serial.readString();
  texto.trim();
  if(texto!="")
  {
    readSerial(texto);
  }
}
