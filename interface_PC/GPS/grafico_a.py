# encoding: utf-8
import wx
import vars
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas


dimensao_x = vars.t_x/7*5
dimensao_y = vars.t_y/3*2

class Painel_grafico_A(wx.Panel):

    def __init__(self, parent): 
        wx.Panel.__init__(self, parent)
        
        
    def atualiza(self):
        
        # self.esconde_grafico()
            
        self.panel = wx.Panel(self)
        self.count = 0.0
        
        self.statxt_aguarde = wx.StaticText(self, -1,  "" , ((dimensao_x/2) - 90, (dimensao_y/2)-150), (180, 6))
        self.statxt_aguarde.SetForegroundColour("black")
        self.statxt_aguarde.SetLabel("     AGUARDE POR FAVOR   \n   Coletando informações...")
        
        def tempo_atingido(event):
            self.timer.Stop()
            self.aguarda()
            
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, tempo_atingido, self.timer)
        self.timer.Start(2500)
        
        # if self.count == 0:
        #     if timer 
            
        # carreg = "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'> <html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'> <head>  <meta http-equiv='content-type' content='text/html;charset=utf-8' /><meta name='generator' /></head><body><img src='imagens/loading.gif'></body></html>"
        # # 
        # self.html.SetPage(carreg, self.endereco) # montando a página html
            # print("x")
        
        self.gauge = wx.Gauge(self, range=100, size=(180, 30), pos=((dimensao_x /2) - 90, (dimensao_y/2) -130) )
        self.statxt_aguarde.Show()
        # self.html.SetPage("<body></body>", self.endereco) # montando a página html
        
        
    def aguarda(self):
        
        # self.esconde_grafico()

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
        
        i = 0
        # self.mostra_grafico()

            
        velocidade = str(vars.velocidade)
        print(( velocidade ))
        velocidade_list = velocidade.split(',')
        velocidade_list = [int(i) for i in velocidade_list]
        print(( velocidade_list ))
        # valor = self.GetSize()
        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.axes.set_title('Velocidades', fontstyle='normal', fontsize="10", verticalalignment='baseline')
        self.canvas = FigureCanvas(self, -1, self.figure)
        t_x, t_y = vars.size_window
        altura = int(t_y/3*1.5)
        # print(("Altura " , altura))
        largura = int(t_x/7*5)
        # print(("Largura " , largura))
        self.canvas.SetSize((largura, altura))
        
        # vars.speed_limit = [40,40,40,40,40,40]
        speed_limit = ''
        itens = vars.num_dados
        print(itens)
        for x in range(itens):
            if x < itens - 1:
                print(x)
                speed_limit = speed_limit + str(vars.speed_limit[x]) + ","  
            
        speed_limit = speed_limit + str(vars.speed_limit[x])
        
        
        for x in range(len(velocidade_list)):
            if(x == 0):
                item_a = "0"
            else:
                item_a = item_a + ", " + str(x);
        
        print("SPEED", speed_limit)
        speed_limit = speed_limit.split(',')
        speed_limit = [int(i) for i in speed_limit]
        print("SPEED", speed_limit)
        
        item_a = item_a.split(',')
        item_a = [int(i) for i in item_a]
        
        self.axes.bar(item_a, velocidade_list, color="gray")
        self.axes.set_autoscale_on
        self.axes.plot(item_a, speed_limit, 'r')
        self.axes.legend(['Velocidade permitida', 'Velocidade atingida'])
        self.axes.set_ylabel('Velocidade Km/h')
        self.axes.set_xlabel('Medicoes')
        self.axes.set_ymargin(4)
    

    def esconde_grafico(self):
        for x in range(3):
            try:
                self.canvas.Hide()
            except:
                pass
        
    def mostra_grafico(self):
        try:
            self.canvas.Show()
        except:
            pass
        
        


