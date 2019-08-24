# importação de bibliotecas
from gpiozero import LED, MotionSensor, Button, DistanceSensor, LightSensor
from threading import Timer
from requests import post, get

# definição de funções
def detecta_movimento ():
    led1.on()
    led2.on()
    global timer
    if timer != None:
        timer.cancel()
        timer = None

def detecta_inercia():
    led1.off()
    
    global timer
    timer = Timer(8, desliga_led2)
    timer.start()
    
def desliga_led2():
    led2.off()
    

def evento_applet():
    dados = {"value1": sensorLuz.value, "value2": sensorDist.distance}
    resultado = post(endereco, json= dados)
    print(resultado.text)
    

# criação de componentes
sensorMovimento = MotionSensor(27)
timer = None
led1 = LED(21)
led2 = LED(22)
botao1 = Button(11)
sensorDist = DistanceSensor (trigger=17, echo=18)
sensorLuz = LightSensor (8)
chave = "hyR4BdbiAm2D4KtIMF7WuCyggZ6yc8666P8zQ1KgqaW"
evento = "Addcolunas"
endereco = "https://maker.ifttt.com/trigger/" + evento + "/with/key/" + chave

sensorMovimento.when_motion = detecta_movimento
sensorMovimento.when_no_motion = detecta_inercia
botao1.when_pressed = evento_applet

# loop infinito
