# encoding: utf-8
import wx, os
import vars
from wx import html2 as html

class Painel_infos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   
        self.html = wx.html2.WebView.New(self) # criando um novo WebView
        self.html.SetSize((vars.t_x/7*5, vars.t_y/3*1)) # definindo o tamanho
        self.flag = 0

    def mostrar_dados(self):
        
        tab = 200
        linha = 60
        l_espace = 22
        
        if (vars.arquivo_aberto == 1 and self.flag == 0):
            endereco = os.path.dirname(os.path.realpath(__file__)) # pegando o endereço de execução para o html (css e imagens)
            endereco = "file://" + endereco + "/" # completando a string endereço para o html
            
        infos = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt' lang='pt-br'><head><link rel='stylesheet' href='css/style.css'></head> <body cellpadding=0 cellspacing=0><div style='width: 29%; height: 60px'><div><img src='imagens/file.png' height='30' ><br>Arquivo<br></div><p class=text2>220324.tlm</p></div><div style='width: 69%; height: 60px'><div><img src='imagens/vin.png' height='30'><br>Número de Identificação do Veículo<br></div><p class=text2>9BWXXXXXXXXXX8901</p></div><div><img src='imagens/velocidade.png' height='40'><br>Velocide Máxima Atingida<br><p class=text>100 <small>km/h</small></p></div><div><img src='imagens/rpm.png' height='40'><br>Rotação Máxima Atingida<br><p class=text>4970 <small>rpm</small></p></div><div><img src='imagens/distancia.png' height='40'><br>Distância Percorrida<br><p class=text>22,3 <small>km</small></p></div><div><img src='imagens/combustivel.png' height='40'><br>Combustivel<br><p class=text>12,2 <small>litros</small></p></div><div><img src='imagens/tempo.png' height='40'><br>Tempo de Viagem<br><p class=text>2h23</p></div></div> <div><img src='imagens/dtc.png' height='40'><br>Falhas no Veículo<br><p class=text>OK</p></div></div></body>"

        
        self.html.SetPage(infos, endereco) # montando a página html
        
        print infos
            
        #     
        #     
        #     self.statxt_nada_carregado.Hide()
        #     self.statxt_arquivo = wx.StaticText(self, -1,  "", (16, 6), (-1, -1))
    
   #    #       self.statxt_info = wx.StaticText(self, -1,  "" , (220, 26), (-1, -1))
        #     self.statxt_info.SetForegroundColour("black")
        #     
        #     self.statxt_inf0 = wx.StaticText(self, -1, "Informações Coletadas no Trajeto" , (16, 30), (-1, -1))
        #     
        #     self.statxt_inf1 = wx.StaticText(self, -1, "Distância percorrida:" , (16, linha), (-1, -1))
        #     self.statxt_inf_r1 = wx.StaticText(self, -1, "" , (tab, linha), (-1, -1))
        #     
        #     self.statxt_inf2 = wx.StaticText(self, -1, "Consumo de Combustível:" , (16, linha + (l_espace*1)), (-1, -1))
        #     self.statxt_inf_r2 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*1)), (-1, -1))
        #     
        #     self.statxt_inf3 = wx.StaticText(self, -1, "Consumo - km/l:" , (16, linha + (l_espace*2)), (-1, -1))
        #     self.statxt_inf_r3 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*2)), (-1, -1))
        # 
        #     self.statxt_inf4 = wx.StaticText(self, -1, "Máxima rotação atingida:" , (16, linha + (l_espace*3)), (-1, -1))
        #     self.statxt_inf_r4 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*3)), (-1, -1))
        #     
        #     self.statxt_inf_r5 = wx.StaticText(self, -1, "Velocidade máxima:" , (16, linha + (l_espace*4)), (-1, -1))
        #     self.statxt_inf_r5 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*4)), (-1, -1))
        #     
        #     self.statxt_inf6 = wx.StaticText(self, -1, "Tempo de viagem:" , (16, linha + (l_espace*5)), (-1, -1))
        #     self.statxt_inf_r6 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*5)), (-1, -1))
        #     
        #     self.flag = 1
        #     
        # 
        # self.statxt_inf_r1.SetLabel(str(vars.km_rodado) + " Km") 
        # self.statxt_inf_r2.SetLabel(str(vars.consumo) + " % total") # precisamos ajustar aqui
        # self.statxt_inf_r3.SetLabel(str(vars.consumo * 50) + " km/l") # precisamos ajustar aqui
        # self.statxt_inf_r4.SetLabel(str(vars.rpmmax) + " RPM") 
        # 
        # if vars.rpmmax > 4500:
        #     self.statxt_inf_r4.SetForegroundColour("red")
        # else:
        #     self.statxt_inf_r4.SetForegroundColour("")
        #     
        # self.statxt_inf_r5.SetLabel(str(vars.velmax) + " Km/h") 
        # self.statxt_inf_r6.SetLabel(str(vars.hora_g) + "h" + str(vars.min_g))
        # self.statxt_arquivo.SetLabel("Arquivo " + vars.caminho_bt + "     --- VIN: " + vars.VIN)
        # 
        self.Layout()