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
import wx.lib.scrolledpanel as scrolled

global size_window
size_window = ((wx.MAXIMIZE/4, wx.MAXIMIZE))
# global larguraOld
# global largura


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
        
        
        
    # grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
    # valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
    # plt.bar(grupos, valores)
    # 
        # # Eixo_x, Eixo_y
        # plt.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        # 
    # plt.ylabel('Velocidade km/h')
    # plt.xlabel('Locais')


    def init_mapa(self):
        mapa(self)
        # # BASE_DIR = Path(__file__).absolute().parent
        # # file = "teste.html"
        # # final_path = BASE_DIR / file
        # # final_path = "file://" + str(final_path) + "?dados=saoos"
        # # 
        # # print(final_path)
        # # print(largura, " largura")
        # #def __init__(self, parent, content='', *args, **kwargs):

        # self.browser = wx.html2.WebView.New(self)
        # url = "http://maps.google.com"
        # # url = ""
        # self.browser.LoadURL(url)
        # #self.SetSizer() 
        # self.browser.SetSize((size_window[0] / 2, 400))
        # # self.browser.SetPage(content,'')
        
        # #self.Center()
        # #self.CenterOnScreen()
        
        # self.Show()
        
def SizeChange(event):
    # width, height = event.GetSize()
    # print(width, height)
    size_window = event.GetSize()
    largura = size_window[0] / 2
    print(largura , "largura")


#executa os frames
class MyApp(wx.App):
    def OnInit(self):
        global frame
        frame = wx.Frame(None, -1, __file__, (0, 0), size_window)
        frame.panel = MyPanel(frame)
        frame.Show()
        self.SetTopWindow(frame)
        return True
        
