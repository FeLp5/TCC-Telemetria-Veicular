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
            self.statxt_02.SetForegroundColour(vars.cor_botoes_texto)
            self.botao0 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[0])), (10, 42), size = (-1,20)) 
            self.botao1 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[1])), (10, 64), size = (-1,20))
            self.botao2 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[2])), (10, 86), size = (-1,20))
            self.botao3 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[3])), (10, 108), size = (-1,20))
            self.botao4 = wx.Button(self, wx.ID_ANY, self.convert_data((titulo_bt[4])), (10, 130), size = (-1,20))
            self.botao_mais = wx.Button(self, wx.NewId(), "Mais antigos", (10, 162),(-1,20))
            self.botao_mais.SetBackgroundColour(vars.cor_botoes)
            self.botao_mais.SetForegroundColour(vars.cor_botoes_texto)

            self.botao0.Hide()
            self.botao1.Hide()
            self.botao2.Hide()
            self.botao3.Hide()
            self.botao4.Hide()
            self.statxt_02.Hide()
            self.botao_mais.Hide()
            
            self.statxt_01 = wx.StaticText(self, -1, "Clique em abrir SD Card \npara abrir os arquivos \nde telemetria" , (16, 100), (-1, -1)) 
            self.statxt_01.SetForegroundColour(vars.cor_botoes_texto)
            self.botao_sdcard = wx.Button(self, wx.NewId(), "Abrir SD Card", (10, count + 32),(-1,20))
            self.botao_sdcard.SetBackgroundColour(vars.cor_botoes)
            self.botao_sdcard.SetForegroundColour(vars.cor_botoes_texto)
            self.Layout()
            vars.criou_botoes = 1
         
        else:
            self.botao0.Hide()
            self.botao1.Hide()
            self.botao2.Hide()
            self.botao3.Hide()
            self.botao4.Hide()
            self.statxt_02.Show()
            
            if cont:
                self.botao0.SetLabel(self.convert_data((titulo_bt[0])))
                self.botao0.Show()
            if cont > 1:    
                self.botao1.SetLabel(self.convert_data((titulo_bt[1])))
                self.botao1.Show()
            if cont > 2: 
                self.botao2.SetLabel(self.convert_data((titulo_bt[2])))
                self.botao2.Show()
            if cont > 3:
                self.botao3.SetLabel(self.convert_data((titulo_bt[3])))
                self.botao3.Show()
            if cont > 4:
                self.botao4.SetLabel(self.convert_data((titulo_bt[4])))
                self.botao4.Show()
            if (cont > 5):
                self.botao_mais.Show()
            
            self.statxt_01.Hide()
            self.botao_sdcard.Hide()

            self.Layout()
            
    #abrir os arquivos recentes - definindo 5 botões de acesso rápido            
    def abrir_recentes(self, nbotao):
        pathname_recente = self.botao_caminho[nbotao]

        try:
            with open(pathname_recente , 'r') as file:
                lines = file.readlines()
                
        except IOError:
            wx.LogError("Não é possível abrir o arquivo '%s'." % pathname)

        self.tratamento(lines)
        
        vars.arquivo_aberto = 1 # global arquivo_aberto -- flag

        vars.caminho_bt = self.bt_camho[nbotao] # pegando o nome do arquivo e tornando global
        vars.data_arquivo = self.convert_data_arquivo(vars.titulo_bt[nbotao])

    def nomes_botoes(self, pasta): # função para abrir os arquivos

        count = - 1
        botoes = 0
        cont = 0
        vars.titulo_bt = [] 
        titulo_bt_od = []
        botao_path = [] 
        q_arqu = 0
        diretorio = ""
        
        for diretorio, subpastas, arquivos in os.walk(pasta): # percorrendo o diretorio

            for arquivo in arquivos:
                pos = arquivo.rfind(".")
                ext = ""
                x = 0
        
                # pegando as extensoes dos arquivos
                for x in range(len(arquivo)): 
                    if( x > pos):  
                        ext = ext + arquivo[x]
   
                # pegando apenas a extensao desejada
                caminho = diretorio + "/" + arquivo
                extensao = "tlm"
                if(ext == extensao ):
                # informacao adicional dos arquivos 
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
            p = p + 1
            
        self.botao_caminho = botao_path

        if cont > 0:
            vars.flag_iniciar = 1 # flag que demonstra que há arquivos de telemetria

        # chamando a função que monta os botões
        self.mostrar_botoes(cont, vars.titulo_bt, botao_path, diretorio) 
        
    # converte a data e hora - para o formato que queremos nos botões
    def convert_data(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y - %H:%M')
        return str(data_convertida)
    
    def convert_data_arquivo(self, data):
        data_convertida = datetime.datetime.fromtimestamp(float(data)).strftime('%d/%m/%Y')
        return str(data_convertida)
    
    # mostra a janela sobre com informações sobre o programa
    def sobre(self,e):
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
            self.nomes_botoes(pathname)

    # fazendo o tratamento dos dados
    def tratamento(self, lines):
        
        #zerando as variáveis para um novo arquivo aberto
        vars.nome_rua = []
        vars.cidade = []
        vars.estado = []
        vars.speed_limit = []
        vars.cancelado = 0
        
        incremento = 1
        vars.query = 0
        vars.num_dados = (len(lines))
        
        nome_arquivo = ''
        velmax = 0
        
        # zerando os vetores
        vars.vetor_velocidade = []
        vars.vetor_rpm = []
        vars.vetor_combustivel = []
        vars.vetor_km = []
        vars.vetor_tempo = []
        vars.vetor_dtc = []
        vars.latitude = [] 
        vars.longitude = []
        vars.vetor_fence = []

        self.vars_sdcard(lines)
        self.requests_mapa(vars.num_dados)
        self.requests_mapa_fence()

    #funçao que retorna os nomes das ruas
    def request_ruas(self):
        if vars.requisicao != 0: #zerando a quantidade de requisições
            vars.requisicao = 0
        
        vars.nome_arquivo_anterior = vars.nome_arquivo
        
        for x in range(vars.num_dados):
            speed_limit = '' # para armazenar o limite de velocidade a ser tratado
            speed_limit_temp = '' #para armazenar o limite temporário
            if(vars.velocidade != ''): #!= ''): # só executa quando algo estiver carregado

                # tomtom - reverse geocode
                ruas = "https://api.tomtom.com/search/2/reverseGeocode/" + vars.latitude[x] +"," + vars.longitude[x] + ".json?&key=" + vars.api_key_tom + "&returnSpeedLimit=true"

                r = requests.get(ruas)
                if r.status_code not in range(200, 299):
                    wx.LogError("Erro na obtenção das informações da API, falta de credenciais") # alertando o usuário 
                    return None, None
                try:
                    # tomtom
                    results = r.json()['addresses'][0]
                    if 'street' in results['address']:
                        vars.nome_rua.append(results['address']['street'])
                    if 'municipality' in results['address']:
                        vars.cidade.append(results['address']['municipality'])
                    if 'countrySubdivision' in results['address']:
                        vars.estado.append(results['address']['countrySubdivision'])
                    if 'speedLimit' in results['address']:
                        # separando a string de ex. 40.00km para 40
                        speed_limit = results['address']['speedLimit']
                        if speed_limit[1] == '.':
                            speed_limit_temp = speed_limit[0]
                        if speed_limit[2] == '.':
                            speed_limit_temp = speed_limit[0] + speed_limit[1]
                        if speed_limit[3] == '.':
                            speed_limit_temp = speed_limit[0] + speed_limit[1] + speed_limit[2]
                        vars.speed_limit.append(speed_limit_temp)
                    else:
                        vars.speed_limit.append("40")

                except:
                    pass
                    
            
                vars.requisicao += 1
            
            if vars.nome_arquivo != vars.nome_arquivo_anterior: #cancelando as requisições feitas em segundo plano
                vars.requisicao = 0
                vars.nome_rua = [] 
                vars.cidade = [] 
                vars.estado = []
                vars.speed_limit = []
                break        

    def vars_sdcard (self, lines):
        for x in range(len(lines)):
            linha = lines[x] # selecionando a linha a ser percorrida
            
            #pegando as infos em vetor
            vars.vetor_velocidade.append(self.convert_velocidade(re.search('v(.+?);', linha).group(1)))
            vars.vetor_rpm.append(re.search('r(.+?);', linha).group(1))
            vars.vetor_combustivel.append(re.search('c(.+?);', linha).group(1))
            vars.vetor_km.append(re.search('k(.+?);', linha).group(1))
            vars.vetor_tempo.append(re.search('h(.+?);', linha).group(1))
            vars.vetor_dtc.append(re.search('d(.+?);', linha).group(1))
            vars.latitude.append(self.latlong(re.search('lt(.+?);', linha).group(1)))
            vars.longitude.append(self.latlong(re.search('lo(.+?);', linha).group(1)))
            vars.vetor_fence.append(re.search('f(.+?);', linha).group(1))

            if x == 0:
                velocidade = vars.vetor_velocidade[0]
                rpm = vars.vetor_rpm[0]
                combustivel = vars.vetor_combustivel[0]
                km = vars.vetor_km[0]
                tempo = vars.vetor_tempo[0]
                dtc = vars.vetor_dtc[0]
                
                velmax = velocidade
                rpmmax = rpm
                consumo = combustivel
                km_rodado = km
                hora = tempo[0] + tempo[1]
                min = tempo[2] + tempo[3]
                hora_n_utm = self.utm(hora)
                vars.hora_inicio = hora_n_utm + "h" + min
            else:
                velocidade = velocidade + "," + vars.vetor_velocidade[x]
                rpm = rpm + "," + vars.vetor_rpm[x]
                combustivel = combustivel + "," + vars.vetor_combustivel[x]
                km = km + "," + vars.vetor_km[x]
                tempo = tempo + "," + vars.vetor_tempo[x]
                dtc = dtc + "," + vars.vetor_dtc[x]
    
                #pegando a velocidade máxima
                if(int(velmax) < int(vars.vetor_velocidade[x])):
                    velmax = vars.vetor_velocidade[x]

                #pegando a rotação máxima
                rpmmax_int = vars.vetor_rpm[x]
                rpmmax = int(rpmmax)
                if rpmmax < int(rpmmax_int):
                    rpmmax = int(rpmmax_int)
                    
                if(x == (len(lines)) -1 ):
                    #pegando o consumo
                    consumo = float(consumo) - float(vars.vetor_combustivel[x])
                    
                    #ṕegando a distancia percorrida
                    km_rodado = float(vars.vetor_km[x])- float(km_rodado)
                
                    #pegando o tempo gasto
                    tempo_gasto = vars.vetor_tempo[x]
                    hora_g = tempo_gasto[0] + tempo_gasto[1]
                    min_g = tempo_gasto[2] + tempo_gasto[3]
                    
                    tempo_gasto = int(hora_g) * 60 + int(min_g) 
                    hora_inicio = int(hora) * 60 + int(min)

                    # precisa ser testado
                    if(hora_inicio > tempo_gasto): #troca de 23h para 0h
                        hora_inicio = hora_inicio - 1440
                    
                    tempo_gasto = int(tempo_gasto) - int(hora_inicio)
                    hora_g = int(tempo_gasto / 60)
                    min_g = tempo_gasto - (hora_g * 60)

                    if  len(str(hora_g)) == 1:
                        hora_g = "0" + str(hora_g)

                    if len(str(min_g)) == 1:
                        min_g = "0" + str(min_g)
  
                    vars.velocidade = velocidade # passando para global
                    vars.rpm = rpm # passando para global
                    vars.velmax = velmax # passando para global
                    vars.rpmmax = rpmmax  # passando para global
                    vars.consumo = consumo  # passando para global
                    vars.km_rodado = km_rodado # passando para global
                    vars.hora_g = hora_g # passando para global
                    vars.min_g = min_g # passando para global
                    
    def requests_mapa (self, nlinhas):
        coordenadas = "" # criando a variável 
        cordenada_central = "" # criando a variável 
        incremento = 0
        
        for x in range(nlinhas): 

            if  x == round(nlinhas / 2) + 1 : 
                
                # pegando a coordenada central
                cordenada_central = vars.latitude[x] + "," + vars.longitude[x] 
                # necessario inserir na string coordenadas também
                coordenadas = coordenadas + vars.latitude[x] + "," + vars.longitude[x]  + "|" 
            elif x == nlinhas - 1:
                # pegando a cordenada final e montando na string
                coordenadas = coordenadas + vars.latitude[x] + "," + vars.longitude[x]  
            else: 
                # tentando economizar as requisições
                if(x == incremento):
                    # pegando as coordenadas e montando a string
                    coordenadas = coordenadas + vars.latitude[x] + "," + vars.longitude[x]  + "|" 
                    incremento = incremento + 1

        # static_map - irá desenhar o mapa e a rota
        vars.static_map = "https://maps.googleapis.com/maps/api/staticmap?center=" + cordenada_central + "&scale=2&path=color:0x0000ff|weight:5|" + coordenadas + "&size=640x400&key=" + vars.api_key

    def requests_mapa_fence(self):

        coordenadas_fence = ""
        for x in range(len(vars.coordenadas_fence_long)-1) :
            coordenadas_fence = coordenadas_fence + str(vars.coordenadas_fence_lat[x]) + "," + str(vars.coordenadas_fence_long[x]) + "|"
        coordenadas_fence = coordenadas_fence + str(vars.coordenadas_fence_lat[x]) + "," + str(vars.coordenadas_fence_long[x + 1])
        
        central_lat = (vars.coordenadas_fence_lat[0] + vars.coordenadas_fence_lat[2]) / 2
        central_long = (vars.coordenadas_fence_long[0] + vars.coordenadas_fence_long[2]) / 2
        
        coordenada_central = str(central_lat) + "," + str(central_long)

        # static_map_fence - irá desenhar o mapa e um shape com o fence
        vars.static_map_fence = "https://maps.googleapis.com/maps/api/staticmap?center=" + coordenada_central + "&path=color:0xff0000ff|weight:0|fillcolor:0x0000ff35|" + coordenadas_fence + "&size=640x400&key=" + vars.api_key
    
    def latlong(self, lt_lo):
        minutes = 0.0
        dec_deg = 0
        decimal = 0.0

        degrees = 0.0
        lt_lo = float(lt_lo)
        degrees = int(lt_lo/100)
        minutes = lt_lo - (degrees * 100)
        dec_deg = minutes / 60.00;
        decimal = degrees + dec_deg;
        return str(decimal)	
        
    def latlong_to_GPS(self, lt_lo):
        minutes = 0.0
        dec_deg = 0
        decimal = 0.0

        degrees = 0.0
        lt_lo = float(lt_lo)
        degrees = int(lt_lo)
        minutes = lt_lo - (degrees)
        dec_deg = minutes * 0.6;
        decimal = degrees + dec_deg;
        return str(decimal)	
        
    def convert_velocidade(self, velocidade): # convertendo de nós para km/h
        velocidade = int(float(velocidade) * 1.852)
        return str(velocidade)

    def utm(self, hora):
        hora = int(hora) - 3
        if hora < 0:
            hora = hora + 24
        return str(hora)

        