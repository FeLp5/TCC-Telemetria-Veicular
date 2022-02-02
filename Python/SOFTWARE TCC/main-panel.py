import wx



class MyPanel(wx.Panel):
    """- This example is coming from one of my applications
    - in the real app, the white window is a drawing area and
      the five lower windows are StaticTexts with a defined
      font size (this is why I force a fixed hWin1eight)"""

    def __init__(self, parent):
        wx.Panel.__init__(self, parent, -1, wx.DefaultPosition, wx.DefaultSize)
        self.parent = parent
        print(wx.DefaultSize)
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