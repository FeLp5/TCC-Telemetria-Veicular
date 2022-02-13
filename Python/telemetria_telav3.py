import os
import sys
import wx
import re
from wx import html2 as webview


import matplotlib

from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx
from matplotlib.figure import Figure

from pathlib import Path

import wx.lib.scrolledpanel as scrolled

# para abrir a janela de selecao de arquivo


# global size_window
# size_window = ((wx.MAXIMIZE/4, wx.MAXIMIZE))
# print ("tamanho da tela", size_window)

#definicoes
cor = wx.NamedColour('white')
corBotoes = ""
b = 0

#pegando a dimensao da tela
app = wx.App(False) # the wx.App object must be created first.    
  # returns a tuple
size_window = wx.GetDisplaySize()


largura_display = size_window[0]- 6
altura_display = size_window[1] - 6
larg = (largura_display / 10) 
alt = (altura_display / 6)
print ( largura_display , altura_display, larg, alt)
class janelas(wx.Frame):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL)
        self.SetBackgroundColour(BackColour)   
        
        
        
        largura_panel = wx.Window.GetSize(self)
        # print(largura_panel)
        valor = self.GetSize()
        #print(largura_panel)
        
    def arquivos(self):
    # def arquivos(scrolled.ScrolledPanel):
        count = - 1
        pasta = './'
        botoes = 0
        cont = 0
        # from array import array
        titulo_bt = [0] * 100
        titulo_bt_od = [0] * 100
        #titulo_bt = [" "," "]
        
       
        
        statxt1 = wx.StaticText(self, -1, "Escolha um arquivo:" , (-1, -1), (-1, -1))
        statxt1.SetForegroundColour("black")
        
        for diretorio, subpastas, arquivos in os.walk(pasta):
            for arquivo in arquivos:
                # print(os.path.join(diretorio, arquivo))
                
                pos = arquivo.rfind(".")
                ext = ""
                
                x = 0
                #cont = cont + 1
                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
                # pegando apenas a extesnao desejada
                extensao = "tlm"
                if(ext == extensao ):
                 # informacao adicional dos arquivos 
                    #print(arquivos)
                    import datetime
                    datacri = ""
                    datamod = ""
                    infos = str(os.stat(arquivo))
                    
                    try:
                        datam = re.search('st_mtime=(.+?),', infos).group(1)
                        #datac = re.search('st_ctime=(.+?),', infos).group(1)
                        
                        # print int(datam)
                        
                        #datacri = datetime.datetime.fromtimestamp(float(datac)).strftime(' - %H:%M')
                        #'%Y-%m-%d-%H:%M'
                        
                        # print('timestamp', datahora)
                        # print(found)
                        # print(found)
                    except AttributeError:
                        pass
                        
                    # try:
                    #     datac = re.search('st_ctime=(.+?),', infos).group(1)
                    #     datacri = datetime.datetime.fromtimestamp(float(datac)).strftime('%Y-%m-%d-%H:%M')
                    #     # print('timestamp', datahora)
                    #     # print(found)
                    #     # print(found)
                    # except AttributeError:
                    #     pass
                    
                    
                    # print("cont", cont)
                    
                    
                    
                    titulo_bt[cont] = int(datam)
                    # if (titulo_bt[cont] < arquivo):
                    #     titulo_bt[cont + 1] = titulo_bt[cont]
                    #     
                    #     #print("cont", cont)
                    # else:
                    #     #print("titulo", titulo_bt[cont], datamod)
                    #     titulo_bt[cont + 1] = arquivo
                    #     #print("cont", cont)
                    # for w in range(cont):
                    #     print("w", w)
                            
                   
                    
                    cont = cont + 1
                    

        
        p=0
        temp = 0
        # ordebnando os arquivos
        for x in range(cont):
            for w in range(cont):
                # print (titulo_bt[w])
                if(titulo_bt[p] > titulo_bt[w]):
                    temp = titulo_bt[p] 
                    titulo_bt[p] = titulo_bt[w]
                    titulo_bt[w] = temp
            p = p+1
            # print ("--")        
            
        # faz a conversao

        def convert_data(data):
            data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
            return str(data_convertida)

        
        #desenha os botoes
        for x in range(5):
            count = count + 22
            botao = wx.Button(self, wx.NewId(), convert_data((titulo_bt[x])), (10, count), (larg * 2, 20))  
            botao.SetBackgroundColour(corBotoes)
            botoes = botoes + 1
            
            # desenha o botao e quebra o laco
            if botoes > 4:
                self.botao_mais = wx.Button(self, wx.NewId(), "Mais antigos", (10, count + 32), (size_window[0]/14, 24))
                break
        # self.botao_mais.Bind(wx.EVT_BUTTON, abrir_arquivos(self, click))
        self.Bind(wx.EVT_BUTTON, self.abrir_arquivos, self.botao_mais)
            

    
    def abrir_arquivos(self, event):
        with wx.FileDialog(self, "Abrir arquivo de telemetria", wildcard="arquivos tlm  (*.tlm)|*.tlm",
                            style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     # the user changed their mind
    
            # Proceed loading the file chosen by the user
            pathname = fileDialog.GetPath()
            try:
                with open(pathname, 'r') as file:
                    self.doLoadDataOrWhatever(file)
            except IOError:
                wx.LogError("Cannot open file '%s'." % newfile)
                
    
            
    def grafico(self):
        valor = self.GetSize()
        #print("grafico", valor)
        
        
        self.figure = Figure()
        
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.canvas.SetSize(((larg * 6)-10, (alt * 2)-10))
        
        # print("window", size_window[0])
        grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
        valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
        # self.SetSizer(self.sizer)
        # self.Fit()
        self.axes.bar(grupos, valores)
        self.axes.set_autoscale_on
        self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        
    def nulo(self):
        print()
        
    def mapa(self):
        # BASE_DIR = Path(__file__).absolute().parent
        # file = "teste.html"
        # final_path = BASE_DIR / file
        # final_path = "file://" + str(final_path) + "?dados=saoos"
        # 
        # print(final_path)
        # print(largura, " largura")
        #def __init__(self, parent, content='', *args, **kwargs):
        valor = self.GetSize()
        print(valor)
        v2 = (size_window[0]/8)*6       
        print(v2)
        
        self.browser = wx.html2.WebView.New(self)
        #url = "http://maps.google.com"
        url = ""
        self.browser.LoadURL(url)
        #self.SetSizer() 
        self.browser.SetSize((larg * 7, alt * 4))
        # self.browser.SetPage(content,'')
        
        #self.Center()
        #self.CenterOnScreen()
        
        self.Show()
        
        

    def ruas(self):
        
        rua = ["Rua da Descida", "Rua da Subida", "Rua da Padaria", "Rua Tres"]
        dist = 0
        for num in range(len(rua)):
            dist = dist + 28
            botao = wx.Button(self, wx.NewId(), rua[num], (10, dist), (larg * 2.7, 24))
            botao.SetBackgroundColour(corBotoes)
        self.Show()
        
    def logo(self):
        imageFile = 'imagens/logo_fatec.png'
        data = open(imageFile, "rb").read()
        # convert to a data stream
        #stream = cStringIO.StringIO(data)

        png = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(self, -1, png, ( 0, 0), (png.GetWidth(), png.GetHeight()))
        
        
# janela5 = janelas(null, -1, wx.NamedColour('gray'))

class MyPanel(wx.Panel):
    """- This example is coming from one of my applications
    - in the real app, the white window is a drawing area and
      the five lower windows are StaticTexts with a defined
      font size (this is why I force a fixed hWin1eight)"""

    def __init__(self, parent):
        
        wx.Panel.__init__(self, parent, -1, wx.DefaultPosition, wx.DefaultSize)
        self.parent = parent
        
        
        valor = self.GetSize()
        #print("panel ", valor)
        #print(wx.DefaultSize)
        #a menu for the beauty of the demo
        menu1 = wx.Menu()
        menu1.Append(101, '&aaa')
        menuBar = wx.MenuBar()
        menuBar.Append(menu1, '&File')
        self.parent.SetMenuBar(menuBar)
        
       
        janela1 = janelas(self, -1, cor)
        janela2 = janelas(self, -1, cor)
        janela3 = janelas(self, -1, cor)
        janela4 = janelas(self, -1, cor)
        global janela5
        janela5 = janelas(self, -1, cor)
        global janela6 
        janela6 = janelas(self, -1, cor)
        
        
        janela1.arquivos()
        janela2.grafico()
        janela3.logo()
        janela4.nulo()
        janela5.ruas()
        janela6.mapa()
        
        
    
        #wblack = ColWin(self, -1, 'RED')
        # wred = ColWin(self, -1, cor)
        # wcoral = ColWin(self, -1, cor)
        staline = wx.StaticLine(self, -1, (-1, -1), (-1, 1), wx.LI_HORIZONTAL)
        
        vsizer1 = wx.BoxSizer(wx.VERTICAL)
        
        hsizer2 = wx.BoxSizer(wx.HORIZONTAL)
        minhe = 200
        hsizer2.Add(janela1, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(janela2, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(vsizer1, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.SetItemMinSize(janela1, (larg * 3, alt * 2))
        hsizer2.SetItemMinSize(janela2, (larg * 5.5, alt * 2))
        hsizer2.SetItemMinSize(vsizer1, (larg * 1.5, alt * 2))
     
       
    
        vsizer1.Add(janela3, 1, wx.EXPAND | wx.BOTTOM, b)
        vsizer1.Add(janela4, 1,wx.EXPAND)
        
        
        hsizer3 = wx.BoxSizer(wx.HORIZONTAL)
        hsizer3.Add(janela5, 0, wx.EXPAND | wx.RIGHT, b)
        hsizer3.Add(janela6, 0, wx.EXPAND | wx.RIGHT, b)
        hsizer3.SetItemMinSize(janela6, (larg * 7, alt * 4))
        hsizer3.SetItemMinSize(janela5, (larg * 3, alt * 4))
        
        vsizer4 = wx.BoxSizer(wx.VERTICAL)
        vsizer4.Add(hsizer2, 0, wx.EXPAND | wx.ALL, b)
        vsizer4.Add(hsizer3, 1, wx.EXPAND | wx.ALL, b)
        #vsizer4.Add(staline, 0, wx.GROW | wx.ALL, 0)
   
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
    # print(size_window[0])
    # janela5.grafico()
    # frame.Show(False)
    # from time import sleep
    # sleep(1)
    

    largura = size_window[0] / 2
    
    # frame.Show(True)
    #print(largura , "largura")

class MyApp(wx.App):

    def OnInit(self):
        
        global frame
        frame = wx.Frame(None, -1, __file__, (0, 0), size_window)
        # print ("size window")
        # print(size_window)
        
        frame.panel = MyPanel(frame)
        
        # frame.Show(True)
        # self.SetTopWindow(frame)
        # return True
 
        
        frame.Show()
        self.SetTopWindow(frame)
        return True
        

def main():
    app = MyApp()
    app.MainLoop()
    
# app.MainLoop()


if __name__ == "__main__" :
    main()

