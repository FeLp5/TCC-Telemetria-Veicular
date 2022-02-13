
import wx


# class ColWin(wx.Window):
#     """- A wx.Window with a coloured background
#     - pos and size == (-1, -1) since sizers are used"""
#     
#     def __init__(self, parent, id, BackColour):
#         wx.Window.__init__(self, parent, id, (-1, -1), (-1, -1), wx.SIMPLE_BORDER )
#         self.SetBackgroundColour(BackColour)
#         largura_panel = wx.Window.GetSize(self)
#         print(largura_panel)
#         
        
        
        
    # grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
    # valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
    # plt.bar(grupos, valores)
    # 
        # # Eixo_x, Eixo_y
        # plt.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])
        # 
    # plt.ylabel('Velocidade km/h')
    # plt.xlabel('Locais')


    def init_mapa(self):
        mapa(self)
        # # BASE_DIR = Path(__file__).absolute().parent
        # # file = "teste.html"
        # # final_path = BASE_DIR / file
        # # final_path = "file://" + str(final_path) + "?dados=saoos"
        # # 
        # # print(final_path)
        # # print(largura, " largura")
        # #def __init__(self, parent, content='', *args, **kwargs):

        # self.browser = wx.html2.WebView.New(self)
        # url = "http://maps.google.com"
        # # url = ""
        # self.browser.LoadURL(url)
        # #self.SetSizer() 
        # self.browser.SetSize((size_window[0] / 2, 400))
        # # self.browser.SetPage(content,'')
        
        # #self.Center()
        # #self.CenterOnScreen()
        
        # self.Show()