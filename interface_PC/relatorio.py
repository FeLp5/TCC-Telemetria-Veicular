# encoding: utf-8
import wx, os
import vars
from wx import html2 as html

class Relatorio(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   
        self.html = wx.html2.WebView.New(self) # criando um novo WebView
        self.html.SetSize((vars.t_x, vars.t_y)) # definindo o tamanho
         
    def atualiza(self):
        
        
        
        #testando uma maneira de gravar um arquivo e fazer as requisições a partir dele
        # with open("batch/test.txt",'w') as f:
        #     f.write("{\n")
        #     f.write('    batchItems": [\n')
        #     f.write('        {"query": "/reverseGeocode/-23.613197,-46.544090.json"},\n')
        #     f.write('        {"query": "/reverseGeocode/-23.615283,-46.543633.json"},\n')
        #     f.write("    ]\n")
        #     f.write("}")

        endereco = os.path.dirname(os.path.realpath(__file__)) # pegando o endereço de execução para o html (css e imagens)
        endereco = "file://" + endereco + "/" # completando a string endereço para o html
        botao_imprimir = wx.Button(self, wx.NewId(), "  imprimir", (10, 46 ), (160,24))
        
        # pegando as informações e montando as strings que comporão o relatório
        itens = ""
        for x in range(len(vars.nome_rua)):
            itens = itens + "<tr><td>" + vars.nome_rua[x]+ "</td><td>" + vars.cidade[x] + "</td><td>" + vars.estado[x] + "</td><td>" + vars.speed_limit[x] + "</td></tr>"

            
        relatorio = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'><html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'><head><link rel='stylesheet' href='css/style.css'></head><body><h2>Relatório</h2><a href='#' onclick='window. print();'><button>Imprimir</button></a><table colspan = 0 width='800' border = 1 ><tr><td>Arquivo</td><td>" + vars.caminho_bt.encode(encoding='UTF-8',errors='strict') + "</td><td>VIN</td><td>" + vars.VIN + "</td><td></td><td></td><td rowspan=4><img src='imagens/logo_FDR.png' size='300'></td></tr><tr><td>Data</td><td></td><td>Hora</td><td></td><td></td><td></td></tr><tr><td>Tempo de Viagem</td><td></td><td>Consumo de Combustível</td><td></td><td></td><td></td></tr><tr><td>Rotação Máxima do Motor</td><td></td><td>Velocidade Máxima Atingida</td><td></td><td></td><td></td></tr></table><table colspan = 0 border = 0 width='800'><tr><td>Ruas trafegadas</td><td>Hora</td><td>Velocidade</td><td>RPM</td><td>Combustível</td><td>Hodômetro</td></tr>"  + itens.encode(encoding='UTF-8',errors='strict') + "</table></body>"
        
        self.html.SetPage(relatorio, endereco) # montando a página html
        
        botao_imprimir.Bind(wx.EVT_BUTTON, self.imprimir) # listener do botão imprimir
        
    def imprimir(self, event):
        self.html.Print()
      
       