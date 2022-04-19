# encoding: utf-8
import wx
import vars
from wx import html2 as webview

class Mapa(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   
        self.browser = wx.html2.WebView.New(self)
        
        t_x, t_y = (640, 480)
        self.browser.SetSize((t_x, t_y))

        # self.statxt_01 = wx.StaticText(self, -1, "ROADS API" , (16, 10), (-1, -1)) 
        # self.statxt_02 = wx.StaticText(self, -1, vars.roads , (16, 30), (-1, -1)) 
        # self.statxt_03 = wx.StaticText(self, -1, "STATIC MAP" , (16, 250), (-1, -1)) 
        # self.statxt_04 = wx.StaticText(self, -1, vars.static_map , (16, 270), (-1, -1)) 
      
    def atualiza_labels(self):
        url = vars.static_map
        # url = ""
        self.browser.LoadURL(url)
        # print vars.static_map

        self.Show()
        
        # self.statxt_02.SetLabel(vars.roads)
        # self.statxt_04.SetLabel(vars.static_map)
      
        # for x in range(len(vars.nomes_das_ruas)):
        #     self.statxt_05 = wx.StaticText(self, -1, vars.nomes_das_ruas[0] , (700, 60), (-1, -1)) 
      
        
      

