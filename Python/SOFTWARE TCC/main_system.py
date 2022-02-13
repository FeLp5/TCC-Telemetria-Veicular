# importando os modulos
import main_initialize

import os
import sys
import wx
import re

from wx import html2 as webview

import matplotlib

from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx
from matplotlib.figure import Figure

from pathlib import Path

import wx.lib.scrolledpanel as scrolled

global size_window
size_window = ((wx.MAXIMIZE/4, wx.MAXIMIZE))





def main():
    app = MyApp()
    app.MainLoop()
      
    
if __name__ == "__main__" :
    main()
