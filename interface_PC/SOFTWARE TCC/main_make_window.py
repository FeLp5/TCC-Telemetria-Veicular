import grafico from .components.main_grafico
import mapa from .components.main_mapa


class janelas(wx.Window):
    """- A wx.Window with a coloured background
    - pos and size == (-1, -1) since sizers are used"""
    
    def __init__(self, parent, id, BackColour):
        wx.Window.__init__(self, parent, id, (-1, -1), (-1, -1), wx.BORDER_DEFAULT | wx.VSCROLL)
        self.SetBackgroundColour(BackColour)   
        
        largura_panel = wx.Window.GetSize(self)
        # print(largura_panel)
        
    def arquivos(self):
    # def arquivos(scrolled.ScrolledPanel):
        count = 0
        pasta = './'
        
        
        # def __init__(self, parent):
    
   #    #       # self.Bind(wx.EVT_SIZE, SizeChange)
        #     scrolled.ScrolledPanel.__init__(self, parent, -1)
    
   #    #       vbox = wx.BoxSizer(wx.VERTICAL)
            # botao = wx.Button()
     
            # desc = wx.StaticText(self, -1, text)
    
       #    #   desc.SetForegroundColour("Blue")

        for diretorio, subpastas, arquivos in os.walk(pasta):
            for arquivo in arquivos:
                # print(os.path.join(diretorio, arquivo))

                pos = arquivo.rfind(".")
                ext = ""
                # ano = ""
                # mes = ""
                # dia = ""
                # hora = ""
                # min = ""
                x = 0
                cont = 0
                cont = cont + 1
                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
                # print (ext)    
                
                
                # pegando apenas a extesnao desejada
                extensao = "tlm"
                if(ext == extensao ):
                    # #pegando o ano
                    # for x in range(len(arquivo)): 
                    #     if( x < 4):  
                    #         ano = ano + arquivo[x]
                    # print (ano)   
                    # #pegando o mes
                    # for x in range(len(arquivo)): 
                    #     if( x > 4 and x < 7):  
                    #         mes = mes + arquivo[x]
                    # print (mes)   
                    # #pegando o dia
                    # for x in range(len(arquivo)): 
                    #     if( x > 7 and x < 10):  
                    #         dia = dia + arquivo[x]
                    # print (dia) 
                    # #pegando o hora
                    # for x in range(len(arquivo)): 
                    #     if( x > 10 and x < 13):  
                    #         hora = hora + arquivo[x]
                    # print (hora)  
                    # #pegando os minutos
                    # for x in range(len(arquivo)): 
                    #     if( x > 10 and x < 13):  
                    #         min = min + arquivo[x]
                    # print (min)      
                    
                    # informacao adicional dos arquivos 
                    
                    import datetime
                    datacri = ""
                    datamod = ""
                    infos = str(os.stat(arquivo))
                    
                    
                        # tamanho = infos.rfind("st_size", 0, ",")
                        # print (tamanho)
                        
                    # print(infos)
                    try:
                        datam = re.search('st_mtime=(.+?),', infos).group(1)
                        #datac = re.search('st_ctime=(.+?),', infos).group(1)
                        datamod = datetime.datetime.fromtimestamp(float(datam)).strftime('%d/%m/%Y - %H:%M')
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
                    
                    arquivo =  str(datamod)
                    
                
            
            
                # abaixo se for necessario informacao adicional dos arquivos 
                # try:
                #     found = re.search('st_size=(.+?),', infos).group(1)
                #     # print(found)
                #     # print(found)
                # except AttributeError:
                #     pass
                
                    count = count + 22
                    botao = wx.Button(self, wx.NewId(), arquivo, (10, count), (size_window[0]/14, 20))  
                    #     vbox.Add(botao[cont], 0, wx.ALIGN_LEFT | wx.ALL, 5)
                    #     # vbox.Add(wx.StaticLine(self, -1, size=(1024, -1)), 0, wx.ALL, 5)
                    #     vbox.Add((20, 20))
        
           #        #   self.SetSizer(vbox)
                    # self.SetupScrolling()
                
   #              
    def init_grafico(self):
        grafico()

        # self.figure = Figure()
        # self.axes = self.figure.add_subplot(111)
        # self.canvas = FigureCanvas(self, -1, self.figure)
        # self.canvas.SetSize((size_window[0] / 4,440))
        
        # # print("window", size_window[0])
        # grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
        # valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
        # # self.SetSizer(self.sizer)
        # # self.Fit()
        # self.axes.bar(grupos, valores)
        # self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        
    def nulo(self):
        print()

    def init_mapa(self):
        mapa(self)
    