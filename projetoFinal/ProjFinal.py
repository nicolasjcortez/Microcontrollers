from flask import Flask, render_template, request, redirect
from pymongo import MongoClient
from json import load
import os
from serial import Serial
from threading import Thread
import webbrowser
from time import sleep

app = Flask(__name__)

def readArduino():
    global threadrodando
    while True:
        sleep(1)
        while threadrodando == True:    
            try:
                meu_serial1 = Serial("/dev/ttyACM0", baudrate=9600,timeout=0.01)  
                retorno = meu_serial1.readline().decode().strip()
                if retorno != "":
                    webbrowser.open("http://www.ti.com/lit/ds/symlink/sn"+retorno[0:2]+"ls"+retorno[2:4]+".pdf")
            except:
                pass
                
#redefinir banco
threadrodando = False
thread = Thread(target = readArduino)
thread.start()
cliente = MongoClient("localhost", 27017)
cliente.drop_database("banco_chips")
banco = cliente["banco_chips"]

colecao = banco["colecao_chips"]
dados = load(open("chips.json"))
colecao.insert(dados)


@app.route('/exibeChips')
def exibeChips():
    global threadrodando
    vez1 = True
    busca  = {}
    listaChips = list( colecao.find(busca) )
    html = "<html>"
    html +="<head><meta name='viewport' content='width=device-width'></head>"
    html += "<body>"
    html += "<style type='text/css'>"
    html += "form {display: inline;}"
    html += "</style>"
    html += "<form action='/formulario' method ='POST'>"
    html += "<input type = 'submit' value = 'Add chip' /></form>"
    html += "<form action='/sendToArduino' method ='POST'>"
    html += "<input type = 'submit' value = 'Send to Arduino' /></form>"
    html += "<br><br>"
    for chip in listaChips:
        if vez1 == True:
            vez1 = False
            continue
        html += "Chip: " + chip["nome"] + "</br>"
        html += "Numero de pinos: " + str(chip["numero_pinos"]) + "</br>"
        html += "Testes: "
        for teste in chip["testes"]:
            html += teste + " "
        html += "<br>"
        html += "<form action='/editForm' method ='POST'>"
        html += "<input type = 'hidden' name='nome' value ='"+chip["nome"]+"' />"
        html += "<input type = 'submit' value = 'Editar Chip' /></form>"
        html += "<form action='/deleteChip' method ='POST'>"
        html += "<input type = 'hidden' name='nome' value ='"+chip["nome"]+"' />"
        html += "<input type = 'submit' value = 'Deletar Chip' /></form>"
        html += "<br><br>"
    html += "</body></html>"
    threadrodando = True
    return html

@app.route('/formulario', methods = ['POST', 'GET'])
def chipForm():
   global threadrodando
   threadrodando = False
   return render_template('formulario.html')

@app.route('/addChip',methods = ['POST', 'GET'])
def addChip():
   global threadrodando
   threadrodando = False
   if request.method == 'POST':
      result = request.form
      testes = result["testes"].split()
      salvaChip(result["nome"], result["numero_pinos"], testes)
      return redirect('/exibeChips')
    
def salvaChip(nome,nPinos,testes):   
    global threadrodando
    threadrodando = False
    dados = {"nome" : nome,
             "numero_pinos" : nPinos,
             "testes" : testes }
    colecao.insert_one(dados)

@app.route('/editForm', methods = ['POST', 'GET'])
def chipFormEdit():
    global threadrodando
    threadrodando = False
    result = request.form
    busca  = {"nome": result['nome'] }
    chip = colecao.find_one(busca)
    espaco = " "
    testes = espaco.join(chip['testes'])
    return render_template('editForm.html', nome =chip['nome'], nPinos=chip['numero_pinos'], testes=testes)

@app.route('/editChip',methods = ['POST', 'GET'])
def editChip():
   global threadrodando
   threadrodando = False
   if request.method == 'POST':
      result = request.form
      testes = result["testes"].split()
      updateChip(result["nome"], result["numero_pinos"], testes)
      return redirect('/exibeChips')

def updateChip(nome,nPinos,testes):   
    global threadrodando
    threadrodando = False
    dados = {"nome" : nome,
             "numero_pinos" : nPinos,
             "testes" : testes }
    colecao.find_one_and_update({"nome": nome}, 
                                 {"$set": dados })
    
@app.route('/deleteChip',methods = ['POST', 'GET'])
def deleteChip():
    global threadrodando
    threadrodando = False
    result = request.form
    busca  = {"nome": result['nome'] }
    colecao.remove(busca)
    return redirect('/exibeChips')

    
@app.route('/sendToArduino',methods = ['POST', 'GET'])
def sendToArduino():
    try:
        meu_serial = Serial("/dev/ttyACM0", baudrate=9600,timeout=0.01)  
        global threadrodando
        threadrodando = False
        if request.method == 'POST':
           html = "<html><body>"
           html += "<h1>Envio para arduino feito</h1>"
           html += "</body></html>"
           busca  = {}
           #meu_serial = Serial("/dev/ttyACM0", baudrate=9600, timeout=0.01)  
           listaChips = list( colecao.find(busca) )
           meu_serial.write(str(len(listaChips)).encode("UTF-8"))
           waitReceive = True
           while waitReceive:
               retorno = meu_serial.readline().decode().strip()
               if retorno != "":
                   texto_recebido = retorno
                   print(texto_recebido)
                   waitReceive = False
           for chip in listaChips:
               meu_serial.write(str(len(chip["nome"])).encode("UTF-8"))
               waitReceive = True
               while waitReceive:
                   retorno = meu_serial.readline().decode().strip()
                   if retorno != "":
                       texto_recebido = retorno
                       print(texto_recebido)
                       waitReceive = False
               for  i in range(0,len(chip["nome"])):
                   asci = ord(chip["nome"][i])
                   #asci = chip["nome"][i]
                   print(chip["nome"][i] + " " + str(asci))
                   meu_serial.write(str(asci).encode("UTF-8"))
                   waitReceive = True
                   while waitReceive:
                       retorno = meu_serial.readline().decode().strip()
                       if retorno != "":
                           texto_recebido = retorno
                           print(texto_recebido)
                           waitReceive = False
               meu_serial.write(chip["numero_pinos"].encode("UTF-8"))
               waitReceive = True
               while waitReceive:
                   retorno = meu_serial.readline().decode().strip()
                   if retorno != "":
                       texto_recebido = retorno
                       print(texto_recebido)
                       waitReceive = False
               meu_serial.write(str(len(chip["testes"])).encode("UTF-8"))
               waitReceive = True
               while waitReceive:
                   retorno = meu_serial.readline().decode().strip()
                   if retorno != "":
                       texto_recebido = retorno
                       print(texto_recebido)
                       waitReceive = False
               for  i in range(0,len(chip["testes"])):
                   if(len(chip["testes"][i]) == 14):
                       meu_serial.write(("1"+chip["testes"][i][0:7]).encode("UTF-8"))
                       waitReceive = True
                       while waitReceive:
                           retorno = meu_serial.readline().decode().strip()
                           if retorno != "":
                               texto_recebido = retorno
                               print(texto_recebido)
                               waitReceive = False
                       outroLadoChip = "1"+chip["testes"][i][13]+chip["testes"][i][12]+chip["testes"][i][11]+chip["testes"][i][10]+chip["testes"][i][9]+chip["testes"][i][8]+chip["testes"][i][7]
                       meu_serial.write(outroLadoChip.encode("UTF-8"))
                       waitReceive = True
                       while waitReceive:
                           retorno = meu_serial.readline().decode().strip()
                           if retorno != "":
                               texto_recebido = retorno
                               print(texto_recebido)
                               waitReceive = False
                   elif(len(chip["testes"][i]) == 16):
                       meu_serial.write(chip["testes"][i][0:8].encode("UTF-8"))
                       waitReceive = True
                       while waitReceive:
                           retorno = meu_serial.readline().decode().strip()
                           if retorno != "":
                               texto_recebido = retorno
                               print(texto_recebido)
                               waitReceive = False
                       outroLadoChip = chip["testes"][i][15]+chip["testes"][i][14]+chip["testes"][i][13]+chip["testes"][i][12]+chip["testes"][i][11]+chip["testes"][i][10]+chip["testes"][i][9]+chip["testes"][i][8]
                       meu_serial.write(outroLadoChip.encode("UTF-8"))
                       waitReceive = True
                       while waitReceive:
                           retorno = meu_serial.readline().decode().strip()
                           if retorno != "":
                               texto_recebido = retorno
                               print(texto_recebido)
                               waitReceive = False
                               
           return html
    except:
        return redirect("/exibeChips")
        
app.run(port=5000)


