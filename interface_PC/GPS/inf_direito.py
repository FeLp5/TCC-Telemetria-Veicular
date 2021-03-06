# encoding: utf-8
import wx, time
from mapa import Mapa
from grafico import Painel_grafico
from mapa_fence import Mapa_fence
from relatorio import Relatorio
import vars


# tema = ("", "", "")


class Painel_Branco(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)  

class Painel_inferior_direito(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)  
        # back_color, for_color, destaque = tema
        self.SetBackgroundColour(vars.cor_fundo)   
        self.sizer = wx.BoxSizer()
        self.SetSizer(self.sizer)
    
        self.panel_um = Painel_Branco(self)
        self.sizer.Add(self.panel_um, 1, wx.EXPAND)

        self.panel_dois = Painel_grafico(self)
        self.sizer.Add(self.panel_dois, 1, wx.EXPAND)

        self.panel_tres = Mapa_fence(self)
        self.sizer.Add(self.panel_tres, 1, wx.EXPAND)
        
        self.panel_quatro = Mapa(self)
        self.sizer.Add(self.panel_quatro, 1, wx.EXPAND)
        
        self.panel_cinco = Relatorio(self)
        self.sizer.Add(self.panel_cinco, 1, wx.EXPAND)

        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        self.panel_cinco.Hide()
        self.SetSize((300, 300))
        self.Centre()

    def show_panel_one(self, event):
        self.panel_um.Show()
        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        self.panel_cinco.Hide()
        self.Layout()
        
    def show_panel_two(self):
        self.panel_um.Hide()
        self.panel_dois.esconde_grafico()# self.painel_dois.Destroy()
        
        if vars.query == 0:
            self.panel_dois.atualiza()
        else:
            self.panel_dois.mostra_grafico()
            self.panel_dois.atual()

        self.panel_dois.Show()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        self.panel_cinco.Hide()
        self.Layout()
        
    def show_panel_tres(self, event):
        self.panel_um.Hide()
        self.panel_dois.Hide()
        self.panel_tres.Show()
        self.panel_tres.atualiza_labels()
        self.panel_quatro.Hide()
        self.panel_cinco.Hide()
        self.Layout()
        
    def show_panel_four(self, event):
        self.panel_um.Hide()
        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Show()
        self.panel_quatro.atualiza_labels()
        self.panel_cinco.Hide()
        self.Layout()
        

    def show_panel_five(self):
        self.panel_um.Hide()
        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        if vars.query == 0:
            self.panel_cinco.atualiza()
        else:
            self.panel_cinco.atual()
        self.panel_cinco.Show()

        self.Layout()