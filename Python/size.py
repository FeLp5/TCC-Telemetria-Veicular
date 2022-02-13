import wx
def size_change(event):
    width,height = event.GetSize()
    print ("Width =",width,"Height =",height)
app = wx.App()
frame = wx.Frame(None, -1, 'Get Size Test')
frame.SetSize(500,250)
frame.Bind(wx.EVT_SIZE, size_change)
frame.Show()
print ("Original Size =",frame.GetSize())
app.MainLoop()