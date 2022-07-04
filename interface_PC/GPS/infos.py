# encoding: utf-8
import wx, os
import vars
from wx import html2 as html

class Painel_infos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour(vars.cor_fundo)   
        self.html = wx.html2.WebView.New(self) # criando um novo WebView
        self.html.SetSize((vars.t_x/7*5, vars.t_y/3*1)) # definindo o tamanho
        self.flag = 0
        blank =  "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt' lang='pt-br'><head><link rel='stylesheet' href='css/style.css'></head> <body cellpadding=0 cellspacing=0 style='background-color: " + vars.cor_fundo + "'><div><p class='texto_tela'></p></div></body>"
        self.html.SetPage(blank, "") # montando a página html

    def mostrar_dados(self):
 
        nome_arquivo = ''
        cont = len(vars.caminho_bt)
        endereco = os.path.dirname(os.path.realpath(__file__)) # pegando o endereço de execução para o html (css e imagens)
        endereco = "file://" + endereco + "/" # completando a string endereço para o html
        
        if vars.vetor_velocidade:
            for y in range(12):
                nome_arquivo = nome_arquivo + vars.caminho_bt[y - 12] 

        # porcentagem para mostrar o fence ativo
        fence_fora  = 0
        cor_alerta_vel = ''
        cor_alerta_fence = ''
        
        # porcentagem = 0
        for x in range(vars.num_dados):

            if vars.vetor_fence[x] == '0':
                fence_fora += 1
        
        if fence_fora > 1:
            texto_vez = "vezes"
            cor_alerta_fence = 'red'
        else:
            texto_vez = "vez"
        
        def verifica_valor(valor):
            if valor == "0" or valor == "0.0":
                valor ="--"
            return valor
        
        vars.nome_arquivo = nome_arquivo
        # if (vars.arquivo_aberto != 1 and self.flag != 0):
        if nome_arquivo == '':
            infos ="<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt' lang='pt-br'><head><link rel='stylesheet' href='css/style.css'></head> <body cellpadding=0 cellspacing=0 " + vars.cor_fundo + "'></body></html>"

        else:
            infos = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt' lang='pt-br'><head><link rel='stylesheet' href='css/style.css'></head> <body cellpadding=0 cellspacing=0 " + vars.cor_fundo + "'><div class='barra'><p class='painel'>DASHBOARD</p></div><div style='clear: both;'><div><img src='imagens/file.png' height='40' ><br>Arquivo<br></div><p class=text2>" + nome_arquivo.encode(encoding='UTF-8',errors='strict')  + "</p></div><div><div><img src='imagens/velocidade.png' height='40'><br>Velocide Máxima<br></div><p class='text' style='color: " +  cor_alerta_vel + "'>" + str(vars.velmax) + "<small> km/h</small></p></div><div><div><img src='imagens/tempo.png' height='40'><br>Tempo de Viagem<br></div><p class=text>" + str(vars.hora_g) + "h" + str(vars.min_g) +  "</p></div><div><div><img src='imagens/cerca.jpg' height='40'><br>Fora do Fence<br></div><p class=text style='color: " +  cor_alerta_fence + "'>" + str(fence_fora) + " <small>" + texto_vez + "</small> </p></div><div><div><img src='imagens/quant_dados.jpg' height='40'><br>Quantidade de Dados<br></div><p class=text>" + str(vars.num_dados) + "</p></div><div class='desabilitado'><div class ='desabilitado'><img src='imagens/vin.png' height='40' style='opacity: 0.2; color: gray'><br>N° Identificação do Veículo<br></div><p class=text2>" + vars.VIN + "</p></div><div  class='desabilitado'><div  class='desabilitado'><img src='imagens/rpm.png' height='40' style='opacity: 0.2; color: gray'><br>Rotação Máxima<br></div><p class=text>" + verifica_valor(str(vars.rpmmax)) + "<small> rpm</small></p></div><div  class='desabilitado'><div  class='desabilitado'><img src='imagens/distancia.png' height='40' style='opacity: 0.2; color: gray'><br>Distância Percorrida<br></div><p class=text>" + verifica_valor(str(vars.km_rodado)) + "<small> km</small></p></div><div  class='desabilitado'><div  class='desabilitado'><img src='imagens/combustivel.png' height='40' style='opacity: 0.2; color: gray'><br>Combustivel<br></div><p class=text>" + verifica_valor(str(vars.consumo)) + "<small> litros</small></p></div><div  class='desabilitado'><div class='desabilitado'><img src='imagens/dtc.png' height='40' style='opacity: 0.2; color: gray'><br>Falhas no Veículo<br></div><p class=text>" + verifica_valor(vars.dtc) + "</p></div></body>"

        self.html.SetPage(infos, endereco) # montando a página html
            
        self.Layout()