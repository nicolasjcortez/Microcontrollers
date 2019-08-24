# importação de bibliotecas
from os import system
from mplayer import Player
from gpiozero import LED,Button
from Adafruit_CharLCD import *
from time import sleep
# para de tocar músicas que tenham ficado tocando da vez passada
system("killall mplayer")


# definição de funções
def pausar():
    player.pause()
    checa()
def checa():
    if(player.paused):
        led.blink()
    else:
        led.on()
def proxima_faixa():
    player.pt_step(1)
    
def anterior_faixa():
    if(player.time_pos>2):
        player.time_pos=0
    else:
        player.pt_step(-1)
# criação de componentes
player=Player()
player.loadlist("playlist.txt")
led=LED(21)
led.on()
lcd=Adafruit_CharLCD(2,3,4,5,6,7,16,2)
botaoAnt=Button(11)
botaoProx=Button(13)
botaoPause=Button(12)
botaoPause.when_pressed=pausar
botaoAnt.when_pressed=anterior_faixa
botaoProx.when_pressed=proxima_faixa

# loop infinito
while True:
    lcd.clear()
    if(player.metadata != None):
        lcd.message("%s"%player.metadata["Title"])
    sleep(0.5)
