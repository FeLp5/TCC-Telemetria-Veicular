# encoding: utf-8
import wx, os, time, threading
import vars
from wx import html2 as html
from datetime import datetime

dimensao_x = vars.t_x/7*5
dimensao_y = vars.t_y/3*2

class Relatorio(wx.Panel):
    
    
    flag_carreg = 0
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        self.SetBackgroundColour("")

        self.html = wx.html2.WebView.New(self) # criando um novo WebView
        self.html.SetSize((vars.t_x/7*5, (vars.t_y/3*2)-40)) # definindo o tamanho
    
        self.endereco = os.path.dirname(os.path.realpath(__file__)) # pegando o endereço de execução para o html (css e imagens)
        self.endereco = "file://" + self.endereco + "/" # completando a string endereço para o html
        self.count = 0.0
        
        self.statxt_aguarde = wx.StaticText(self, -1,  "" , ((dimensao_x/2) - 90, (dimensao_y/2)-150), (180, 6))
        self.statxt_aguarde.SetForegroundColour("black")
        self.statxt_aguarde.SetLabel("     AGUARDE POR FAVOR   \n   Coletando informações...")
        
    def atualiza(self):

        def tempo_atingido(event):
                self.timer.Stop()
                self.aguarda()
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, tempo_atingido, self.timer)
        self.timer.Start(2500)
        
        # if self.count == 0:
        #     if timer 
                
            
        carreg = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'> <head>  <meta http-equiv='content-type' content='text/html;charset=utf-8' /><meta name='generator' /></head><body></body></html>"
        # 
        self.html.SetPage(carreg, self.endereco) # montando a página html
            # print("x")
        
        self.gauge = wx.Gauge(self, range=100, size=(180, 30), pos=((dimensao_x /2) - 90, (dimensao_y/2) -130) )
        self.statxt_aguarde.Show()
        # self.html.SetPage("<body></body>", self.endereco) # montando a página html
        
        
    def aguarda(self):
        
        self.j = 0
        print("aguarda")
        def tempo_alcancado(event):
            
            if vars.requisicao != vars.num_dados:
                print("REqus ", vars.requisicao)
                print("NUM DADOS ", vars.num_dados)
                
                valor = float(float(vars.requisicao) / float(vars.num_dados)) * 100
                self.count = valor 
    
                self.gauge.SetValue(int(self.count))
                print("GAUGE ", int(self.count))
                # self.count += 1
                # time.sleep(0.2)
                # self.Layout()
                # self.atualiza()
                
            else:
                vars.query = 1
                self.gauge.SetValue(int(100))
                self.j += 1
                print("J", self.j)
                if self.j == 2:
                    self.timer2.Stop()
                    self.gauge.Hide()
                    self.statxt_aguarde.Hide()
                    self.atual()
        
        self.timer2 = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, tempo_alcancado, self.timer2)
        self.timer2.Start(500)
            
            
    def atual(self):
        
        self.statxt_aguarde.Hide()
        
        #testando uma maneira de gravar um arquivo e fazer as requisições a partir dele
        # with open("batch/test.txt",'w') as f:
        #     f.write("{\n")
        #     f.write('    batchItems": [\n')
        #     f.write('        {"query": "/reverseGeocode/-23.613197,-46.544090.json"},\n')
        #     f.write('        {"query": "/reverseGeocode/-23.615283,-46.543633.json"},\n')
        #     f.write("    ]\n")
        #     f.write("}")

        # botao_imprimir = wx.Button(self, wx.NewId(), "  imprimir", ((vars.t_x/7*5)-180, 16), (160,25))
        nome_arquivo = ''
        hoje = datetime.today().strftime('%d-%m-%Y')
        hora_agora = datetime.today().strftime('%H:%M')
        
        # print("VETOR" , len(vars.vetor_velocidade))
        # print("CIDADE" , len(vars.cidade))
        # pegando as informações e montando as strings que comporão o relatório
        itens = ""
        # for y in range(12):
        #     nome_arquivo = nome_arquivo + vars.caminho_bt[y - 12] 
            
        # print("RANGE", range(len(vars.nome_rua)))
        # print("RANGE", (len(vars.nome_rua)))
        print(vars.nome_rua)
        for x in range(len(vars.nome_rua)):
            tempo = vars.vetor_tempo[x]
            hora = tempo[0] + tempo[1] + "h" + tempo[2] + tempo[3]
            # print("XXXX" , x)
            if int(vars.vetor_velocidade[x]) > int(vars.speed_limit[x]):
                alerta = "<img class='alerta' src='imagens/alerta.png'>"
                print alerta
            else:
                alerta = ""
            
            itens = itens + "<tr><td>" + hora + "</td><td>" + vars.nome_rua[x]+ "</td><td>" + vars.cidade[x] + "</td><td>" + vars.estado[x] + "</td><td>" + vars.vetor_velocidade[x] + "</td><td>" + vars.speed_limit[x] + "</td><td style='text-align: center'>" + alerta + "</td><td>"+ vars.vetor_fence[x] + "</td></tr>"
            
        relatorio = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'><html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'><head><link rel='stylesheet' href='css/style.css'>	</head><body><div class='barra'><p class='painel'>RELATÓRIO</p></div><br><br><a href='#' onclick='window. print();'><button>Imprimir</button></a><table><tr><td style='height: 25%'>Data do Relatório</td><td>" + hoje + "</td><td>Hora do Relatório</td><td>" + str(hora_agora) + "</td><td rowspan=4 style='text-align: center;'><img src='imagens/logo_FDR.png' size='300' ></td></tr><tr><td style='height: 25%'>Arquivo</td><td>" + vars.nome_arquivo.encode(encoding='UTF-8',errors='strict') + "</td><td>VIN</td><td>" + vars.VIN + "</td></tr><tr><td style='height: 25%'>Data do Trajeto</td><td>" + vars.data_arquivo + "</td><td>Hora de início do Trajeto</td><td>" + vars.hora_inicio + "</td></tr><tr><td style='height: 25%'>Tempo de Viagem</td><td>" + str(vars.hora_g) + "h" + str(vars.min_g) + "</td><td>Velocidade Máxima Atingida</td><td>" + str(vars.velmax) + " km/h</td></tr></table><br><table><tr><td>Hora</td><td>Via</td><td>Cidade</td><td>Estado</td><td>Velocidade</td><td>Velocidade Permitida</td><td></td><td>Fence</td>			</tr>"  +itens.encode(encoding='UTF-8',errors='strict') + "</table></body></html>"
        
        
        self.html.SetPage(relatorio, self.endereco) # montando a página html
        
        # botao_imprimir.Bind(wx.EVT_BUTTON, self.imprimir) # listener do botão imprimir
        
    def imprimir(self, event):
        self.html.Print()
      
       