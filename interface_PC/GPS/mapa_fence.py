# encoding: utf-8
import wx, os
import vars
from wx import html2 as webview

class Mapa_fence(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   
        self.html = wx.html2.WebView.New(self)
        
        self.largura = (vars.t_x/7*5)
        self.altura = (vars.t_y/3*2) - 50

        self.html.SetSize((self.largura, self.altura)) # definindo o tamanho
        
      
    def atualiza_labels(self):
        url = vars.static_map_fence

        largura_div = 640
        altura_div = self.altura - 60
        

        if (vars.arquivo_aberto == 1):
            endereco = os.path.dirname(os.path.realpath(__file__)) # pegando o endereço de execução para o html (css e imagens)
            endereco = "file://" + endereco + "/" # completando a string endereço para o html
            
        mapa = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt' lang='pt-br'><head><link rel='stylesheet' href='css/style.css'></head> <body cellpadding=0 cellspacing=0 style='background-color: " + vars.cor_fundo + "'><div  class='barra' style='width: 120px'><p class='painel'>FENCE</p></div><div class='mapa' style='width: " + str(largura_div) + "px; height: " +  str(altura_div)  + "px; border-radius: 5px;  overflow: hidden' ><img src='"+ url + "' style='width: 100%; overflow: hidden'></div></body>"

        self.html.SetPage(mapa, endereco) # montando a página html


        self.Show()

      
        
      

