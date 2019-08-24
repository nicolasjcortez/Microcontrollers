# importação de bibliotecas
from gpiozero import LED,Button
from time import sleep
from Adafruit_CharLCD import *


# definição de funções
def acende_led():
    led.toggle()
def pisca_led():
    global x
    x=x+1
    led2.blink(n=4)
    lcd.clear()
    lcd.message("%d"%x)
def simu_pressed():
    if(botao3.is_pressed and botao4.is_pressed):
        return True

# criação de componentes
x=0
led = LED(21)
led2=LED(22)
led3=LED(23)
led5=LED(25)
botao3=Button(13)
botao4=Button(14)
lcd=Adafruit_CharLCD(2,3,4,5,6,7,16,2)
botao=Button(11)
botao2=Button(12)
botao.when_pressed=acende_led
botao2.when_pressed=pisca_led
led3.blink(on_time=1.0,off_time=3.0)
# loop infinito
while True:
    if simu_pressed()==True:
        led5.on()
    else:
        led5.off()
    sleep(0.2)