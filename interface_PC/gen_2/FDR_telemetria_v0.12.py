#!/usr/bin/env python
# encoding: utf-8


import wx, os, re, datetime
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas



#pegando a dimensao da tela
app = wx.App(False) # the wx.App object must be created first.    
  # returns a tuple
size_window = wx.GetDisplaySize()
cor_botoes = ""



# janela = 0
# flag = 0
        
class Painel(wx.Frame):

    def __init__(self, parent, id, title, flag ):
        super(Painel, self).__init__(parent, title=title, size=size_window)

        self.InitUI()
        self.Centre()
        

    def InitUI(self):
        
        # global flag 
        # global janela
        

        self.j = 0
        panel = wx.Panel(self)
        
        font = wx.SystemSettings.GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(9)
        
       
        panel.SetBackgroundColour('')
        vbox = wx.BoxSizer(wx.VERTICAL)
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)
        vbox1 = wx.BoxSizer(wx.VERTICAL)
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)

        pn_se = wx.Panel(panel) # painel superior esquerdo
        pn_se.SetBackgroundColour('#ededed')
        
        pn_sm = wx.Panel(panel) #painel superior central
        # pn_sm_g1 = wx.Panel(panel) #painel superior central - gráfico
        # pn_sm_g1.SetBackgroundColour('green')
        # pn_sm_g2 = wx.Panel(panel) #painel superior central
        # 
        pn_sd = wx.Panel(panel) #painel superior direito
        pn_sd.SetBackgroundColour('white')
        
        pn_ie = wx.Panel(panel) #painel inferior esquerdo
        pn_ie.SetBackgroundColour('gray')
        pn_id = wx.Panel(panel) #painel inferior direito
        pn_id.SetBackgroundColour('white')

        vbox.Add(hbox1, 0, wx.ALL | wx.EXPAND, 0)
        # hbox1.Add(pn_se, wx.ID_ANY, wx.EXPAND | wx.ALL, 10)
        # hbox1.Add(pn_sm, wx.ID_ANY, wx.EXPAND | wx.ALL, 10)
        # hbox1.Add(pn_sd, wx.ID_ANY, wx.EXPAND | wx.ALL, 10)
        
        hbox1.Add(pn_se, 0, wx.ALL | wx.EXPAND, 0)
    
        hbox1.Add(pn_sm, 2, wx.ALL | wx.EXPAND, 0)

        hbox1.SetItemMinSize(pn_se, (180, 200))
        # hbox1.Add(pn_sm_g1, 2, wx.ALL | wx.EXPAND, 0)
        # hbox1.Add(pn_sm_g2, 2, wx.ALL | wx.EXPAND, 0)
        
        hbox1.Add(pn_sd, 0, wx.ALL | wx.EXPAND, 0)

        vbox.Add(vbox1, 1, wx.ALL | wx.EXPAND, 0)
        vbox1.Add(hbox2, 1, wx.ALL | wx.EXPAND, 0)
        hbox2.Add(pn_ie, 0, wx.ALL | wx.EXPAND, 0)
        hbox2.SetItemMinSize(pn_ie, (180, 200))
        hbox2.Add(pn_id, 2, wx.ALL | wx.EXPAND, 0)
        panel.SetSizer(vbox)
        
        # print janela
        # pn_sm_g1.Hide()
        # pn_sm_g2.Hide()
        # box = wx.StaticBox(pn_sq, label='Trajetos', pos=(5, 15), size=(300, 200))
    
        # Janela esquerda superior
        self.statxt_01 = wx.StaticText(pn_se, -1, "Trajetos" , (16, 10), (-1, -1))
        
        cont, titulo_bt, bt_caminho = self.arquivos() # pegando os nomes e caminhos dos botoes
        self.bt_camho = bt_caminho
        #desenha os botoes
        count = 20
        botoes = 0

        nome_botao = []
        for x in range(cont):
            count = count + 22
            nome_botao.append(str(self.convert_data((titulo_bt[x]))))
            btn = wx.Button(pn_se, label=self.convert_data((titulo_bt[x])), pos= (10, count), size = (-1,20))
            btn.Bind(wx.EVT_BUTTON, lambda evt, temp=self.convert_data((titulo_bt[x])): OnButton(evt, temp) )

            print nome_botao

            def OnButton(Event, button_label):
                for y in range(len(nome_botao)):
                    if(button_label == nome_botao[y]):
                        print y
                        self.abrir_recentes(y)
                        
#             
            # desenha o botao e quebra o laco
            if (x > 3):
                botao_mais = wx.Button(pn_se, wx.NewId(), "Mais antigos", (10, count + 32),(200,20))
                self.Bind(wx.EVT_BUTTON, self.abrir_arquivos, botao_mais)

                break
                

        # Setting up the menu.
        filemenu= wx.Menu()
        menuOpen = filemenu.Append(wx.ID_OPEN, "&Abrir"," Abrir um arquivo")
        menuAbout= filemenu.Append(wx.ID_ABOUT, "&Sobre"," Dados sobre este programa")
        menuExit = filemenu.Append(wx.ID_EXIT,"S&air"," Fechar o programa")
    
        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&Arquivo") # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar)  # Adding the MenuBar to the Frame content.
    
        # Events.
        self.Bind(wx.EVT_MENU, self.abrir_arquivos, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)


        #logos
        imageFile = 'imagens/logo_fatec.png'
        data = open(imageFile, "rb").read()
        png = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, png, (10, 10), (png.GetWidth()+30, png.GetHeight()))
        
        imageFile = 'imagens/logo_FDR.jpg'
        data = open(imageFile, "rb").read()
        jpg = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, jpg, (10, 120), (png.GetWidth()+30, png.GetHeight()+50))
        
        
        #tela central superior
        
        tab = 200
        linha = 40
        l_espace = 23
        
        def mudar_tela(self, janela):
            
            
            if(janela == 0):
                self.statxt_info = wx.StaticText(pn_sm, -1,  "" , (220, 26), (-1, -1))
                self.statxt_info.SetForegroundColour("black")
                
                
                self.statxt_inf0 = wx.StaticText(pn_sm, -1, "Informações Coletadas no Trajeto" , (16, 10), (-1, -1))
                
                self.statxt_inf1 = wx.StaticText(pn_sm, -1, "Distância percorrida:" , (16, linha), (-1, -1))
                self.statxt_inf_r1 = wx.StaticText(pn_sm, -1, "" , (tab, linha), (-1, -1))
                
                self.statxt_inf2 = wx.StaticText(pn_sm, -1, "Consumo de Combustível:" , (16, linha + (l_espace*1)), (-1, -1))
                self.statxt_inf_r2 = wx.StaticText(pn_sm, -1, "" , (tab, linha + (l_espace*1)), (-1, -1))
                
                self.statxt_inf3 = wx.StaticText(pn_sm, -1, "Consumo - km/l:" , (16, linha + (l_espace*2)), (-1, -1))
                self.statxt_inf_r3 = wx.StaticText(pn_sm, -1, "" , (tab, linha + (l_espace*2)), (-1, -1))
            
                self.statxt_inf4 = wx.StaticText(pn_sm, -1, "Máxima rotação atingida:" , (16, linha + (l_espace*3)), (-1, -1))
                self.statxt_inf_r4 = wx.StaticText(pn_sm, -1, "" , (tab, linha + (l_espace*3)), (-1, -1))
                
                self.statxt_inf5 = wx.StaticText(pn_sm, -1, "Velocidade máxima:" , (16, linha + (l_espace*4)), (-1, -1))
                self.statxt_inf_r5 = wx.StaticText(pn_sm, -1, "" , (tab, linha + (l_espace*4)), (-1, -1))
                
                self.statxt_inf6 = wx.StaticText(pn_sm, -1, "Tempo de viagem:" , (16, linha + (l_espace*5)), (-1, -1))
                self.statxt_inf_r6 = wx.StaticText(pn_sm, -1, "" , (tab, linha + (l_espace*5)), (-1, -1))
                self.botao_grafico = wx.Button(pn_sm, wx.NewId(), "Gráfico de velocidades", (10, 212), (180, 24))
                self.botao_graficoRPM = wx.Button(pn_sm, wx.NewId(), "Gráfico de rotações", (200, 212), (180, 24))
                self.botao_grafico.Bind(wx.EVT_BUTTON, lambda evt, temp=1: abrir_janela(evt, temp))
                self.botao_graficoRPM.Bind(wx.EVT_BUTTON, lambda evt, temp=2: abrir_janela(evt, temp))
                self.statxt_info.Show()
                self.statxt_inf0.Show()
                self.statxt_inf1.Show()
                self.statxt_inf_r1.Show()
                self.statxt_inf2.Show()
                self.statxt_inf_r2.Show()
                self.statxt_inf3.Show()
                self.statxt_inf_r3.Show()
                self.statxt_inf4.Show()
                self.statxt_inf_r4.Show()
                self.statxt_inf5.Show()
                self.statxt_inf_r5.Show()
                self.statxt_inf6.Show()
                self.statxt_inf_r6.Show()
                self.botao_grafico.Show()
                self.botao_graficoRPM.Show()
            else:

                
                self.statxt_info = wx.StaticText(self, -1,  "Gráfico de RPMs" , (20, 26), (-1, -1))
                
                valor = self.GetSize()
                self.figure = Figure()
                self.axes = self.figure.add_subplot(111)
                self.canvas = FigureCanvas(pn_sm, -1, self.figure)
                self.canvas.SetSize((200, 200))
                
                grupos = ""
                item = "0"
                x = 1
                for x in range(12):
                    item = item + ", " + str(x);
                    print "itens"
                    print item
        
                
                valores = (100,100)
                self.axes.bar(grupos, valores)
                self.axes.set_autoscale_on
                self.axes.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
             
                self.botao_voltar = wx.Button(pn_sm, wx.NewId(), "Voltar", (10, 212), (180, 24))
              
                self.botao_voltar.Bind(wx.EVT_BUTTON, lambda evt, temp=0: abrir_janela(evt, temp))
       

        #     
        #     pn_sm.Layout()

                # self.statxt_info.Show()
                # self.statxt_inf0.Show()
                # self.statxt_inf_r1.Show()     
               
        
        
        mudar_tela(self, 0)
        def abrir_janela(evt, jnla):
            # global janela
            janela = jnla
            mudar_tela(self, janela)
            # self.InitUI()
            
    #gráficos
            
            


    def tratamento(self, lines):
        # print "entrou"
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
                rpmmax = rpm # pegando o valor inicial
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
                 
                #pegando a rotação máxima
                rpmmax_int = re.search('r(.+?);', linha).group(1)
                rpmmax = int(rpmmax)
                if rpmmax < int(rpmmax_int):
                    rpmmax = int(rpmmax_int)
                    
                if(x == (len(lines) - 1)):
                    hora_g = tempo[0] + tempo[1]
                    # print hora_g
                    # precisa ser testado
                    if(int(hora) > int(hora_g) and flag == 0 ): #testando por causa da troca de 23h para 0h
                        hora = int(hora) - 24
                        # print "entrou" + str(hora)
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
                
        self.statxt_inf_r1.SetLabel(str(km_rodado) + " Km") 
        self.statxt_inf_r2.SetLabel(str(consumo) + " % total") # precisamos ajustar aqui
        self.statxt_inf_r3.SetLabel(str(consumo * 50) + " km/l") # precisamos ajustar aqui
        self.statxt_inf_r4.SetLabel(str(rpmmax) + " RPM") 
        if rpmmax > 4500:
            self.statxt_inf_r4.SetForegroundColour("red")
        else:
            self.statxt_inf_r4.SetForegroundColour("")
        self.statxt_inf_r5.SetLabel(str(velmax) + " Km/h") 
        self.statxt_inf_r6.SetLabel(str(hora_g) + "h" + str(min_g)) 
        
        # print "tempo gasto " + str(hora_g) + "h" + str(min_g)
        # print "velocidade máxima " + velmax 
        # 
        # print "Consumo"
        # print consumo 
        # 
        # print "Distancia percorrida"
        # print km_rodado             
        # 
        # print "Tempo Gasto"
        # print tempo_gasto         
        # 

        print "velocidade"
        print velocidade
        # print "latitude"
        # print latitude
        # print "longitude"
        # print longitude
        print "rpm"
        print rpm
        # print "combustivel"
        # print combustivel
        # print "km"
        # print km
        # print "tempo"
        # print tempo
        # print "dtc"
        # print dtc
        # Painel.InitUI.valores_graficos(self)
        
        


    def abrir_recentes(self, nbotao):


        pathname = self.bt_camho[nbotao]
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
            wx.LogError("Não é possível abrir o arquivo '%s'." % pathname)

        self.tratamento(lines)
            
    def arquivos(self):
        count = - 1
        pasta = './'
        botoes = 0
        cont = 0
        
        # from array import array
        titulo_bt = [] 
        titulo_bt_od = []
        botao_path = [] 
        #titulo_bt = [" "," "]
        q_arqu = 0
        
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


                    titulo_bt.insert(cont, int(datam))

                    botao_path.insert(cont, str(arquivo))
                    # print  botao_path[cont]
                    
                    cont = cont + 1
        

        p=0
        temp = 0
        # ordenando os arquivos
        for x in range(cont):
            for w in range(cont):
                # print (titulo_bt[w])
                if(titulo_bt[p] > titulo_bt[w]):
                    temp = titulo_bt[p] 
                    titulo_bt[p] = titulo_bt[w]
                    titulo_bt[w] = temp
                    
                    temp_path = botao_path[p]
                    botao_path[p] = botao_path[w]
                    botao_path[w] = temp_path
                    
            p = p+1
            # print ("--")        
            
        return cont, titulo_bt, botao_path
        
    def convert_data(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
        return str(data_convertida)
    
    
    def OnAbout(self,e):
        print   "okkkkk"
        # Create a message dialog box
        dlg = wx.MessageDialog(self, " Informações sobre o uso do veículo ", "FDR Telemetria v0.1", wx.OK)
        dlg.ShowModal() # Shows it
        dlg.Destroy() # finally destroy it when finished.
    
    def OnExit(self,e):
        self.Close(True)  # Close the frame.
    
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
                
            self.tratamento(lines)

def main():

    app = wx.App()
    ex = Painel(None, id =  -1, title='FDR - Telemetria',  flag = wx.MAXIMIZE)
    ex.Show()
    app.MainLoop()
    

if __name__ == '__main__':
    main()