# encoding: utf-8

import wx

#pegando a dimensao da tela
app = wx.App(False) # Criando um objeto wx.App
size_window = wx.GetDisplaySize() # retorna uma tuple
t_x, t_y = size_window

cor_botoes = ""
cor_botoes_ativos = "#BABABA"
# tema = ("", "", "")
cor_fundo = "#CECECE"

#flags
flag_iniciar = 0
criou_botoes = 0
arquivo_aberto = 0
# iniciado = 0
# mudanca = 0


#vetores
vetor_velocidade = [] 
vetor_km = []
vetor_combustivel = []
vetor_dtc = []
vetor_rpm = []
vetor_tempo = []
vetor_fence = []


#dados coletados
nome_arquivo = ''
nome_arquivo_anterior = ''
num_dados = 0
km_rodado = 0
consumo = 0
rpmmax = 0 
velmax = 0 
hora_g = 0 
min_g = 0
rpm = 0
flag = 0
velocidade = 0
dtc = '--'
hora_inicio = ''
data_arquivo = ''


VIN = "--"

# itens da tela
titulo_bt = 0000
caminho_bt = ''
caminho_SDCARD = ''



# dados para as apis
roads = ""
static_map = ""
ruas = ""
static_map_fence = ""

# nomes_das_ruas = []
latitude = []
longitude = []

#credenciais apis
api_key="AIzaSyBMnrCuQWbuZlsYWl1l4D86wTzFUVT_EHU"
secret_key="PqTf169GaYbiKoLY-6URq2N2RHc="

api_key_tom = "hA4agzFVTHHbGuTurS0H2HHxgJYuQe4X"

# variáveis para reverse geocode
query = 0
nome_rua = []
cidade = []
estado = []
speed_limit = []

requisicao = 0

coordenadas_fence = ((-46.5086835, -23.6931603), (-46.501404, -236931603), (-46.501404, -23.7000299), (-46.5086835, -23.7000299))

coordenadas_fence_long = [-46.5086835, -46.501404, -46.501404, -46.5086835]
coordenadas_fence_lat = [-23.6931603, -23.6931603, -23.7000299, -23.7000299]






