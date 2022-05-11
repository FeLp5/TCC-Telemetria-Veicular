# encoding: utf-8
import wx
import vars


class Relatorio(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   

        texto_relatorio = ''
        self.statxt_01 = wx.StaticText(self, -1, "Ruas em que o veículo trafegou" , (30, 20), (-1, -1)) 
        self.statxt_05 = wx.StaticText(self, -1, texto_relatorio , (30, 60), (-1, -1)) 
         
    def atualiza_labels(self):

        texto_relatorio = ''

        for x in range(len(vars.nomes_das_ruas)):
            if x == 0:
                texto_relatorio = texto_relatorio + vars.nomes_das_ruas[x] + "\n"
            elif vars.nomes_das_ruas[x] != vars.nomes_das_ruas[x - 1]:
                texto_relatorio = texto_relatorio + vars.nomes_das_ruas[x] + "\n"
            
        self.statxt_05.SetLabel(texto_relatorio) 

      
        
      

