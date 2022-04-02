# encoding: utf-8
import os
import sys
import wx
import re
import datetime
import wx.dataview
from wx import html2 as webview


import matplotlib

from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx
from matplotlib.figure import Figure

from pathlib import Path

import wx.lib.scrolledpanel as scrolled

#definicoes
cor = wx.NamedColour('black')
corBotoes = ""
b = 1
grafico = "esconder"

rpm = ""
tamanho = 0

#pegando a dimensao da tela
app = wx.App(False) # the wx.App object must be created first.    
  # returns a tuple
size_window = wx.GetDisplaySize()


largura_display = size_window[0]- 6
altura_display = size_window[1] - 6
larg = (largura_display / 10) #definindo um bloco dividindo a tela
alt = (altura_display / 6) #definindo um bloco dividindo a tela

info = ""


dados = []

print ( largura_display , altura_display, larg, alt)

class arquivos(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL | wx.FULL_REPAINT_ON_RESIZE)
        
        self.SetBackgroundColour(BackColour)   
        
        largura_panel = wx.Window.GetSize(self)
        valor = self.GetSize()
        
        count = - 1
        pasta = './'
        botoes = 0
        cont = 0
        
        # from array import array
        self.titulo_bt = [0] * 100
        self.titulo_bt_od = [0] * 100
        self.botao_path = [""] * 100
        #self.titulo_bt = [" "," "]
        
        statxt1 = wx.StaticText(self, -1, "Selecione um arquivo:" , (16, 4), (-1, -1))
        statxt1.SetForegroundColour("black")
        statxt2 = wx.StaticText(self, -1,  " " , (220, 4), (-1, -1))
        statxt2.SetForegroundColour("black")
        self.statxt3 = wx.StaticText(self, -1,  "" , (220, 26), (-1, -1))
        self.statxt3.SetForegroundColour("black")
        self.statxt4 = wx.StaticText(self, -1,  "" , (220, 48), (-1, -1))
        self.statxt4.SetForegroundColour("black")
        self.statxt5 = wx.StaticText(self, -1,  "" , (220, 70), (-1, -1))
        self.statxt5.SetForegroundColour("black")
        
        for diretorio, subpastas, arquivos in os.walk(pasta):
            for arquivo in arquivos:
                # print(os.path.join(diretorio, arquivo))
                
                pos = arquivo.rfind(".")
                ext = ""
                
                x = 0
  
                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
                # pegando apenas a extesnao desejada
                extensao = "tlm"
                if(ext == extensao ):
                 # informacao adicional dos arquivos 

                    datacri = ""
                    datamod = ""
                    infos = str(os.stat(arquivo))
                    
                    try:
                        datam = re.search('st_mtime=(.+?),', infos).group(1)

                    except AttributeError:
                        pass
                        
                    self.titulo_bt[cont] = int(datam)

                    self.botao_path[cont] = str(arquivo)
                    print  self.botao_path[cont]
                    
                    cont = cont + 1
        
        p=0
        temp = 0
        # ordenando os arquivos
        for x in range(cont):
            for w in range(cont):
                # print (self.titulo_bt[w])
                if(self.titulo_bt[p] > self.titulo_bt[w]):
                    temp = self.titulo_bt[p] 
                    self.titulo_bt[p] = self.titulo_bt[w]
                    self.titulo_bt[w] = temp
                    
                    temp_path = self.botao_path[p]
                    self.botao_path[p] = self.botao_path[w]
                    self.botao_path[w] = temp_path
                    
            p = p+1
            # print ("--")        
            
        #desenha os botoes
        count = 2
        for x in range(cont):
            count = count + 22
            # globals()['strg%s' % n] = 'Hello'
            globals()['botao%s' % x]  = wx.Button(self, wx.NewId(), self.convert_data((self.titulo_bt[x])), (10, count), (larg * 1.4, 20))  
            globals()['botao%s' % x].SetBackgroundColour(corBotoes)
            botoes = botoes + 1
            
            # desenha o botao e quebra o laco
            if botoes > 4:
                self.botao_mais = wx.Button(self, wx.NewId(), "Mais antigos", (10, count + 32), (size_window[0]/14, 24))
                self.Bind(wx.EVT_BUTTON, self.abrir_arquivos, self.botao_mais)
                break

        print botoes
       
        # self.botao_mais.Bind(wx.EVT_BUTTON, abrir_arquivos(self, click))
        if botoes:
            self.Bind(wx.EVT_BUTTON, self.abrir_recente1, botao0)
        if botoes > 1:
            self.Bind(wx.EVT_BUTTON, self.abrir_recente2, botao1) 
        if botoes > 2: 
            self.Bind(wx.EVT_BUTTON, self.abrir_recente3, botao2) 
        if botoes > 3:
            self.Bind(wx.EVT_BUTTON, self.abrir_recente4, botao3) 
        if botoes > 4:
            self.Bind(wx.EVT_BUTTON, self.abrir_recente5, botao4) 

            
    def convert_data(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
        return str(data_convertida)
            
    def abrir_recentes(self, nbotao):

        pathname = self.botao_path[nbotao]
        # Proceed loading the file chosen by the user
        print str(pathname)
        
        try:
            with open(pathname, 'r') as file:
                lines = file.readlines()
                print(lines)
                # print str(file)
                self.lines = lines
                info = lines
                
        except IOError:
            wx.LogError("Cannot open file '%s'." % pathname)
            
    def abrir_recente1(self, event):
            self.abrir_recentes(0)
            self.statxt3.SetLabel(self.botao_path[0])
            self.statxt4.SetLabel(self.convert_data(self.titulo_bt[0]))
            self.statxt5.SetLabel(str(self.lines))
            botao0.SetBackgroundColour("blue")
            botao1.SetBackgroundColour(corBotoes)
            botao2.SetBackgroundColour(corBotoes)
            botao3.SetBackgroundColour(corBotoes)
            botao4.SetBackgroundColour(corBotoes)
               
    def abrir_recente2(self, event):
            self.abrir_recentes(1)
            self.statxt3.SetLabel(self.botao_path[1])
            self.statxt4.SetLabel(self.convert_data(self.titulo_bt[1]))
            self.statxt5.SetLabel(str(self.lines))
            botao0.SetBackgroundColour(corBotoes)
            botao1.SetBackgroundColour("blue")
            botao2.SetBackgroundColour(corBotoes)
            botao3.SetBackgroundColour(corBotoes)
            botao4.SetBackgroundColour(corBotoes)
               
    def abrir_recente3(self, event):
            self.abrir_recentes(2)
            self.statxt3.SetLabel(self.botao_path[2])
            self.statxt4.SetLabel(self.convert_data(self.titulo_bt[2]))
            self.statxt5.SetLabel(str(self.lines))
            botao0.SetBackgroundColour(corBotoes)
            botao1.SetBackgroundColour(corBotoes)
            botao2.SetBackgroundColour("blue")
            botao3.SetBackgroundColour(corBotoes)
            botao4.SetBackgroundColour(corBotoes)
            
    def abrir_recente4(self, event):
            self.abrir_recentes(3)
            self.statxt3.SetLabel(self.botao_path[3])
            self.statxt4.SetLabel(self.convert_data(self.titulo_bt[3]))
            self.statxt5.SetLabel(str(self.lines))
            botao0.SetBackgroundColour(corBotoes)
            botao1.SetBackgroundColour(corBotoes)
            botao2.SetBackgroundColour(corBotoes)
            botao3.SetBackgroundColour("blue")
            botao4.SetBackgroundColour(corBotoes)
            
    def abrir_recente5(self, event):
            self.abrir_recentes(4)
            self.statxt3.SetLabel(self.botao_path[4])
            self.statxt4.SetLabel(self.convert_data(self.titulo_bt[4]))
            self.statxt5.SetLabel(str(self.lines))
            show_infos.statxt_info.SetLabel(str(self.lines)) 
            botao0.SetBackgroundColour(corBotoes)
            botao1.SetBackgroundColour(corBotoes)
            botao2.SetBackgroundColour(corBotoes)
            botao3.SetBackgroundColour(corBotoes)
            botao4.SetBackgroundColour("blue")
        
    def abrir_arquivos(self, event):
        with wx.FileDialog(self, "Abrir arquivo de telemetria", wildcard="arquivos tlm  (*.tlm)|*.tlm",
                            style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     # the user nbotaochanged their mind
    
            # Proceed loading the file chosen by the user
            pathname = fileDialog.GetPath()
            print str(pathname)
            
            try:
                with open(pathname, 'r') as file:
                    lines = file.readlines()
                    print(lines)
                    # print str(file)
                    self.lines = lines
                    
            except IOError:
                wx.LogError("Cannot open file '%s'." % newfile)
                
            self.statxt3.SetLabel(pathname)
            self.statxt4.SetLabel("")
            self.statxt5.SetLabel(str(self.lines))
            botao0.SetBackgroundColour(corBotoes)
            botao1.SetBackgroundColour(corBotoes)
            botao2.SetBackgroundColour(corBotoes)
            botao3.SetBackgroundColour(corBotoes)
            botao4.SetBackgroundColour(corBotoes)
            
            self.tratamento(lines)
            
            
    def nulo(self):
        print()
        
    def tratamento(self, lines):
        
        flag = 0
        tamanho = str(len(lines))
        
        for x in range(len(lines)): 
            linha = lines[x]
            if(x == 0):
                velocidade = re.search('v(.+?);', linha).group(1)
                latitude = re.search('lt(.+?);', linha).group(1)
                longitude = re.search('lo(.+?);', linha).group(1)
                rpm = re.search('r(.+?);', linha).group(1)
                combustivel = re.search('c(.+?);', linha).group(1)
                km = re.search('k(.+?);', linha).group(1)
                tempo = re.search('h(.+?);', linha).group(1)
                dtc = re.search('d(.+?);', linha).group(1)
                velmax = velocidade # pegando o valor inicial
                consumo = combustivel # pegando o valor inicial
                km_rodado = km # pegando a distancia percorrida
                hora = tempo[0] + tempo[1]
                min = tempo[2] + tempo[3]
                
            else:
                velocidade = velocidade + "," + re.search('v(.+?);', linha).group(1)
                latitude = latitude + "," + re.search('lt(.+?);', linha).group(1)
                longitude = longitude + "," + re.search('lo(.+?);', linha).group(1)
                rpm = rpm + "," + re.search('r(.+?);', linha).group(1)
                combustivel = combustivel + "," + re.search('c(.+?);', linha).group(1)
                
                km = km + "," + re.search('k(.+?);', linha).group(1)
                tempo = tempo + "," + re.search('h(.+?);', linha).group(1)
                dtc = dtc + "," + re.search('d(.+?);', linha).group(1)
                
                

                #pegando a velocidade máxima
                if(velmax < re.search('v(.+?);', linha).group(1)):
                    velmax = re.search('v(.+?);', linha).group(1)
        
                if(x == (len(lines) - 1)):
                    hora_g = tempo[0] + tempo[1]
                    print hora_g
                    # precisa ser testado
                    if(int(hora) > int(hora_g) and flag == 0 ): #testando por causa da troca de 23h para 0h
                        hora = int(hora) - 24
                        print "entrou" + str(hora)
                        flag = 1
                    
                    
                    
                    #pegando o consumo
                    consumo = float(consumo) - float(re.search('c(.+?);', linha).group(1))
                    
                    #ṕegando a distancia percorrida
                    km_rodado = float(re.search('k(.+?);', linha).group(1))- float(km_rodado)
                
                    #pegando o tempo gasto, precisamos testar
                    tempo_gasto = (re.search('h(.+?);', linha).group(1))
                    hora_g = tempo_gasto[0] + tempo_gasto[1]
                    min_g = tempo_gasto[2] + tempo_gasto[3]
                    
                    hora_g = int(hora_g) - int(hora)
                    if(int(min_g) < int(min)):
                       ming_g = int(min_g) - int(min) + 60
                    else:
                        min_g = int(min_g) -int(min)
                
                
        MyPanel.Refresh
        print "tempo gasto " + str(hora_g) + "h" + str(min_g)
        print "velocidade máxima " + velmax 
        
        print "Consumo"
        print consumo 
        
        print "Distancia percorrida"
        print km_rodado             
        
        print "Tempo Gasto"
        print tempo_gasto         
        


        print "velocidade"
        print velocidade
        print "latitude"
        print latitude
        print "longitude"
        print longitude
        print "rpm"
        print rpm
        print "combustivel"
        print combustivel
        print "km"
        print km
        print "tempo"
        print tempo
        print "dtc"
        print dtc
        

    
class mapa(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL | wx.FULL_REPAINT_ON_RESIZE)
        self.SetBackgroundColour(BackColour)   
        
        largura_panel = wx.Window.GetSize(self)
        valor = self.GetSize()
        
        valor = self.GetSize()
        print(valor)
        v2 = (size_window[0]/8)*6       
        print(v2)
        
        self.browser = wx.html2.WebView.New(self)
        # url = "https://www.google.com/maps/@-23.6022347,-46.5409388,17.05z"
        url = ""
        self.browser.LoadURL(url)

        self.browser.SetSize((larg * 7, alt * 4))

        self.Show()
        
        

class ruas(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL | wx.FULL_REPAINT_ON_RESIZE)
        self.SetBackgroundColour(BackColour)   
        

        
        largura_panel = wx.Window.GetSize(self)
        valor = self.GetSize()
        
        statxt1 = wx.StaticText(self, -1, "O veiculo transitou pelas seguintes vias:" , (16, 4), (-1, -1))
        
        rua = ["Rua da Descida", "Rua da Subida", "Rua da Padaria", "Rua Tres"]
        dist = 0
        for num in range(len(rua)):
            dist = dist + 28
            botao = wx.Button(self, wx.NewId(), rua[num], (10, dist), (larg * 2.7, 24))
            botao.SetBackgroundColour(corBotoes)
        self.Show()
        
class logo(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL | wx.FULL_REPAINT_ON_RESIZE)
        
        self.SetBackgroundColour(BackColour)   
        
        
        
        
        # largura_panel = wx.Window.GetSize(self)
        # valor = self.GetSize()
        imageFile = 'imagens/logo_fatec.png'
        data = open(imageFile, "rb").read()
        png = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(self, -1, png, (10, 10), (png.GetWidth()+30, png.GetHeight()))
        
        imageFile = 'imagens/logo_FDR.jpg'
        data = open(imageFile, "rb").read()
        jpg = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(self, -1, jpg, (10, 120), (png.GetWidth()+30, png.GetHeight()+50))
        

        
        
class show_grafico(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)

        valor = self.GetSize()
        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.canvas.SetSize(((larg * 6)-10, (alt * 2)-10))
        
        grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
        valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]

        self.axes.bar(grupos, valores)
        self.axes.set_autoscale_on
        self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        self.botao_voltar = wx.Button(self, wx.NewId(), "Voltar", (10, 220), (size_window[0]/14, 24))

class show_graficoRPM(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        self.statxt_info = wx.StaticText(self, -1,  "Gráfico de RPMs" , (20, 26), (-1, -1))
        
        valor = self.GetSize()
        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.canvas.SetSize(((larg * 6)-10, (alt * 2)-10))
        
        grupos = ""
        item = "0"
        x = 1
        for x in range(12):
           item = item + ", " + str(x);
           print "itens"
           print item

        
        valores = rpm
        self.axes.bar(grupos, valores)
        self.axes.set_autoscale_on
        self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        self.botao_voltar = wx.Button(self, wx.NewId(), "Voltar", (10, 220), (size_window[0]/14, 24))    
    
class show_infos(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
    
        tab = 200
        linha = 40
        l_espace = 23
        
        self.m_dataViewListCtrl1 = wx.dataview.DataViewListCtrl( self, wx.ID_ANY, wx.DefaultPosition, wx.DefaultSize, 0 )
        
        self.statxt_info = wx.StaticText(self, -1,  "" , (220, 26), (-1, -1))
        self.statxt_info.SetForegroundColour("black")
        
        
        self.statxt_inf0 = wx.StaticText(self, -1, "Informações Coletadas no Trajeto" , (16, 10), (-1, -1))
        
        self.statxt_inf1 = wx.StaticText(self, -1, "Distância percorrida:" , (16, linha), (-1, -1))
        self.statxt_inf_r1 = wx.StaticText(self, -1, "" , (tab, linha), (-1, -1))
        
        self.statxt_inf2 = wx.StaticText(self, -1, "Consumo de Combustível:" , (16, linha + (l_espace*1)), (-1, -1))
        self.statxt_inf_r2 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*1)), (-1, -1))
        
        self.statxt_inf3 = wx.StaticText(self, -1, "Consumo - km/l:" , (16, linha + (l_espace*2)), (-1, -1))
        self.statxt_inf_r3 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*2)), (-1, -1))
       
        self.statxt_inf4 = wx.StaticText(self, -1, "Máxima rotação atingida:" , (16, linha + (l_espace*3)), (-1, -1))
        self.statxt_inf_r4 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*3)), (-1, -1))
        
        self.statxt_inf_r5 = wx.StaticText(self, -1, "Velocidade máxima:" , (16, linha + (l_espace*4)), (-1, -1))
        self.statxt_inf_r5 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*4)), (-1, -1))
        
        self.statxt_inf6 = wx.StaticText(self, -1, "Tempo de viagem:" , (16, linha + (l_espace*5)), (-1, -1))
        self.statxt_inf_r6 = wx.StaticText(self, -1, "" , (tab, linha + (l_espace*5)), (-1, -1))
        
        self.botao_grafico = wx.Button(self, wx.NewId(), "Gráfico de velocidades", (10, 212), (180, 24))
        self.botao_graficoRPM = wx.Button(self, wx.NewId(), "Gráfico de rotações", (200, 212), (180, 24))
            
     
class informacoes(wx.Window):      
    # def __init__(self):
    #     wx.Frame.__init__(self, None, wx.ID_ANY, 'Program')
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (5, 5), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL | wx.FULL_REPAINT_ON_RESIZE | wx.BORDER_SUNKEN)
        
       
        
        self.SetBackgroundColour(BackColour)   
        sizer = wx.BoxSizer()
        self.SetSizer(sizer)
    
        self.panel_one = show_infos(self)
        sizer.Add(self.panel_one, 1, wx.EXPAND)
        self.panel_one.botao_grafico.Bind(wx.EVT_BUTTON, self.show_panel_two)
        self.panel_one.botao_graficoRPM.Bind(wx.EVT_BUTTON, self.show_panel_tres)
        self.panel_two = show_grafico(self)
        sizer.Add(self.panel_two, 1, wx.EXPAND)
        self.panel_two.botao_voltar.Bind(wx.EVT_BUTTON, self.show_panel_one)
        self.panel_tres = show_graficoRPM(self)
        sizer.Add(self.panel_tres, 1, wx.EXPAND)
        self.panel_tres.botao_voltar.Bind(wx.EVT_BUTTON, self.show_panel_one)
        self.SetSize((300, 300))
        self.panel_two.Hide()
        self.panel_tres.Hide()
        self.Centre()
        
    def show_panel_one(self, event):
        self.panel_one.Show()
        self.panel_two.Hide()
        self.panel_tres.Hide()
        self.Layout()
    def show_panel_two(self, event):
        self.panel_one.Hide()
        self.panel_two.Show()
        self.panel_tres.Hide()
        self.Layout()
    def show_panel_tres(self, event):
        self.panel_one.Hide()
        self.panel_two.Hide()
        self.panel_tres.Show()
        self.Layout()
         
        

class MyPanel(wx.Panel):
    
    def __init__(self, parent):
        
        wx.Panel.__init__(self, parent, -1, wx.DefaultPosition, wx.DefaultSize)
        self.parent = parent
        
 
        
        valor = self.GetSize()

        janela1 = arquivos(self, -1, cor)
        janela2 = informacoes(self, -1, cor)
        janela3 = logo(self, -1, cor)

        global janela5
        janela5 = ruas(self, -1, cor)
        global janela6 
        janela6 = mapa(self, -1, cor)
        
        janela1
        janela2
        janela3

        janela5
        janela6
        
        staline = wx.StaticLine(self, -1, (-1, -1), (-1, 1), wx.LI_HORIZONTAL)
        
        vsizer1 = wx.BoxSizer(wx.VERTICAL)
        
        hsizer2 = wx.BoxSizer(wx.HORIZONTAL)
        minhe = 200
        hsizer2.Add(janela1, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(janela2, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.Add(janela3, 3, wx.EXPAND | wx.RIGHT, b)
        hsizer2.SetItemMinSize(janela1, (larg * 3, alt * 2))
        hsizer2.SetItemMinSize(janela2, (larg * 5.5, alt * 2))
        hsizer2.SetItemMinSize(vsizer1, (larg * 1.5, alt * 2))
     

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
        
        self.parent.CreateStatusBar() # A Statusbar in the bottom of the window


    
        # Setting up the menu.
        self.filemenu= wx.Menu()
        menuOpen = self.filemenu.Append(wx.ID_OPEN, "&Abrir"," Abrir um arquivo")
        self.menuAbout= self.filemenu.Append(wx.ID_ABOUT, "&Sobre"," Dados sobre este programa")
        self.menuExit = self.filemenu.Append(wx.ID_EXIT,"S&air"," Fechar o programa")
    
        # Creating the menubar.
        self.menuBar = wx.MenuBar()
        self.menuBar.Append(self.filemenu,"&Arquivo") # Adding the "filemenu" to the MenuBar
        self.parent.SetMenuBar(self.menuBar)  # Adding the MenuBar to the Frame content.
    
        # Events.
        self.Bind(wx.EVT_MENU, OnOpen, menuOpen)
        self.Bind(wx.EVT_MENU, OnExit, self.menuExit)
        self.Bind(wx.EVT_MENU, self.OnAbout, self.menuAbout)

        
        
    def OnAbout(self,e):
        print   "okkkkk"
        # Create a message dialog box
        dlg = wx.MessageDialog(self, " A sample editor \n in wxPython", "About Sample Editor", wx.OK)
        dlg.ShowModal() # Shows it
        dlg.Destroy() # finally destroy it when finished.
    
def OnExit(self,e):
        MyApp.Close(True)  # Close the frame.
    
def OnOpen(self,e):
        """ Open a file"""
        print "ok"
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*", wx.FD_OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), 'r')
            self.control.SetValue(f.read())
            f.close()
        dlg.Destroy()                                                                                                               

    

class MyApp(wx.App):

    def OnInit(self):
        
        global frame
        frame = wx.Frame(None, -1, "Telemetria Veicular FDR", (0, 0), size_window)
        frame.panel = MyPanel(frame)
        frame.Show()
        self.SetTopWindow(frame)
        return True
        

def main():
    app = MyApp()
    app.MainLoop()
    
if __name__ == "__main__" :
    main()
    


