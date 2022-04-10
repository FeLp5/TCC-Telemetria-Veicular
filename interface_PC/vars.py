# encoding: utf-8

import wx

#pegando a dimensao da tela
app = wx.App(False) # the wx.App object must be created first.    
  # returns a tuple
size_window = wx.GetDisplaySize()
cor_botoes = ""

# variáveis globais
flag_iniciar = 0
criou_botoes = 0
# a = 0
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
criou_botoes = 0
t_x, t_y = size_window
arquivo_aberto = 0
print (t_x , t_y )
tema = ("", "", "")
titulo_bt = 0000

roads = ""
static_map = ""

api_key="XXXXXXXXXXX"
VIN = "9BWXXXXXXXXXXXXXX2345"


