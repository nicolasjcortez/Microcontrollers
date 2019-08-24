# importação de bibliotecas
from os import system
from gpiozero import Button, LED
from Adafruit_CharLCD import Adafruit_CharLCD
from time import sleep
from requests import post

# parâmetros iniciais do Telegram
chave = "664078074:AAGPZzx0xuq81fve6u6InSQlRGC0HsaEZwk"
id_da_conversa = "643806337"
endereco_base = "https://api.telegram.org/bot" + chave


# definição de funções
def gravar_5sec():
    lcd.message("Gravando...")
    system("arecord --duration 5 --format cd audio.wav")
    lcd.clear()
    return

def fotos():
    for i in range(5):
        system("fswebcam --resolution 640x480 --skip 10 foto%d.jpg"%i)
        l1.blink(n=1,on_time=0.3)
        sleep(2)    
    return

def msg_bot():
    endereco = endereco_base + "/sendMessage"
    dados = {"chat_id": id_da_conversa, "text": "Oi!"}
    resposta = post(endereco, json=dados)
    return

# criação de componentes
l1=LED(21)
b1=Button(11)
b2=Button(12)
b3=Button(13)
lcd = Adafruit_CharLCD(2, 3, 4, 5, 6, 7, 16, 2)


b1.when_pressed=gravar_5sec
b2.when_pressed=fotos
b3.when_pressed=msg_bot
