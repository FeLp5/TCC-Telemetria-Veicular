# encoding: utf-8
import wx
import vars


class Mapa(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   

        # self.browser = wx.html2.WebView.New(self)
        # # url = "https://www.google.com/maps/@-23.6022347,-46.5409388,17.05z"
        # url = ""
        # self.browser.LoadURL(url)
        # t_x, t_y = size_window


        self.statxt_01 = wx.StaticText(self, -1, "ROADS API" , (16, 10), (-1, -1)) 
        self.statxt_02 = wx.StaticText(self, -1, vars.roads , (16, 30), (-1, -1)) 
        self.statxt_03 = wx.StaticText(self, -1, "STATIC MAP" , (16, 250), (-1, -1)) 
        self.statxt_04 = wx.StaticText(self, -1, vars.static_map , (16, 270), (-1, -1)) 
      
    def atualiza_labels(self):
        self.statxt_02.SetLabel(vars.roads)
        self.statxt_04.SetLabel(vars.static_map)
      
      
       ##   self.browser.SetSize((t_x/7*6, t_y/3 *2))

       ##   self.Show()
