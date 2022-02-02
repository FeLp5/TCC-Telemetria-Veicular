# Name:      LearnSizers1.py
# Purpose:   A demo app to learn sizers
# Author:    Jean-Michel Fauth, Switzerland
# Copyright: (c) 2005 Jean-Michel Fauth
# Licence:   GPL

# os dev:    windows 98
# py dev:    Python 2.4.1
# wx dev:    wxPython 2.6.1
# Revision:  27 June 2005

# Contributor and minor fixes: Marcelo Barbero, Argentina
# Revision: 03 August 2005

# LearnSizer*.py is a small application aimed to learn the sizers and
# their usage.
# This application is a collection of panels. Each panel contains
# a few widgets laid out with sizers. The panels are numbered
# MyPanel_0 to MyPanel_n. The panel 0 is the simpliest case. The
# order of the panels follows an increasing complexity in the
# layout. It is warmly recommended to start with MyPanel_0.
#
# Do not forget:
# - the wxPython doc
# - the wiki page http://wiki.wxpython.org/index.cgi/UsingSizers
# - dir(<instance of a sizer>) for a list of methods
# - print sizer.method.__doc__
#
# 

import os
import sys
import wx
import re
from wx import html2 as webview

# from numpy import arange, sin, pi
import matplotlib
# matplotlib.use('WXAgg')

from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx
from matplotlib.figure import Figure

# import gi
# gi.require_version('Gtk', '3.0')
# gi.require_version('WebKit2', '4.0')
# 
# from gi.repository import Gtk, WebKit2

from pathlib import Path

# grafico
# grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
# valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
# plt.bar(grupos, valores)
# 
# # Eixo_x, Eixo_y
# plt.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
# 
# plt.ylabel('Velocidade km/h')
# plt.xlabel('Locais')
# plot.show()

# ###################################################################################

size_window = (1024,768)

# class ColWin(wx.Window):
#     """- A wx.Window with a coloured background
#     - pos and size == (-1, -1) since sizers are used"""
#     
#     def __init__(self, parent, id, BackColour):
#         wx.Window.__init__(self, parent, id, (-1, -1), (-1, -1), wx.SIMPLE_BORDER )
#         self.SetBackgroundColour(BackColour)
#         largura_panel = wx.Window.GetSize(self)
#         print(largura_panel)
#         
        
class janelas(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (-1, -1), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL)
        self.SetBackgroundColour(BackColour)   
        
        largura_panel = wx.Window.GetSize(self)
        print(largura_panel)
        
    def arquivos(self):
        count = 0
        pasta = './'
        for diretorio, subpastas, arquivos in os.walk(pasta):
            for arquivo in arquivos:
                # print(os.path.join(diretorio, arquivo))
                infos = str(os.stat(arquivo))
                # tamanho = infos.rfind("st_size", 0, ",")
                # print (tamanho)

                try:
                    found = re.search('st_size=(.+?),', infos).group(1)
                    # print(found)
                    # print(found)
                except AttributeError:
                    pass
                
                count = count + 22
                b1 = wx.Button(self, wx.NewId(), arquivo, (10, count), (160, 20))  
                
    def grafico(self):
        self.figure = Figure()
        
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.canvas.SetSize((size_window[0] / 2,440))
        
        print("window", size_window[0])
        grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
        valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
        # self.SetSizer(self.sizer)
        # self.Fit()
        self.axes.bar(grupos, valores)
        self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        
    def nulo(self):
        print()
        
        
# grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
# valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
# plt.bar(grupos, valores)
# 
    # # Eixo_x, Eixo_y
    # plt.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
    # 
# plt.ylabel('Velocidade km/h')
# plt.xlabel('Locais')

    def mapa(self):
        
        BASE_DIR = Path(__file__).absolute().parent
        file = "teste.html"
        final_path = BASE_DIR / file
        final_path = "file://" + str(final_path) + "?dados=saoos"
        
        print(final_path)
        #def __init__(self, parent, content='', *args, **kwargs):

        
        self.browser = wx.html2.WebView.New(self)
        self.browser.LoadURL(final_path)
        #self.SetSizer() 
        self.browser.SetSize((size_window[0] / 2, 400)) 
        # self.browser.SetPage(content,'')

        #self.Center()
        #self.CenterOnScreen()
        self.Show()
 
        
# janela5 = janelas(null, -1, wx.NamedColour('gray'))

class MyPanel(wx.Panel):
    """- This example is coming from one of my applications
    - in the real app, the white window is a drawing area and
      the five lower windows are StaticTexts with a defined
      font size (this is why I force a fixed hWin1eight)"""

    def __init__(self, parent):
        wx.Panel.__init__(self, parent, -1, wx.DefaultPosition, wx.DefaultSize)
        self.parent = parent

        #a menu for the beauty of the demo
        menu1 = wx.Menu()
        menu1.Append(101, '&aaa')
        menuBar = wx.MenuBar()
        menuBar.Append(menu1, '&File')
        self.parent.SetMenuBar(menuBar)
        
        cor = wx.NamedColour('gray')
        janela1 = janelas(self, -1, cor)
        janela2 = janelas(self, -1, cor)
        janela3 = janelas(self, -1, cor)
        janela4 = janelas(self, -1, cor)
        global janela5
        janela5 = janelas(self, -1, cor)
        global janela6 
        janela6 = janelas(self, -1, cor)
        
        
        janela1.arquivos()
        janela2.nulo()
        janela3.nulo()
        janela4.nulo()
        janela5.grafico()
        janela6.mapa()
        
        #wblack = ColWin(self, -1, 'RED')
        # wred = ColWin(self, -1, cor)
        # wcoral = ColWin(self, -1, cor)
        staline = wx.StaticLine(self, -1, (-1, -1), (-1, 1), wx.LI_HORIZONTAL)
        
        vsizer1 = wx.BoxSizer(wx.VERTICAL)
        b = 0
        
        hsizer2 = wx.BoxSizer(wx.HORIZONTAL)
        b = 0
        minhe = 240
        hsizer2.Add(janela1, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(janela2, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(vsizer1, 3, wx.EXPAND, border=b)
        hsizer2.SetItemMinSize(janela1, (-1, minhe))
        hsizer2.SetItemMinSize(janela2, (-1, minhe))
        hsizer2.SetItemMinSize(vsizer1, (-1, minhe))
    
        vsizer1.Add(janela3, 1, wx.EXPAND | wx.BOTTOM, b)
        vsizer1.Add(janela4, 1, wx.EXPAND)
        
        hsizer3 = wx.BoxSizer(wx.HORIZONTAL)
        hsizer3.Add(janela5, 0, wx.EXPAND | wx.RIGHT, b)
        hsizer3.Add(janela6, 0, wx.EXPAND | wx.RIGHT, b)
        hsizer3.SetItemMinSize(janela6, (-1, 200))
        hsizer3.SetItemMinSize(janela5, (-1, 200))
        

        
        vsizer4 = wx.BoxSizer(wx.VERTICAL)
        b = 0
        vsizer4.Add(hsizer2, 0, wx.EXPAND | wx.ALL, b)
        vsizer4.Add(hsizer3, 1, wx.EXPAND | wx.ALL, b)
        vsizer4.Add(staline, 0, wx.GROW | wx.ALL, 0)

       
        
        self.SetSizerAndFit(vsizer4)
        
        # for item in self.sizerItems: 
        #     print item.GetWidth()
        





# class MyFrame(wx.Frame):
# 
#     def __init__(self, parent, id):
#         s = __file__
#         wx.Frame.__init__(self, parent, id, s, (0, 0), (500, 400))
# 
# 
#         self.panel = eval('MyPanel_%d(self)' % panel)
# 
#         self.Bind(wx.EVT_CLOSE, self.OnCloseWindow)
# 
#     def OnCloseWindow(self, event):
#         self.Destroy()


def SizeChange(event):
    # width, height = event.GetSize()
    # print(width, height)
    size_window = event.GetSize()
    print(size_window[0])
    # janela5.grafico()
    frame.Show()

class MyApp(wx.App):

    def OnInit(self):
        
        global frame
        frame = wx.Frame(None, -1, __file__, (0, 0), size_window)
        print ("size window")
        print(size_window)

        frame.panel = MyPanel(frame)
        self.Bind(wx.EVT_SIZE, SizeChange)

        # frame.Show(True)
        # self.SetTopWindow(frame)
        # return True
        
        frame.Show()
        self.SetTopWindow(frame)
        return True
        

def main():
    app = MyApp()
    app.MainLoop()
    

if __name__ == "__main__" :
    
    main()
