# encoding: utf-8
import wx
import vars
from matplotlib.figure import Figure
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas

class Painel_grafico_B(wx.Panel): #RPM
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)

    def montar_grafico(self):
        rpm = str(vars.rpm)
        i = 0
        rpm_list = rpm.split(',')
        rpm_list = [int(i) for i in rpm_list]

        figure = Figure()
        
        axes = figure.add_subplot(111)
        axes.set_title('Grafico de RPM ', fontstyle='italic')
        canvas = FigureCanvas(self, -1, figure)
        t_x, t_y = vars.size_window
        canvas.SetSize((int(t_x/4*3), int(t_y/3 - 30)))
        for x in range(len(rpm_list)):
            if(x == 0):
                item_b = "0"
            else:
                item_b = item_b + ", " + str(x);

        item_b = item_b.split(',')
        item_b = [int(i) for i in item_b]
        
        axes.bar(item_b, rpm_list, color="green")

 