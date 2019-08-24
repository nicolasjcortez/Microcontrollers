# importação de bibliotecas
from os import system
from gpiozero import Button, LED, Buzzer
from time import sleep
from requests import post, get

# Mata todos os aplicativos "mplayer" e "arecord"
system("killall mplayer")
system("killall arecord")

# parâmetros iniciais do Telegram
chave = "664078074:AAGPZzx0xuq81fve6u6InSQlRGC0HsaEZwk"
id_da_conversa = "643806337"
endereco_base = "https://api.telegram.org/bot" + chave


# definição de funções
def msg_bot(msg):
    endereco = endereco_base + "/sendMessage"
    dados = {"chat_id": id_da_conversa, "text": msg}
    resposta = post(endereco, json=dados)
    return

def tocar_bz():
    bz.on()
    return

def parar_bz():
    bz.off()
    msg_bot("Alguém na porta.")
    system("fswebcam --resolution 640x480 --skip 10 foto_portaria.jpg")
    arquivo = {"photo": open("foto_portaria.jpg", "rb")}
    endereco = endereco_base + "/sendPhoto"
    data = {"chat_id":id_da_conversa}
    resposta = post(endereco, data=data, files=arquivo)
    return

# criação de componentes
l1=LED(21)
b1=Button(11)
b2=Button(12)
bz=Buzzer(16)

# loop infinito
b1.when_pressed=tocar_bz
b1.when_released=parar_bz

proximo_id_de_update = 0

b2.when_pressed=l1.off

while True:
    endereco = endereco_base + "/getUpdates"
    dados = {"offset": proximo_id_de_update}
    resposta = get(endereco, json=dados)
    dicionario_da_resposta = resposta.json()
    for resultado in dicionario_da_resposta["result"]:
        mensagem = resultado["message"]
        if "text" in mensagem:
            texto = mensagem["text"]
            if texto == "Abrir":
                l1.on()
            elif texto == "Alarme":
                bz.beep(n=5, on_time=0.3, off_time=0.5)
        proximo_id_de_update = resultado["update_id"] + 1
    sleep(1)

