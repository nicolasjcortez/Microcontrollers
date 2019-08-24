# importação de bibliotecas
from os import system
from gpiozero import Button, LED, Buzzer, DistanceSensor
from time import sleep
from requests import post, get
from subprocess import Popen
from urllib.request import urlretrieve
from mplayer import Player
from datetime import datetime, timedelta

aplicativo = None
# Mata todos os aplicativos "mplayer" e "arecord"
system("killall mplayer")
system("killall arecord")

# parâmetros iniciais do Telegram
chave = "664078074:AAGPZzx0xuq81fve6u6InSQlRGC0HsaEZwk"
id_da_conversa = "643806337"
endereco_base = "https://api.telegram.org/bot" + chave


# definição de funções
def iniciar_gravacao():
    global aplicativo
    comando = ["arecord", "--duration", "30", "audio.wav"]
    aplicativo = Popen(comando) # executa em plano de fundo
    return
    
def parar_gravacao():
    global aplicativo
    if aplicativo != None:
        aplicativo.terminate()
        aplicativo = None
    system("opusenc audio.wav audio.ogg")
    audio = {"voice":open("audio.ogg","rb")}
    audio_bot(audio)
    return

def audio_bot(audio):
    endereco = endereco_base + "/sendVoice"
    dados = {"chat_id": id_da_conversa}
    resposta = post(endereco, data=dados,files=audio)
    return

def msg_bot(msg):
    endereco = endereco_base + "/sendMessage"
    dados = {"chat_id": id_da_conversa, "text": msg}
    resposta = post(endereco, json=dados)
    return

def tocar_bz():
    bz.on()
    return

def parar_bz():
    global antes
    
    bz.off()
    dt=timedelta(seconds=15)
    agora=datetime.now()
    if antes is None or agora>antes+dt:
        msg_bot("Alguém na porta.")
        system("fswebcam --resolution 640x480 --skip 10 foto_portaria.jpg")
        arquivo = {"photo": open("foto_portaria.jpg", "rb")}
        endereco = endereco_base + "/sendPhoto"
        data = {"chat_id":id_da_conversa}
        resposta = post(endereco, data=data, files=arquivo)
        antes=datetime.now()
    return

def download_arquivo(id_do_arquivo):
    endereco = endereco_base + "/getFile"
    dados = {"file_id": id_do_arquivo}
    resposta = get(endereco, json=dados)
    dicionario = resposta.json()
    final_do_link = dicionario["result"]["file_path"]
    link_do_arquivo = "https://api.telegram.org/file/bot" + chave + "/" + final_do_link
    arquivo_de_destino = "audio_portaria.ogg"
    urlretrieve(link_do_arquivo, arquivo_de_destino)
    return

def aproximou():
    global t_chegada
    t_chegada=datetime.now()
    return

def afastou():
    global t_chegada
    agora = datetime.now()
    dt = timedelta(seconds=10)
    if agora>t_chegada+dt:
        msg_bot("Pessoa saiu.")
    return

# criação de componentes
l1=LED(21)
b1=Button(11)
b2=Button(12)
b3=Button(13)
bz=Buzzer(16)
player = Player()
sensor = DistanceSensor(trigger=17, echo=18)

# loop infinito
b1.when_pressed=tocar_bz
b1.when_released=parar_bz

proximo_id_de_update = 0

b2.when_pressed=l1.off

b3.when_pressed = iniciar_gravacao
b3.when_released = parar_gravacao

sensor.threshold_distance = 0.1
sensor.when_in_range = aproximou
sensor.when_out_of_range = afastou

antes=None

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
        elif "voice" in mensagem:
            id_do_arquivo = mensagem["voice"]["file_id"]
            download_arquivo(id_do_arquivo)
            player.volume = 100
            player.loadfile("audio_portaria.ogg")
        proximo_id_de_update = resultado["update_id"] + 1
    sleep(1)
