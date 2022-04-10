# encoding: utf-8
import wx
import vars


#variáveis globais
# titulo_bt = ""


class Painel_infos(wx.Panel):

    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
    
        tab = 200
        linha = 60
        l_espace = 22
        # global a
        print "titulo bt ", vars.titulo_bt
        
        self.statxt_arquivo = wx.StaticText(self, -1,  "", (16, 6), (-1, -1))

    
        self.statxt_info = wx.StaticText(self, -1,  "" , (220, 26), (-1, -1))
        self.statxt_info.SetForegroundColour("black")
        
        self.statxt_inf0 = wx.StaticText(self, -1, "Informações Coletadas no Trajeto" , (16, 30), (-1, -1))
        
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
        


        
        
    def mostrar_dados(self):
        # print(( "aqui" , a))
        # global km_rodado, consumo, rpmmax, velmax, hora_g, min_g
        self.statxt_inf_r1.SetLabel(str(vars.km_rodado) + " Km") 
        self.statxt_inf_r2.SetLabel(str(vars.consumo) + " % total") # precisamos ajustar aqui
        self.statxt_inf_r3.SetLabel(str(vars.consumo * 50) + " km/l") # precisamos ajustar aqui
        self.statxt_inf_r4.SetLabel(str(vars.rpmmax) + " RPM") 
        if vars.rpmmax > 4500:
            self.statxt_inf_r4.SetForegroundColour("red")
        else:
            self.statxt_inf_r4.SetForegroundColour("")
        self.statxt_inf_r5.SetLabel(str(vars.velmax) + " Km/h") 
        self.statxt_inf_r6.SetLabel(str(vars.hora_g) + "h" + str(vars.min_g))
        
        self.statxt_arquivo.SetLabel("Arquivo " + vars.titulo_bt + "     --- VIN: " + vars.VIN)
        
        
        self.Layout()