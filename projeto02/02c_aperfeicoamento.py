# COMECE COPIANDO AQUI O SEU CÓDIGO DA IMPLEMENTAÇÃO
# importação de bibliotecas
from flask import Flask,render_template,redirect
from threading import Timer
from py_irsend.irsend import *
import json
# criação do servidor
app=Flask(__name__)

# definição de funções das páginas
lista=json.loads(open("arquivo.json").read())

@app.route("/")
def principal():
    return render_template("pagina.html",janks=lista)

@app.route("/ligardesligar")
def ligarTv():
    send_once("aquario",["KEY_POWER"])
    return redirect("/")

@app.route("/aumvolume")
def aumentaVolume():
    send_once("aquario",["KEY_VOLUMEUP"])
    return redirect("/")

@app.route("/dimvolume")
def dimVolumeTv():
    send_once("aquario",["KEY_VOLUMEDOWN"])
    return redirect("/")

@app.route("/mute")
def muteTV():
    send_once("aquario",["KEY_MUTE"])
    return redirect("/")

@app.route("/canal/<string:numero_canal>")
def mudacanal(numero_canal):
    send_once("aquario",["KEY_"+numero_canal[0],"KEY_"+numero_canal[1],"KEY_"+numero_canal[2],"KEY_"+numero_canal[3]])
    return redirect("/")

@app.route("/n/<int:m>")
def timer(m):
    t=Timer(m,ligarTv)
    t.start()
    return redirect("/")

app.run(port=5000,debug=True)

# rode o servidor



# DEPOIS FAÇA OS NOVOS RECURSOS