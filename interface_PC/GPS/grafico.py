# encoding: utf-8
import wx
import vars
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas


dimensao_x = vars.t_x/7*5
dimensao_y = vars.t_y/3*2

class Painel_grafico(wx.Panel):

    def __init__(self, parent): 
        wx.Panel.__init__(self, parent)
        self.panel = wx.Panel(self)
        self.count = 0.0
        
        self.statxt_aguarde = wx.StaticText(self, -1,  "" , ((dimensao_x/2) - 90, (dimensao_y/2)-150), (180, 6))
        self.statxt_aguarde.SetForegroundColour("black")
        self.statxt_aguarde.SetLabel("     AGUARDE POR FAVOR   \n   Coletando informações...")
        self.gauge = wx.Gauge(self, range=100, size=(180, 30), pos=((dimensao_x /2) - 90, (dimensao_y/2) -130) )

        
    def atualiza(self):
        
        self.gauge.Show()

        def tempo_atingido(event):
            self.timer.Stop()
            self.aguarda()
            
        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, tempo_atingido, self.timer)
        self.timer.Start(2500)
        
        self.statxt_aguarde.Show()

    def aguarda(self):
        
        self.j = 0
        def tempo_alcancado(event):
            if vars.nome_arquivo == vars.nome_arquivo_anterior:
                if vars.requisicao != vars.num_dados:
                    valor = float(float(vars.requisicao) / float(vars.num_dados)) * 100
                    self.count = valor 
                    try:   
                        self.gauge.SetValue(int(self.count))
                    except:
                        pass

                else:
                    vars.query = 1
                    self.gauge.SetValue(int(100))
                    self.j += 1
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
        self.gauge.Hide()
        self.gauge.Hide()
            
        velocidade = str(vars.velocidade)
        velocidade_list = velocidade.split(',')
        velocidade_list = [int(i) for i in velocidade_list]
        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.axes.set_title('Velocidades', fontstyle='normal', fontsize="10", verticalalignment='baseline')
        self.canvas = FigureCanvas(self, -1, self.figure)
        t_x, t_y = vars.size_window
        altura = int(t_y/3*1.5)
        largura = int(t_x/7*5)
        self.canvas.SetSize((largura, altura))
        
        speed_limit = ''
        itens = vars.num_dados
        for x in range(itens):
            if x < itens - 1:
                speed_limit = speed_limit + str(vars.speed_limit[x]) + ","  
            
        speed_limit = speed_limit + str(vars.speed_limit[x])
        
        
        for x in range(len(velocidade_list)):
            if(x == 0):
                item_a = "0"
            else:
                item_a = item_a + ", " + str(x);
        
        speed_limit = speed_limit.split(',')
        speed_limit = [int(i) for i in speed_limit]
        
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
        
        


