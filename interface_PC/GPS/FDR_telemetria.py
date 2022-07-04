#!/usr/bin/env python
# encoding: utf-8

import wx, os, re, datetime, time, threading

#importando as classes em python
from mapa import Mapa 
from arquivos import Painel_arquivos
from grafico import Painel_grafico
from mapa_fence import Mapa_fence
from infos import Painel_infos
from relatorio import Relatorio
from inf_direito import Painel_inferior_direito
import vars

class Painel(wx.Frame):
    def __init__(self, parent, id, title, flag ):
        
        print ("\n ####### FDR TELEMETRIA V0.1 #######")
        print (" #######  FATEC SANTO ANDRÉ  #######\n") # imprimindo no terminal apenas estético

        # informando que as credenciais das apis precisam ser definidas para uso desta interface
        if vars.api_key == " " or vars.secret_key == " " or vars.api_key_tom == " ":
             wx.LogMessage("Para a utilização desta interface é\nnecessário inserir as credenciais das\nAPI's Tom Tom e Google no\narquivo vars.py")
        
        
        super(Painel, self).__init__(parent, title=title, size = vars.size_window)

        self.Centre()
        
        panel = wx.Panel(self)
        border = 1 # tamanho da borda da frame
        font = wx.SystemSettings.GetFont(wx.SYS_SYSTEM_FONT)
        font.SetPointSize(9)

        panel.SetBackgroundColour(vars.cor_fundo)
        vbox = wx.BoxSizer(wx.VERTICAL)
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)
        vbox1 = wx.BoxSizer(wx.VERTICAL)
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)

        pn_se = Painel_arquivos(panel) # painel superior esquerdo
        pn_se.SetBackgroundColour('')
        pn_se.mostrar_botoes(0, vars.titulo_bt, "", "") # chamando o metodo mostrar botoes
        
        def botao_sdcard(self):
             pn_se.abrir_diretorio(self)
             status_botoes(self)
             acao_botao_recente(self, -1)
            
        pn_se.Bind(wx.EVT_BUTTON, botao_sdcard, pn_se.botao_sdcard)
        

        def abrir_relatorio(self):
            if vars.query == 0:
                vars.requisicoes = []
                thread = threading.Thread(target = pn_se.request_ruas)
                thread.start()
                
            pn_id.show_panel_five()
        
        def abrir_grafico_velocidades(self):
            if vars.query == 0:
                vars.requisicoes = []
                thread = threading.Thread(target = pn_se.request_ruas)
                thread.start()
                
            pn_id.show_panel_two()
        
        def acao_botao_recente(Event, num_botao): # para mostrar qual botão foi selecionado

            if num_botao == -1:
                pn_se.botao0.SetBackgroundColour("")
                pn_se.botao1.SetBackgroundColour("")
                pn_se.botao2.SetBackgroundColour("")                
                pn_se.botao3.SetBackgroundColour("")
                pn_se.botao4.SetBackgroundColour("")

            else:
                pn_se.abrir_recentes(num_botao)
                status_botoes(self)
    
                if num_botao == 0:
                    pn_se.botao0.SetBackgroundColour(vars.cor_botoes_ativos)
                else:
                    pn_se.botao0.SetBackgroundColour("")

                if num_botao == 1:
                    pn_se.botao1.SetBackgroundColour(vars.cor_botoes_ativos)
                else:
                    pn_se.botao1.SetBackgroundColour("")

                if num_botao == 2:
                    pn_se.botao2.SetBackgroundColour(vars.cor_botoes_ativos)
                else:
                    pn_se.botao2.SetBackgroundColour("")
                    
                if num_botao == 3:
                    pn_se.botao3.SetBackgroundColour(vars.cor_botoes_ativos)

                else:
                    pn_se.botao3.SetBackgroundColour("")
  
                if num_botao == 4:
                    pn_se.botao4.SetBackgroundColour(vars.cor_botoes_ativos)
                else:
                    pn_se.botao4.SetBackgroundColour("")

        pn_sm = Painel_infos(panel) #painel superior central
        pn_sd = wx.Panel(panel) #painel superior direito
        pn_sd.SetBackgroundColour('')
        
        pn_ie = wx.Panel(panel) #painel inferior direito
        pn_ie.SetBackgroundColour('')
        
        pn_id = Painel_inferior_direito(panel) #painel inferior esquerdo
        pn_id.SetBackgroundColour('')
        
        botao_grafico = wx.Button(pn_ie, wx.NewId(), "  Gráfico Velocidade", (10, 46 ), (160,24))
        botao_graficoRPM = wx.Button(pn_ie, wx.NewId(), "Fence", (10,80), (160,24))
        botao_mapa = wx.Button(pn_ie, wx.NewId(), "Rota", (10, 114 ), (160,24))
        botao_relatorio = wx.Button(pn_ie, wx.NewId(), "Relatório", (10, 148), (160,24))
        
        #ações botões
        botao_grafico.Bind(wx.EVT_BUTTON, abrir_grafico_velocidades)
        botao_graficoRPM.Bind(wx.EVT_BUTTON, pn_id.show_panel_tres)
        botao_mapa.Bind(wx.EVT_BUTTON, pn_id.show_panel_four)
        botao_relatorio.Bind(wx.EVT_BUTTON, abrir_relatorio)

        # Configurando o menu
        filemenu= wx.Menu()
        # Menu Arquivo
        menuOpen = filemenu.Append(wx.ID_OPEN, "&Selecionar o SDCard","Selecionar o SDCard")
        menuExit = filemenu.Append(wx.ID_EXIT,"S&air"," Fechar o programa")
        
        # Menu Visualizar
        visualizar = wx.Menu()
        g_velocidades = visualizar.Append(wx.ID_ANY, "&Gráfico Velocidades")
        g_rotacoes = visualizar.Append(wx.ID_ANY, "&Fence")
        g_mapa = visualizar.Append(wx.ID_ANY, "&Rota")
        relatorio = visualizar.Append(wx.ID_ANY, "&Relatório")
        
        # Menu Ajuda
        ajuda_menu = wx.Menu()
        ajuda = ajuda_menu.Append(wx.ID_ANY, "&Ajuda")
        sobre = ajuda_menu.Append(wx.ID_ANY, "&Sobre", "Dados sobre este programa")

        # Criando a barra de menu
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu,"&Arquivo") # Adicionando o menu Arquivo na Barra de Menu
        menuBar.Append(visualizar,"&Visualizar") # Adicionando o menu Visualizar na Barra de Menu
        menuBar.Append(ajuda_menu,"&Ajuda") # Adicionando o menu Ajuda na Barra de Menu


        self.SetMenuBar(menuBar)  # Adicionando o menu no Frame

        # status dos botões, necessário verificar a flag, só assim não dará erro, 
        # pois os botões podem ainda não existir
        def status_botoes(self):
            
            # verificando a flag, se sinalizado ativa os listeners
            if vars.flag_iniciar == 1:
                pn_se.botao0.Bind(wx.EVT_BUTTON, lambda evt, temp=0: acao_botao_recente(evt, temp))
                pn_se.botao1.Bind(wx.EVT_BUTTON, lambda evt, temp=1: acao_botao_recente(evt, temp))
                pn_se.botao2.Bind(wx.EVT_BUTTON, lambda evt, temp=2: acao_botao_recente(evt, temp))
                pn_se.botao3.Bind(wx.EVT_BUTTON, lambda evt, temp=3: acao_botao_recente(evt, temp))
                pn_se.botao4.Bind(wx.EVT_BUTTON, lambda evt, temp=4: acao_botao_recente(evt, temp))
                pn_sm.mostrar_dados()
                pn_id.show_panel_one(self)
                menuOpen.Enable(False) # desabilitando o botão para que não seja possível reabrir o SDCARD

            #verificando se algum arquivo foi carrgado, se sim habilita os botões
            if vars.arquivo_aberto == 0:
                botao_grafico.Disable()
                botao_graficoRPM.Disable()
                botao_mapa.Disable()
                botao_relatorio.Disable()
                g_velocidades.Enable(False)
                g_rotacoes.Enable(False)
                g_mapa.Enable(False)
                relatorio.Enable(False)
            elif vars.arquivo_aberto == 1:
                botao_grafico.Enable()
                botao_graficoRPM.Enable()
                botao_mapa.Enable()
                botao_relatorio.Enable()
                g_velocidades.Enable(True)
                g_rotacoes.Enable(True)
                g_mapa.Enable(True)
                relatorio.Enable(True)
                    
        def abrir_arquivos(event):
            with wx.FileDialog(self, "Abrir arquivo de telemetria", wildcard="arquivos tlm  (*.tlm)|*.tlm",
                                style=wx.FD_OPEN | wx.FD_FILE_MUST_EXIST) as fileDialog:
                if fileDialog.ShowModal() == wx.ID_CANCEL:
                    return    
                # carregando o arquivo selecionado
                pathname = fileDialog.GetPath()

                try:
                    with open(pathname, 'r') as file:
                        lines = file.readlines()
                        self.lines = lines

                except IOError:
                    wx.LogError("Não foi possível abrir o arquivo '%s'." % newfile)
                
                pn_se.tratamento(lines)
                vars.arquivo_aberto = 1 # global arquivo_aberto -- flag
                vars.caminho_bt = pathname # pegando o nome/caminho do arquivo e tornando global
                
                infos = str(os.stat(pathname)) # armazenando na variável os dados dos arquivos
                
                try:
                    datam = re.search('st_mtime=(.+?),', infos).group(1)
                    
                except AttributeError:
                    pass

                vars.data_arquivo = datetime.datetime.fromtimestamp(float(datam)).strftime('%d/%m/%Y')
                status_botoes(self)
                acao_botao_recente(self, -1) # apagando os destaques dos botões
                
        pn_se.Bind(wx.EVT_BUTTON, abrir_arquivos, pn_se.botao_mais) # evento do botão mais antigos
        
        status_botoes(self) # chama o método

        vbox.Add(hbox1, 0, wx.ALL | wx.EXPAND, border)
        hbox1.Add(pn_se, 0, wx.ALL | wx.EXPAND, border)
        hbox1.Add(pn_sm, 2, wx.ALL | wx.EXPAND, border)
        hbox1.SetItemMinSize(pn_se, (180, int(vars.t_y/3)))

        hbox1.Add(pn_sd, 0, wx.ALL | wx.EXPAND, border)

        vbox.Add(vbox1, 1, wx.ALL | wx.EXPAND, border)
        vbox1.Add(hbox2, 1, wx.ALL | wx.EXPAND, border)
        hbox2.Add(pn_ie, 0, wx.ALL | wx.EXPAND, border)
        hbox2.SetItemMinSize(pn_ie, (180, 200))
        hbox2.Add(pn_id, 2, wx.ALL | wx.EXPAND, border)
        panel.SetSizer(vbox)
            
        # Eventos dos botões da barra de menus
        self.Bind(wx.EVT_MENU, botao_sdcard, menuOpen)
        self.Bind(wx.EVT_MENU, self.sair, menuExit)
        self.Bind(wx.EVT_MENU, pn_se.sobre, sobre)
        self.Bind(wx.EVT_MENU, abrir_grafico_velocidades, g_velocidades)
        self.Bind(wx.EVT_MENU, pn_id.show_panel_tres, g_rotacoes)
        self.Bind(wx.EVT_MENU, pn_id.show_panel_four, g_mapa)
        self.Bind(wx.EVT_MENU, abrir_relatorio, relatorio)

        #logos
        imageFile = 'imagens/logo_fatec.png'
        data = open(imageFile, "rb").read()
        png1 = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, png1, (10, 10), (png1.GetWidth()+30, png1.GetHeight()))
        
        imageFile = 'imagens/logo_FDR.png'
        data = open(imageFile, "rb").read()
        png2 = wx.Image(imageFile, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        wx.StaticBitmap(pn_sd, -1, png2, (10, 90), (png2.GetWidth()+30, png2.GetHeight()+50))


    def sair(self,e):
        self.Close(True)  # Fecha o programa

        
def main():

    app = wx.App()
    ex = Painel(None, id =  -1, title='FDR - Telemetria',  flag = wx.MAXIMIZE)
    ex.Show()
    app.MainLoop()
    

if __name__ == '__main__':
    main()