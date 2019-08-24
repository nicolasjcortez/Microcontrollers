# COMECE COPIANDO AQUI O SEU CÓDIGO DA IMPLEMENTAÇÃO
# DEPOIS FAÇA OS NOVOS RECURSOS

# importação de bibliotecas
from os import system
from mplayer import Player
from gpiozero import LED,Button
from Adafruit_CharLCD import *
from time import sleep
import random
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
        
def acelera_faixa():
    player.speed=2
    
def desacelera_faixa():
    if(player.speed==2):
        player.speed=1
    else:
        player.pt_step(1)
        
# criação de componentes
player=Player()
lista=[]
arq=open("playlist.txt","r")
lista_faixas=arq.readlines()
random.shuffle(lista_faixas)
arq.close()
arq2=open("playlist2.txt",'w')
arq2.writelines(lista_faixas)
arq2.close()
player.loadlist("playlist2.txt")
led=LED(21)
led.on()
lcd=Adafruit_CharLCD(2,3,4,5,6,7,16,2)
botaoAnt=Button(11)
botaoProx=Button(13)
botaoPause=Button(12)
botaoPause.when_pressed=pausar
botaoAnt.when_pressed=anterior_faixa
#botaoProx.when_pressed=proxima_faixa
botaoProx.when_held=acelera_faixa
botaoProx.when_released=desacelera_faixa

# loop infinito
i=0
while True:
    lcd.clear()
    if(player.metadata != None):
        pos=player.time_pos
        tot=player.length
        min1=pos//60
        min2=tot//60
        seg1=pos%60
        seg2=tot%60
        tit=player.metadata["Title"]
        if i>=len(tit)-15:
            i=0
        lcd.message("%s"%tit[i:len(tit)])
        i=i+1
        lcd.message("\n%02d:%02d de %02d:%02d" %(min1,seg1,min2,seg2))
        
    sleep(0.5)

