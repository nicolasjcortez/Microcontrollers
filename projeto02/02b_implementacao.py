# importação de bibliotecas
from flask import Flask
from threading import Timer
from py_irsend.irsend import *
# criação do servidor
app=Flask(__name__)

# definição de funções das páginas
@app.route("/ligardesligar")
def ligarTv():
    send_once("aquario",["KEY_POWER"])
    return "A TV ligou/desligou!"

@app.route("/aumvolume")
def aumentaVolume():
    send_once("aquario",["KEY_VOLUMEUP"])
    return "O volume aumentou!"

@app.route("/dimvolume")
def dimVolumeTv():
    send_once("aquario",["KEY_VOLUMEDOWN"])
    return "O volume abaixou!"

@app.route("/mute")
def muteTV():
    send_once("aquario",["KEY_MUTE"])
    return "A TV está muda!"

@app.route("/canal/<string:numero_canal>")
def mudacanal(numero_canal):
    send_once("aquario",["KEY_"+numero_canal[0],"KEY_"+numero_canal[1],"KEY_"+numero_canal[2],"KEY_"+numero_canal[3]])
    return "Mudou para o canal "+numero_canal

@app.route("/n/<int:m>")
def timer(m):
    t=Timer(m,ligarTv)
    t.start()
    return "Vai desligar em "+str(m)

app.run(port=5000,debug=False)

# rode o servidor
