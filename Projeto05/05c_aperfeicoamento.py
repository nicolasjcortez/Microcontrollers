# importação de bibliotecas
from gpiozero import LED, Button, MotionSensor, DistanceSensor, LightSensor
from threading import Timer
from requests import post, get
from flask import Flask
from datetime import datetime, timedelta
from pymongo import MongoClient, ASCENDING, DESCENDING

# criação do servidor
app = Flask(__name__)

# definição de funções das páginas
@app.route("/<int:led>/<string:estado>")
def altera_led(led, estado):
    if(estado == "on"):
        leds[led-1].on()
    elif(estado == "off"):
        leds[led-1].off()
    salva_banco()
    print("funciona")
    return str(led)+" "+estado

@app.route("/estadoLED")
def define_estado():
    busca = {}
    ordenacao = [ ["hora", DESCENDING] ]
    documento = colecao.find_one(busca, sort = ordenacao)

    listaEstado = []
    for i in range(0,5):
        if (documento["lista_leds"][i] == True):
            listaEstado.append("Luz " + str(i+1) + ": acesa")
        else:
            listaEstado.append("Luz " + str(i+1) + ": apagada")
    
    return "<html><body><ul><li>" + listaEstado[0] + "</li>" + "<li> " + listaEstado[1] + "</li>"+ "<li> " + listaEstado[2] + "</li>"+ "<li> " + listaEstado[3] + "</li>" + "<li> " + listaEstado[4] + "</li>"+ "</ul></body></html>"
    

def salva_banco():
    tempo = datetime.now()
    dados = {"hora": tempo, "lista_leds": [leds[0].is_lit, leds[1].is_lit,
                                           leds[2].is_lit, leds[3].is_lit,
                                           leds[4].is_lit]}
    colecao.insert(dados)

def botao1():
    leds[0].toggle()
    salva_banco()
    
def botao2():
    leds[1].toggle()
    salva_banco()
    
def botao3():
    leds[2].toggle()
    salva_banco()
    
def botao4():
    leds[3].toggle()
    salva_banco()
    
    
def acende_LED5():
    leds[4].on()
    salva_banco()
    
def apaga_LED5():
    leds[4].off()
    salva_banco()

# criação dos componentes
leds = [LED(21), LED(22), LED(23), LED(24), LED(25)]
botoes = [Button(11), Button(12), Button(13), Button(14)]
sensorLuz = LightSensor (8)
cliente = MongoClient("localhost", 27017)
banco = cliente["banco_aula"]
colecao = banco["colecao_aula"]


botoes[0].when_pressed = botao1
botoes[1].when_pressed = botao2
botoes[2].when_pressed = botao3
botoes[3].when_pressed = botao4

sensorLuz.when_dark = acende_LED5
sensorLuz.when_light = apaga_LED5

# rode o servidor
app.run(port=5000, debug = False)

# https://4a277e96.ngrok.io