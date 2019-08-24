# importação de bibliotecas
from gpiozero import LED, Button
from time import sleep
from Adafruit_CharLCD import Adafruit_CharLCD
from lirc import init,nextcode



# definição de funções
def apagar_todos():
    for led in leds:
        led.off()

def acende_todos():
    for led in leds:
        led.on()
        
# criação de componentes
leds = [LED(21), LED(22), LED(23), LED(24), LED(25)]
lcd=Adafruit_CharLCD(2,3,4,5,6,7,16,2)
receptor=init("aula",blocking=False)
Botao1=Button(11)
Botao2=Button(12)
lcdselect=0
# loop infinito
Botao1.when_pressed=acende_todos
Botao2.when_pressed=apagar_todos
while True:
    codigo=nextcode()
    if codigo==["KEY_1"]:
        lcdselect=0
    elif codigo==["KEY_2"]:
        lcdselect=1
    elif codigo==["KEY_3"]:
        lcdselect=2
    elif codigo==["KEY_4"]:
        lcdselect=3
    elif codigo==["KEY_5"]:
        lcdselect=4
    elif codigo==["KEY_OK"]:
        leds[lcdselect].toggle()
    elif codigo==["KEY_UP"]:
        if lcdselect<4:
            lcdselect=lcdselect+1
        
    elif codigo==["KEY_DOWN"]:
        if lcdselect>0:
            lcdselect=lcdselect-1
        
    lcd.clear() 
    lcd.message("LED "+str(lcdselect+1) + "\nSelecionado")
    
    sleep(0.2)
    