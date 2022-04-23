# encoding: utf-8
import wx, datetime, os, re, requests
import vars


class Painel_arquivos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
    
    #mostrar os botões
    def mostrar_botoes(self, cont, titulo_bt, bt_caminho, diretorio): 
        self.bt_camho = bt_caminho
        count = 20
        botoes = 0
        x = 0

        if(vars.flag_iniciar == 0 and vars.criou_botoes == 0):
            titulo_bt = [0] * 5
            self.statxt_02 = wx.StaticText(self, -1, "Trajetos Recentes" , (16, 10), (-1, -1))
            self.botao0 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[0])), (10, 42), size = (-1,20)) 
            self.botao1 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[1])), (10, 64), size = (-1,20))
            self.botao2 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[2])), (10, 86), size = (-1,20))
            self.botao3 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[3])), (10, 108), size = (-1,20))
            self.botao4 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[4])), (10, 130), size = (-1,20))
            self.botao_mais = wx.Button(self, wx.NewId(), "Mais antigos", (10, 162),(-1,20))

            self.botao0.Hide()
            self.botao1.Hide()
            self.botao2.Hide()
            self.botao3.Hide()
            self.botao4.Hide()
            self.statxt_02.Hide()
            self.botao_mais.Hide()
            
            self.statxt_01 = wx.StaticText(self, -1, "Clique em abrir SD Card \npara abrir os arquivos \nde telemetria" , (16, 100), (-1, -1)) 
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
            
            # return "ok"

    #abrir os arquivos recentes - definindo 5 botões de acesso rápido            
    def abrir_recentes(self, nbotao):
        pathname_recente = self.bt_camho[nbotao]
        # for x in range(len(self.bt_camho)):
        # # Proceed loading the file chosen by the user
        # # print(( str(pathname)))
        #     print("Caminho -> " + str(x) + " " + self.bt_camho[x])
        try:
            with open(pathname_recente , 'r') as file:
                # print(pathname_recente)
                lines = file.readlines()
                # print(((lines)))
                # print(( str(file)))
                # self.lines = lines
                # info = lines
                
        except IOError:
            wx.LogError("Não é possível abrir o arquivo '%s'." % pathname)

        self.tratamento(lines)
        
        vars.arquivo_aberto = 1 # global arquivo_aberto -- flag

        # vars.titulo_bt = self.bt_camho[nbotao] 
        vars.caminho_bt = self.bt_camho[nbotao] # pegando o nome do arquivo e tornando global
        
    def arquivos(self, pasta): # função para abrir os arquivos

        count = - 1
        botoes = 0
        cont = 0
        vars.titulo_bt = [] 
        titulo_bt_od = []
        botao_path = [] 
        q_arqu = 0
        diretorio = ""
        
        for diretorio, subpastas, arquivos in os.walk(pasta): # percorrendo o diretorio
        
            # print("DIRETORIO", diretorio)
            # print("SUBPASTAS", pasta)
            for arquivo in arquivos:
                pos = arquivo.rfind(".")
                ext = ""
                x = 0

                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
                        # print(ext)
                        
                # pegando apenas a extesnao desejada
                caminho = diretorio + "/" + arquivo
                # print("CAMINHO" + caminho)
                extensao = "tlm"
                if(ext == extensao ):
                # informacao adicional dos arquivos 
                    # datacri = "" # data de criação dos arquivos
                    # datamod = "" # data de modificação dos arquivos
                    
                    infos = str(os.stat(caminho)) # armazenando na variável os dados dos arquivos
                    
                    try:
                        datam = re.search('st_mtime=(.+?),', infos).group(1)
                        
                    except AttributeError:
                        pass

                    vars.titulo_bt.insert(cont, int(datam)) # adicionando os valores no array
                    botao_path.insert(cont, caminho) # adicionando os caminhos no array
                    cont = cont + 1 # incrementando
                    
        p = 0 # criando e definindo um contador p
        temp = 0 # criando uma variável temporária com 0
        # ordenando os arquivos
        
        #definindo a ordem em que os botões aparecerão na tela 
        for x in range(cont):
            for w in range(cont):
                if(vars.titulo_bt[p] > vars.titulo_bt[w]):
                    temp = vars.titulo_bt[p] 
                    vars.titulo_bt[p] = vars.titulo_bt[w]
                    vars.titulo_bt[w] = temp
                    
                    temp_path = botao_path[p]
                    botao_path[p] = botao_path[w]
                    botao_path[w] = temp_path
                    # print( vars.titulo_bt[w])
            p = p + 1
        
        if cont > 0:
            vars.flag_iniciar = 1 # flag que demonstra que há arquivos de telemetria
            # print vars.flag_iniciar
        
        # chamando a função que monta os botões
        self.mostrar_botoes(cont, vars.titulo_bt, botao_path, diretorio) 
        
    # converte a data e hora - para o formato que queremos nos botões
    def convert_data(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
        return str(data_convertida)
    
    # mostra a janela sobre com informações sobre o programa
    def OnAbout(self,e):
        dlg = wx.MessageDialog(self, "\n      FDR Telemetria      \n    Fatec Santo André     \n\nAbril de 2022 - Versão 0.1", "FDR Telemetria v0.1", wx.OK)
        dlg.ShowModal() # Mostra a mensagem
        dlg.Destroy() # Destroi e finaliza
                
    # abre a janela para selcionar o diretório que contém o SD Card             
    def abrir_diretorio(self, event):
        with wx.DirDialog (None, "Escolha o SD Card com os arquivos de Telemetria", "",
                    wx.DD_DEFAULT_STYLE | wx.DD_DIR_MUST_EXIST) as fileDialog:
            if fileDialog.ShowModal() == wx.ID_CANCEL:
                return     
    
            # armazenando o diretório selecionado
            pathname = fileDialog.GetPath()
            self.arquivos(pathname)

    # fazendo o tratamento dos dados
    def tratamento(self, lines):
        coordenadas = "" # criando a variável 
        cordenada_central = "" # criando a variável 
        vars.nomes_das_ruas = []
        
        # código para reduzir as requisições, podemos ajustar a quantidade de requisições por aqui
        incrementoA = 0
        incrementoB = 1
        # gerando um fator para diminuir requisições
        fator = 0
        if (len(lines) < 250):
            fator = 10
        if(len(lines) < 100):
            fator = 2
        if(len(lines) < 50):
            fator = 1
            
        # percorrendo os dados do arquivo e montando as strings
        for x in range(len(lines)): 
            linha = lines[x] # selecionando a linha a ser percorrida

            # pegando as coordenadas e cordenada central para centralizar o mapa (static map)
            if  x == round(len(lines) / 2, 0) + 1 : 
                cordenada_central = re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) # pegando a coordenada central
                coordenadas = coordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|" # necessario inserir na string coordenadas também
            elif x == len(lines) - 1:
                coordenadas = coordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) # pegando a cordenada final e montando na string
            else: # tentando economizar as requisições
 
                if(x == incrementoA * fator):
                    coordenadas = coordenadas + re.search('lt(.+?);', linha).group(1) + "," + re.search('lo(.+?);', linha).group(1) + "|" # pegando as coordenadas e montando a string
                    incrementoA = incrementoA + 1
                    print "VEZES", x
                    
                
            
            if(x == 0): # pegando o valor inicial para montar a string
                velocidade = re.search('v(.+?);', linha).group(1)
                latitude = re.search('lt(.+?);', linha).group(1) 
                longitude = re.search('lo(.+?);', linha).group(1)
                rpm = re.search('r(.+?);', linha).group(1)
                combustivel = re.search('c(.+?);', linha).group(1)
                km = re.search('k(.+?);', linha).group(1)
                tempo = re.search('h(.+?);', linha).group(1)
                dtc = re.search('d(.+?);', linha).group(1)
                
                velmax = velocidade
                rpmmax = rpm
                consumo = combustivel
                km_rodado = km
                hora = tempo[0] + tempo[1]
                min = tempo[2] + tempo[3]
                lat = latitude
                long  = longitude
                
                #gerando  os nomes das ruas para esta cordenada
                self.ruas(latitude, longitude, x)
                
            else: #pegando as demais linhas
                velocidade = velocidade + "," + re.search('v(.+?);', linha).group(1)
                latitude = latitude + "," + re.search('lt(.+?);', linha).group(1)
                longitude = longitude + "," + re.search('lo(.+?);', linha).group(1)
                rpm = rpm + "," + re.search('r(.+?);', linha).group(1)
                combustivel = combustivel + "," + re.search('c(.+?);', linha).group(1)
                km = km + "," + re.search('k(.+?);', linha).group(1)
                tempo = tempo + "," + re.search('h(.+?);', linha).group(1)
                dtc = dtc + "," + re.search('d(.+?);', linha).group(1)
                
               
                if (x == incrementoB * fator): # economizando nas requisições
                # gerando os outros nomes das ruas para as demais coordenadas
                    self.ruas(re.search('lt(.+?);', linha).group(1), re.search('lo(.+?);', linha).group(1), x)
                    incrementoB = incrementoB + 1
                    print "RUAS", x

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

                    # precisa ser testado
                    if(int(hora) > int(hora_g) and flag == 0 ): #testando por causa da troca de 23h para 0h
                        hora = int(hora) - 24

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
                        
                    vars.velocidade = velocidade # passando para global
                    vars.rpm = rpm # passando para global
                    vars.velmax = velmax # passando para global
                    vars.rpmmax = rpmmax  # passando para global
                    vars.consumo = consumo  # passando para global
                    vars.km_rodado = km_rodado # passando para global
                    vars.hora_g = hora_g # passando para global
                    vars.min_g = min_g # passando para global
        

        # roads retornará a velocidade da via
        vars.roads = "https://roads.googleapis.com/v1/speedLimits?path=" + coordenadas  + "&key=" + vars.api_key
        
        # static_map - irá desenhar o mapa e a rota
        vars.static_map = "https://maps.googleapis.com/maps/api/staticmap?center=" + cordenada_central + "&path=color:0x0000ff|weight:5|" + coordenadas + "&size=640x400&key=" + vars.api_key

        print(( "ROADS API -> ", vars.roads))
        print(( "STATIC_MAP API ->" , vars.static_map))

    #funçao que retorna os nomes das ruas
    def ruas(self, lat, long, cont):
        print "REQUISIÇÕES: " , cont

        if(vars.velocidade == 4000): #!= ''): # só executa quando algo estiver carregado
        
            # geocode - trará os nomes das ruas
            ruas = "https://maps.googleapis.com/maps/api/geocode/json?latlng=" +lat+","+long+"&location_type=ROOFTOP&result_type=street_address&key=" + vars.api_key
            
            r = requests.get(ruas)
            if r.status_code not in range(200, 299):
                return None, None
            try:
                results = r.json()['results'][0]
                for types in results['address_components']:
                    field = types.get('types', [])
                    if 'route' in field:
                        nome_rua = types['long_name']
                    if 'administrative_area_level_2' in field:
                        nome_rua = nome_rua + "   - Cidade: " + types['long_name']

                print "ERROR"
            except:
                print "ERRO"
                pass
            
                    
            vars.nomes_das_ruas.append(nome_rua)
            # print vars.nomes_das_ruas
           
    
    #função de conversão de latitude / longitude para graus/minutos
    def latlong(self, lt_lo):
        minutes = 0.0
        dec_deg = 0
        decimal = 0.0

        degrees = 0.0
        # position = 0.0
        
        # print("LATLONG ", lt_lo)
        degrees = int(lt_lo/100)
        # print("GRAUS ", degrees)
        minutes = lt_lo - (degrees * 100)
        # print("MINUTES ", minutes)
        dec_deg = minutes / 60.00;
        print("Décimos de GRAUS ", dec_deg)
        # decimal = degrees + dec_deg;
        print("décimos ", decimal)
        
        return decimal	
