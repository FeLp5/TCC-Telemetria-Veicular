import wx

def init_mapa(self):
    mapa(self)

        
def SizeChange(event):
    # width, height = event.GetSize()
    # print(width, height)
    size_window = event.GetSize()
    largura = size_window[0] / 2
    print(largura , "largura")


#executa os frames
class MyApp(wx.App):
    def OnInit(self):
        global frame
        frame = wx.Frame(None, -1, __file__, (0, 0), size_window)
        frame.panel = MyPanel(frame)
        frame.Show()
        self.SetTopWindow(frame)
        return True
        
