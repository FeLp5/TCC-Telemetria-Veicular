# encoding: utf-8
import wx
from mapa import Mapa
from grafico_a import Painel_grafico_A
from grafico_b import Painel_grafico_B

# variáveis globais
tema = ("", "", "")


class Painel_Branco(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)  

class Painel_inferior_direito(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)  
        back_color, for_color, destaque = tema
        self.SetBackgroundColour(back_color)   
        self.sizer = wx.BoxSizer()
        self.SetSizer(self.sizer)
    
        self.panel_um = Painel_Branco(self)
        self.sizer.Add(self.panel_um, 1, wx.EXPAND)

        self.panel_dois = Painel_grafico_A(self)
        self.sizer.Add(self.panel_dois, 1, wx.EXPAND)

        self.panel_tres = Painel_grafico_B(self)
        self.sizer.Add(self.panel_tres, 1, wx.EXPAND)
        
        self.panel_quatro = Mapa(self)
        self.sizer.Add(self.panel_quatro, 1, wx.EXPAND)

        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        self.SetSize((300, 300))
        self.Centre()

    def show_panel_one(self, event):
        self.panel_um.Show()
        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        # self.panel_um.mostrar_dados()
        # global a
        # print(( a))
        a = 1
        
    def show_panel_two(self, event):
        self.panel_um.Hide()
        self.panel_dois.Show()
        self.panel_tres.Hide()
        self.panel_quatro.Hide()
        self.panel_dois.montar_grafico()
        self.Layout()
    def show_panel_tres(self, event):
        self.panel_um.Hide()
        self.panel_dois.Hide()
        self.panel_tres.Show()
        self.panel_tres.montar_grafico()
        self.panel_quatro.Hide()
        self.Layout()
    def show_panel_four(self, event):
        self.panel_um.Hide()
        self.panel_dois.Hide()
        self.panel_tres.Hide()
        self.panel_quatro.Show()
        self.panel_quatro.atualiza_labels()
        self.Layout()