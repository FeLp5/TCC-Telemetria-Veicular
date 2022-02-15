import wx
from wx import html2 as webview


def mapa(self):
        # BASE_DIR = Path(__file__).absolute().parent
        # file = "teste.html"
        # final_path = BASE_DIR / file
        # final_path = "file://" + str(final_path) + "?dados=saoos"
        # 
        # print(final_path)
        # print(largura, " largura")
        #def __init__(self, parent, content='', *args, **kwargs):

        self.browser = wx.html2.WebView.New(self)
        url = "http://maps.google.com"
        # url = ""
        self.browser.LoadURL(url)
        #self.SetSizer() 
        self.browser.SetSize((size_window[0] / 2, 400))
        # self.browser.SetPage(content,'')
        
        #self.Center()
        #self.CenterOnScreen()
        
        self.Show()