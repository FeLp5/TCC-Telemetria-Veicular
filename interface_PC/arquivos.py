# encoding: utf-8
import wx, datetime, os, re
import vars
# from FDR_telemetria import Painel as painel


class Painel_arquivos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        
        # Janela esquerda superior
 
    def mostrar_botoes(self, cont, titulo_bt, bt_caminho, diretorio):
        self.bt_camho = bt_caminho
        count = 20
        botoes = 0
        x = 0
        
        # global vars.criou_botoes
        
        if(vars.flag_iniciar == 0 and vars.criou_botoes == 0):
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
            vars.criou_botoes = 1
         
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
            
            return "ok"

                
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
        # global arquivo_aberto
        vars.arquivo_aberto = 1
        

        # s_botoes(self)
        # global vars.titulo_bt
        vars.titulo_bt = self.bt_camho[nbotao] # pegando o nome do arquivo e tornando global
        
        
    def arquivos(self, pasta):
        # global vars.flag_iniciar
        count = - 1
        # global pasta
        botoes = 0
        cont = 0

        # from array import array
        vars.titulo_bt = [] 
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


                    vars.titulo_bt.insert(cont, int(datam))

                    botao_path.insert(cont, str(caminho))
                    # print((  botao_path[cont]))
                    
                    cont = cont + 1
        
        
            
        p=0
        temp = 0
        # ordenando os arquivos
        for x in range(cont):
            for w in range(cont):
                # print(( (titulo_bt[w])))
                if(vars.titulo_bt[p] > vars.titulo_bt[w]):
                    temp = vars.titulo_bt[p] 
                    vars.titulo_bt[p] = vars.titulo_bt[w]
                    vars.titulo_bt[w] = temp
                    
                    temp_path = botao_path[p]
                    botao_path[p] = botao_path[w]
                    botao_path[w] = temp_path
                    print( vars.titulo_bt[w])
            p = p+1
            # print(( ("--")        ))
        
        # return cont, titulo_bt, botao_path, diretorio
        print "CONT " , cont
        if cont > 0:
            print "**"
            vars.flag_iniciar = 1
            print vars.flag_iniciar
            # s_botoes(self)
            
        
        self.mostrar_botoes(cont, vars.titulo_bt, botao_path, diretorio)
        
  

        
        
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
            vars.iniciado
            
            
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
            
            # vars.titulo_bt
            vars.titulo_bt = pathname # pegando o nome do arquivo e tornando global
                
                
    def abrir_diretorio(self, event):
        with wx.DirDialog (None, "Escolha o SD Card com os dados de Telemetria", "",
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
        lat = self.latlong(-2345.03232)
        long = self.latlong(-04655.0021)
        print(("LAT " , lat, "LONG " , long))
        ##

        tamanho = str(len(lines))
        # print(( lines)
        # global km_rodado, consumo, rpmmax, velmax, hora_g, min_g, rpm, velocidade
        for x in range(len(lines)): 
            linha = lines[x]
            if(x == 0):
                vars.velocidade = re.search('v(.+?);', linha).group(1)
                latitude = re.search('lt(.+?);', linha).group(1)
                longitude = re.search('lo(.+?);', linha).group(1)
                vars.rpm = re.search('r(.+?);', linha).group(1)
                combustivel = re.search('c(.+?);', linha).group(1)
                km = re.search('k(.+?);', linha).group(1)
                tempo = re.search('h(.+?);', linha).group(1)
                dtc = re.search('d(.+?);', linha).group(1)
                vars.velmax = vars.velocidade # pegando o valor inicial
                vars.rpmmax = vars.rpm # pegando o valor inicial
                vars.consumo = combustivel # pegando o valor inicial
                vars.km_rodado = km # pegando a distancia percorrida
                hora = tempo[0] + tempo[1]
                min = tempo[2] + tempo[3]
                
            else:
                vars.velocidade = vars.velocidade + "," + re.search('v(.+?);', linha).group(1)
                latitude = latitude + "," + re.search('lt(.+?);', linha).group(1)
                longitude = longitude + "," + re.search('lo(.+?);', linha).group(1)
                vars.rpm = vars.rpm + "," + re.search('r(.+?);', linha).group(1)
                combustivel = combustivel + "," + re.search('c(.+?);', linha).group(1)
                
                km = km + "," + re.search('k(.+?);', linha).group(1)
                tempo = tempo + "," + re.search('h(.+?);', linha).group(1)
                dtc = dtc + "," + re.search('d(.+?);', linha).group(1)
                
                
                ## montando as strings para as apis
                ## roads retornará a velocidade da via
                ## maps_api - irá desenhar o mapa e a rota
                ## geocode - trará os nomes das ruas 
                
                if x == len(lines) - 1 : 
                    t_x, t_y = vars.size_window
                    t_x = t_x/7*6
                    t_y = t_y/3*2
                    tamanho_janela = str(t_x) + "&" + str(t_y)
                    vars.roads = "https://roads.googleapis.com/v1/speedLimits?path=" + cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "&key=" + vars.api_key
                    vars.static_map = "https://maps.googleapis.com/maps/api/staticmap?size=" + tamanho_janela + "&center=" + cordenada_central + "&path=color:0x0000ff|weight:5|" + cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "&key=" + vars.api_key
                    # roads = "https://roads.googleapis.com/v1/speedLimits?path=" + api1 + "&key=" + api_key
                    print(( "ROADS API -> ", vars.roads))
                    print(( "STATIC_MAP API ->" , vars.static_map))
                elif  x == round(len(lines) / 2, 0) + 1 : 
                    print(( "ok"))
                    print(( x))
                    cordenada_central = re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) # pggando a coordenada central
                    cordenadas = cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|" # inserindo também na string aqui
                else:
                    cordenadas = cordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|"
                    

                ## ------------------------------------------------
                
                #pegando a velocidade máxima
                if(vars.velmax < re.search('v(.+?);', linha).group(1)):
                    vars.velmax = re.search('v(.+?);', linha).group(1)
                 
                #pegando a rotação máxima
                rpmmax_int = re.search('r(.+?);', linha).group(1)
                vars.rpmmax = int(vars.rpmmax)
                if vars.rpmmax < int(rpmmax_int):
                    vars.rpmmax = int(rpmmax_int)
                    
                if(x == (len(lines) - 1)):
                    vars.hora_g = tempo[0] + tempo[1]
                    # print(( hora_g))
                    # precisa ser testado
                    if(int(hora) > int(vars.hora_g) and flag == 0 ): #testando por causa da troca de 23h para 0h
                        hora = int(hora) - 24
                        # print(( "entrou" + str(hora)))
                        flag = 1
                    
                    #pegando o consumo
                    vars.consumo = float(vars.consumo) - float(re.search('c(.+?);', linha).group(1))
                    
                    #ṕegando a distancia percorrida
                    vars.km_rodado = float(re.search('k(.+?);', linha).group(1))- float(vars.km_rodado)
                
                    #pegando o tempo gasto, precisamos testar
                    tempo_gasto = (re.search('h(.+?);', linha).group(1))
                    vars.hora_g = tempo_gasto[0] + tempo_gasto[1]
                    vars.min_g = tempo_gasto[2] + tempo_gasto[3]
                    
                    vars.hora_g = int(vars.hora_g) - int(hora)
                    if(int(vars.min_g) < int(min)):
                        vars.ming_g = int(vars.min_g) - int(min) + 60
                    else:
                        vars.min_g = int(vars.min_g) -int(min)
                        
        
        # vars.s_botoes(self)
        # painel = Painel(self)
        # painel.mostra_botoes()

        
        
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