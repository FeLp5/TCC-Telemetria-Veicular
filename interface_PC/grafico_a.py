# encoding: utf-8
import wx
import vars
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

class Painel_grafico_A(wx.Panel):

    def __init__(self, parent): 
        wx.Panel.__init__(self, parent)
        self.panel = wx.Panel(self)

    def montar_grafico(self):

        i = 0
        velocidade = str(vars.velocidade)
        velocidade_list = velocidade.split(',')
        velocidade_list = [int(i) for i in velocidade_list]
        print(( velocidade ))
        # valor = self.GetSize()
        figure = Figure()
        axes = figure.add_subplot(111)
        axes.set_title('Grafico de Velocidades', fontstyle='normal', fontsize="10", verticalalignment='baseline')
        canvas = FigureCanvas(self, -1, figure)
        t_x, t_y = vars.size_window
        altura = int(t_y/3*1.5)
        # print ("Altura " , altura)
        largura = int(t_x/7*6)
        # print ("Largura " , largura)
        canvas.SetSize((largura, altura))

        for x in range(len(velocidade_list)):
            if(x == 0):
                item_a = "0"
            else:
                item_a = item_a + ", " + str(x);
    
        item_a = item_a.split(',')
        item_a = [int(i) for i in item_a]
        axes.bar(item_a, velocidade_list, color="gray")
        axes.set_autoscale_on
        axes.plot(item_a,velocidade_list, 'r')
        axes.legend(['Velocidade permitida', 'Velocidade atingida'])
        axes.set_ylabel('Velocidade Km/h')
        axes.set_xlabel('Medicoes')
        axes.set_ymargin(4)
        

