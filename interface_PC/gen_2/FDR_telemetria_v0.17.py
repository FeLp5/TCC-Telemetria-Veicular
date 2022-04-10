#!/usr/bin/env python
# encoding: utf-8


import wx, os, re, datetime
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from wx import html2 as webview


#pegando a dimensao da tela
app = wx.App(False) # the wx.App object must be created first.    
  # returns a tuple
size_window = wx.GetDisplaySize()
cor_botoes = ""

# variáveis globais
a = 0
km_rodado = 0
consumo = 0
rpmmax = 0 
velmax = 0 
hora_g = 0 
min_g = 0
rpm = 0
flag = 0
velocidade = 0
iniciado = 0
mudanca = 0
flag_iniciar = 0
t_x, t_y = size_window

apy_key="XXXXXXXXXXX"
class Painel_arquivos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        # Janela esquerda superior
    # def montar_botoes(self, cont, titulo_bt, botao_path, diretorio):
    
        self.mostrar_botoes(0, "", "", "")
        
    def mostrar_botoes(self, cont, titulo_bt, bt_caminho, diretorio):
        self.bt_camho = bt_caminho
        count = 20
        botoes = 0
        x = 0
        
        # self.panel_s1 = wx.Panel(self) # mostrando pasta indefinida
        # self.panel_s2 = wx.Panel(self) # tela com botões encontrados
        # self.panel_s2.SetBackgroundColour("")
        # self.frame = wx.BoxSizer()
        # 
        # self.SetSizer(self.frame)
        # self.panel_s1.SetSize((300, 300))
        # self.panel_s2.SetSize((300, 300))

        # self.Centre()

        # self.frame.Add(self.panel_s1, 1, wx.EXPAND)
        # self.frame.Add(self.panel_s2, 1, wx.EXPAND)
        # self.Layout()

        

        global flag_iniciar
        
        if(flag_iniciar == 0):
            titulo_bt = [0] * 5
            self.statxt_02 = wx.StaticText(self, -1, "Trajetos Recentes" , (16, 10), (-1, -1))
            self.botao0 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[0])), (10, 42), size = (-1,20)) 
            self.botao1 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[1])), (10, 64), size = (-1,20))
            self.botao2 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[2])), (10, 86), size = (-1,20))
            self.botao3 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[3])), (10, 108), size = (-1,20))
            self.botao4 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[4])), (10, 130), size = (-1,20))
            self.botao_mais = wx.Button(self, wx.NewId(), "Mais antigos", (10, 162),(-1,20))
            self.Bind(wx.EVT_BUTTON, self.abrir_arquivos, self.botao_mais)
  
            self.botao0.Hide()
            self.botao1.Hide()
            self.botao2.Hide()
            self.botao3.Hide()
            self.botao4.Hide()
            self.statxt_02.Hide()
            self.botao_mais.Hide()
            
            self.statxt_01 = wx.StaticText(self, -1, "A pasta não contém \nnenhum arquivo de \ntelemetria" , (16, 100), (-1, -1)) 
            self.botao_sdcard = wx.Button(self, wx.NewId(), "Abrir SD Card", (10, count + 32),(-1,20))
            self.Layout()
            flag_iniciar = 1
        else:
            self.botao0.Hide()
            self.botao1.Hide()
            self.botao2.Hide()
            self.botao3.Hide()
            self.botao4.Hide()
            self.statxt_02.Show()
            
            self.botao0.SetLabel(self.convert_data((titulo_bt[0])))
            self.botao1.SetLabel(self.convert_data((titulo_bt[1])))
            self.botao2.SetLabel(self.convert_data((titulo_bt[2])))
            self.botao3.SetLabel(self.convert_data((titulo_bt[3])))
            self.botao4.SetLabel(self.convert_data((titulo_bt[4])))
            if cont:
                self.botao0.Show()
            if cont > 1:    
                self.botao1.Show()
            if cont > 2: 
                self.botao2.Show()
            if cont > 3:
                self.botao3.Show()
            if cont > 4:
                self.botao4.Show()
            if (cont > 5):
                self.botao_mais.Show()

            
            self.statxt_01.Hide()
            self.botao_sdcard.Hide()
                

            self.Layout()
            


                
    def abrir_recentes(self, nbotao):
        pathname_recente = self.bt_camho[nbotao]
        for x in range(len(self.bt_camho)):
        # Proceed loading the file chosen by the user
        # print(( str(pathname)))
            print("Caminho -> " + str(x) + " " + self.bt_camho[x])
        try:
            with open(pathname_recente , 'r') as file:
                print(pathname_recente)
                lines = file.readlines()
                # print(((lines)))
                # print(( str(file)))
                self.lines = lines
                info = lines
                
        except IOError:
            wx.LogError("Não é possível abrir o arquivo '%s'." % pathname)

        self.tratamento(lines)
        
        
    def arquivos(self, pasta):
        count = - 1
        # global pasta
        botoes = 0
        cont = 0

        # from array import array
        titulo_bt = [] 
        titulo_bt_od = []
        botao_path = [] 
        #titulo_bt = [" "," "]
        q_arqu = 0
        diretorio = ""
        for diretorio, subpastas, arquivos in os.walk(pasta):
        
            print("DIRETORIO", diretorio)
            print("SUBPASTAS", pasta)
                
            for arquivo in arquivos:
                # print(((os.path.join(diretorio, arquivo))))

                pos = arquivo.rfind(".")
                ext = ""
                
                x = 0

                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
                        print(ext)
                # pegando apenas a extesnao desejada
                caminho = diretorio + "/" + arquivo
                print("CAMINHO" + caminho)
                extensao = "tlm"
                if(ext == extensao ):
                # informacao adicional dos arquivos 

                    datacri = ""
                    datamod = ""
                    
                    infos = str(os.stat(caminho))
                    
                    try:
                        datam = re.search('st_mtime=(.+?),', infos).group(1)
                        
                    except AttributeError:
                        pass


                    titulo_bt.insert(cont, int(datam))

                    botao_path.insert(cont, str(caminho))
                    # print((  botao_path[cont]))
                    
                    cont = cont + 1
        

        p=0
        temp = 0
        # ordenando os arquivos
        for x in range(cont):
            for w in range(cont):
                # print(( (titulo_bt[w])))
                if(titulo_bt[p] > titulo_bt[w]):
                    temp = titulo_bt[p] 
                    titulo_bt[p] = titulo_bt[w]
                    titulo_bt[w] = temp
                    
                    temp_path = botao_path[p]
                    botao_path[p] = botao_path[w]
                    botao_path[w] = temp_path
                    print( titulo_bt[w])
            p = p+1
            # print(( ("--")        ))
        
        # return cont, titulo_bt, botao_path, diretorio

        self.mostrar_botoes(cont, titulo_bt, botao_path, diretorio)
        
        
    def convert_data(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
        return str(data_convertida)
    
    
    def OnAbout(self,e):
        print((   "okkkkk"))
        # Create a message dialog box
        dlg = wx.MessageDialog(self, " Informações sobre o uso do veículo ", "FDR Telemetria v0.1", wx.OK)
        dlg.ShowModal() # Shows it
        dlg.Destroy() # finally destroy it when finished.
    
    def abrir_arquivos(self, event):
        with wx.FileDialog(self, "Abrir arquivo de telemetria", wildcard="arquivos tlm  (*.tlm)|*.tlm",
                            style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     # the user nbotaochanged their mind
    
            # Proceed loading the file chosen by the user
            pathname = fileDialog.GetPath()
            global iniciado
            
            
            # print(( str(pathname)))
            
            try:
                with open(pathname, 'r') as file:
                    lines = file.readlines()
                    # print(((lines)))
                    # print(( str(file)))
                    self.lines = lines
                    
            except IOError:
                wx.LogError("Cannot open file '%s'." % newfile)
            
            
            
            
            self.tratamento(lines)
            # self.arquivos(pathname)


                
                
    def abrir_diretorio(self, event):
        with wx.DirDialog (None, "Choose input directory", "",
                    wx.DD_DEFAULT_STYLE | wx.DD_DIR_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     # the user nbotaochanged their mind
    
            # Proceed loading the file chosen by the user
            pathname = fileDialog.GetPath()

            print(pathname)
            # if iniciado == 1:
            #     self.tratamento(lines)
            # else:
            #     iniciado = 1

            self.arquivos(pathname)


    def tratamento(self, lines):
        # print(( "entrou"))

        cordenadas = ""
        ## testando funcao latlong
        lat = self.latlong(-2342.09544)
        long = self.latlong(-04629.71966)
        print(("LAT " , lat, "LONG " , long))
        ##

        tamanho = str(len(lines))
        # print(( lines)
        global km_rodado, consumo, rpmmax, velmax, hora_g, min_g, rpm, velocidade
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
                
                
                ## montando as strings para as apis
                ## roads retornará a velocidade da via
                ## maps_api - irá desenhar o mapa e a rota
                ## geocode - trará os nomes das ruas 
                
                if x == len(lines) - 1 : 
                    t_x, t_y = size_window
                    t_x = t_x/7*6
                    t_y = t_y/3*2
                    tamanho_janela = str(t_x) + "&" + str(t_y)
                    roads = "https://roads.googleapis.com/v1/speedLimits?path=" + cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "&key=" + apy_key
                    static_map = "https://maps.googleapis.com/maps/api/staticmap?size=" + tamanho_janela + "&center=" + cordenada_central + "&path=color:0x0000ff|weight:5|" + cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "&key=" + apy_key
                    # roads = "https://roads.googleapis.com/v1/speedLimits?path=" + api1 + "&key=" + apy_key
                    print(( "ROADS API -> ", roads))
                    print(( "STATIC_MAP API ->" , static_map))
                elif  x == round(len(lines) / 2, 0) + 1 : 
                    print(( "ok"))
                    print(( x))
                    cordenada_central = re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) # pggando a coordenada central
                    cordenadas = cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|" # inserindo também na string aqui
                else:
                    cordenadas = cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|"
                    

                ## ------------------------------------------------
                
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
                    # print(( hora_g))
                    # precisa ser testado
                    if(int(hora) > int(hora_g) and flag == 0 ): #testando por causa da troca de 23h para 0h
                        hora = int(hora) - 24
                        # print(( "entrou" + str(hora)))
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
                        
        
            
        
        
    def latlong(self, lt_lo):
        minutes = 0.0
        dec_deg = 0
        decimal = 0.0
        degrees = 0.0
        # position = 0.0
        
        print("LATLONG ", lt_lo)
        degrees = int(lt_lo/100)
        print("GRAUS ", degrees)
        minutes = lt_lo - (degrees * 100)
        print("MINUTES ", minutes)
        dec_deg = minutes / 60.00;
        print("Décimos de GRAUS ", dec_deg)
        decimal = degrees + dec_deg;
        print("décimos ", decimal)
        
        return decimal	
        
        # return (km_rodado, consumo, rpmmax, velmax, hora_g, min_g)
class mapa(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")   
        

        self.browser = wx.html2.WebView.New(self)
        # url = "https://www.google.com/maps/@-23.6022347,-46.5409388,17.05z"
        url = ""
        self.browser.LoadURL(url)
        t_x, t_y = size_window

        self.browser.SetSize((t_x/7*6, t_y/3 *2))

        self.Show()
        
class Painel_grafico_A(wx.Panel):

    def __init__(self, parent): 
        wx.Panel.__init__(self, parent)
        self.panel = wx.Panel(self)
        posicao_y_botao = t_y/3 - 24
        print(posicao_y_botao, " <--- pos")
        self.botao_voltar = wx.Button(self, wx.NewId(), "Voltar", (10, posicao_y_botao ), (200,24))

    def montar_grafico(self):
        global velocidade

        i = 0
        velocidade = str(velocidade)
        velocidade_list = velocidade.split(',')
        velocidade_list = [int(i) for i in velocidade_list]
        print(( velocidade ))
        # valor = self.GetSize()
        figure = Figure()
        axes = figure.add_subplot(111)
        axes.set_title('Grafico de Velocidades', fontstyle='normal', fontsize="10", verticalalignment='baseline')
        canvas = FigureCanvas(self, -1, figure)
        t_x, t_y = size_window
        canvas.SetSize((t_x/4*3, t_y/3 - 30))

        for x in range(len(velocidade_list)):
            if(x == 0):
                item_a = "0"
            else:
                item_a = item_a + ", " + str(x);
    
        item_a = item_a.split(',')
        item_a = [int(i) for i in item_a]
        

        axes.bar(item_a, velocidade_list, color="gray")
        # axes.set_autoscale_on
        axes.plot(item_a,velocidade_list, 'r')
        axes.legend(['Velocidade permitida', 'Velocidade atingida'])
        axes.set_ylabel('Velocidade Km/h')
        axes.set_xlabel('Medicoes')
        axes.set_ymargin(10)


    
class Painel_grafico_B(wx.Panel): #RPM
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        posicao_y_botao = t_y/3 - 24
        print(posicao_y_botao, " <--- pos")
        self.botao_voltar = wx.Button(self, wx.NewId(), "Voltar", (10, posicao_y_botao ), (200,24))
    def montar_grafico(self):
        
        global rpm
        rpm = str(rpm)
        i = 0
        print(( rpm ))
        
        # rpm = list(rpm)
        rpm_list = rpm.split(',')
        rpm_list = [int(i) for i in rpm_list]
        # rpm = "(" + rpm + ")"
        print(( rpm ))
        
        
        figure = Figure()
        
        axes = figure.add_subplot(111)
        axes.set_title('Grafico de RPM ', fontstyle='italic')
        canvas = FigureCanvas(self, -1, figure)
        t_x, t_y = size_window
        canvas.SetSize((t_x/4*3, t_y/3 - 30))
        for x in range(len(rpm_list)):
            if(x == 0):
                item_b = "0"
            else:
                item_b = item_b + ", " + str(x);
           # print(( "itens")

        
        item_b = item_b.split(',')
        item_b = [int(i) for i in item_b]
        
        # valores = rpm
        axes.bar(item_b, rpm_list, color="green")
        # self.axes.set_autoscale_on
        # self.axes.plot(rpm)
 
    
class Painel_infos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
    
        tab = 200
        linha = 40
        l_espace = 23
        global a
        posicao_y_botao = t_y/3 - 24
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
        
        self.botao_grafico = wx.Button(self, wx.NewId(), "Gráfico de velocidades", (10, posicao_y_botao), (180, 24))
        self.botao_graficoRPM = wx.Button(self, wx.NewId(), "Gráfico de rotações", (200, posicao_y_botao), (180, 24))
        
        
    def mostrar_dados(self):
        print(( "aqui" , a))
        global km_rodado, consumo, rpmmax, velmax, hora_g, min_g
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
        self.Layout()

class Painel_central_superior(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)  
        
        self.SetBackgroundColour("")   
        self.sizer = wx.BoxSizer()
        self.SetSizer(self.sizer)
    
        self.panel_one = Painel_infos(self)
        self.sizer.Add(self.panel_one, 1, wx.EXPAND)
        self.panel_one.botao_grafico.Bind(wx.EVT_BUTTON, self.show_panel_two)
        self.panel_one.botao_graficoRPM.Bind(wx.EVT_BUTTON, self.show_panel_tres)
        
        self.panel_two = Painel_grafico_A(self)
        self.sizer.Add(self.panel_two, 1, wx.EXPAND)
        self.panel_two.botao_voltar.Bind(wx.EVT_BUTTON, self.show_panel_one)
        self.panel_tres = Painel_grafico_B(self)
        self.sizer.Add(self.panel_tres, 1, wx.EXPAND)
        self.panel_tres.botao_voltar.Bind(wx.EVT_BUTTON, self.show_panel_one)
        self.panel_two.Hide()
        self.panel_tres.Hide()
        self.SetSize((300, 300))
        self.Centre()

    def show_panel_one(self, event):
        self.panel_one.Show()
        self.panel_two.Hide()
        self.panel_tres.Hide()
        self.panel_one.mostrar_dados()
        global a
        print(( a))
        a = 1
        
    def show_panel_two(self, event):
        self.panel_one.Hide()
        self.panel_two.Show()
        self.panel_tres.Hide()
        self.panel_two.montar_grafico()
        self.Layout()
    def show_panel_tres(self, event):
        self.panel_one.Hide()
        self.panel_two.Hide()
        self.panel_tres.Show()
        self.panel_tres.montar_grafico()
        self.Layout()
         
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
        print(( a))
        b = 1
        font = wx.SystemSettings.GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(9)
        
        panel.SetBackgroundColour('')
        vbox = wx.BoxSizer(wx.VERTICAL)
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)
        vbox1 = wx.BoxSizer(wx.VERTICAL)
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)

        pn_se = Painel_arquivos(panel) # painel superior esquerdo
        pn_se.SetBackgroundColour('')
        
        if flag_iniciar == 1:
            pn_se.Bind(wx.EVT_BUTTON, pn_se.abrir_diretorio, pn_se.botao_sdcard)
            pn_se.botao0.Bind(wx.EVT_BUTTON, lambda evt, temp=0: acao_botao_recente(evt, temp))
            pn_se.botao1.Bind(wx.EVT_BUTTON, lambda evt, temp=1: acao_botao_recente(evt, temp))
            pn_se.botao2.Bind(wx.EVT_BUTTON, lambda evt, temp=2: acao_botao_recente(evt, temp))
            pn_se.botao3.Bind(wx.EVT_BUTTON, lambda evt, temp=3: acao_botao_recente(evt, temp))
            pn_se.botao4.Bind(wx.EVT_BUTTON, lambda evt, temp=4: acao_botao_recente(evt, temp))

        def acao_botao_recente(Event, num_botao):
            print("NUM" , num_botao)
            pn_se.abrir_recentes(num_botao)
            pn_sm.show_panel_one(self) # mostrando a tela de texto dos dados
            if num_botao == 0:
                pn_se.botao0.SetBackgroundColour("gray")
            else:
                pn_se.botao0.SetBackgroundColour("")
                
            if num_botao == 1:
                pn_se.botao1.SetBackgroundColour("gray")
            else:
                pn_se.botao1.SetBackgroundColour("")
                
            if num_botao == 2:
                pn_se.botao2.SetBackgroundColour("gray")
            else:
                pn_se.botao2.SetBackgroundColour("")
                
            if num_botao == 3:
                pn_se.botao3.SetBackgroundColour("gray")
            else:
                pn_se.botao3.SetBackgroundColour("")
                
            if num_botao == 4:
                pn_se.botao4.SetBackgroundColour("gray")
            else:
                pn_se.botao4.SetBackgroundColour("")
                    
                    
        pn_sm = Painel_central_superior(panel) #painel superior central
        pn_sd = wx.Panel(panel) #painel superior direito
        pn_sd.SetBackgroundColour('')
        
        pn_ie = wx.Panel(panel) #painel inferior esquerdo
        pn_ie.SetBackgroundColour('')
        pn_id = mapa(panel) #painel inferior direito
        pn_id.SetBackgroundColour('')

        vbox.Add(hbox1, 0, wx.ALL | wx.EXPAND, b)
        hbox1.Add(pn_se, 0, wx.ALL | wx.EXPAND, b)
        hbox1.Add(pn_sm, 2, wx.ALL | wx.EXPAND, b)
        hbox1.SetItemMinSize(pn_se, (180, t_y/3))

        hbox1.Add(pn_sd, 0, wx.ALL | wx.EXPAND, b)

        vbox.Add(vbox1, 1, wx.ALL | wx.EXPAND, b)
        vbox1.Add(hbox2, 1, wx.ALL | wx.EXPAND, b)
        hbox2.Add(pn_ie, 0, wx.ALL | wx.EXPAND, b)
        hbox2.SetItemMinSize(pn_ie, (180, 200))
        hbox2.Add(pn_id, 2, wx.ALL | wx.EXPAND, b)
        panel.SetSizer(vbox)
        
        

                
        # Setting up the menu.
        filemenu= wx.Menu()
        menuOpen = filemenu.Append(wx.ID_OPEN, "&Selecionar o SDCard","Selecionar o SDCard")
        menuAbout= filemenu.Append(wx.ID_ABOUT, "S&obre"," Dados sobre este programa")
        menuExit = filemenu.Append(wx.ID_EXIT,"S&air"," Fechar o programa")
    
        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&Arquivo") # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar)  # Adding the MenuBar to the Frame content.
    
        # Events.
        self.Bind(wx.EVT_MENU, pn_se.abrir_diretorio, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        self.Bind(wx.EVT_MENU, pn_se.OnAbout, menuAbout)


        #logos
        imageFile = 'imagens/logo_fatec.png'
        data = open(imageFile, "rb").read()
        png = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, png, (10, 10), (png.GetWidth()+30, png.GetHeight()))
        
        imageFile = 'imagens/logo_FDR.jpg'
        data = open(imageFile, "rb").read()
        jpg = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, jpg, (10, 120), (png.GetWidth()+30, png.GetHeight()+50))

        tab = 200
        linha = 40
        l_espace = 23
        
        
        



    
    def OnExit(self,e):
        self.Close(True)  # Close the frame.
    
    
                
            
            


def main():

    app = wx.App()
    ex = Painel(None, id =  -1, title='FDR - Telemetria',  flag = wx.MAXIMIZE)
    ex.Show()
    app.MainLoop()
    

if __name__ == '__main__':
    main()