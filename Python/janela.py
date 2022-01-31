import os

import re

count = 0

pasta = './'
for diretorio, subpastas, arquivos in os.walk(pasta):
    for arquivo in arquivos:
        print(os.path.join(diretorio, arquivo))
        infos = str(os.stat(arquivo))
        # tamanho = infos.rfind("st_size", 0, ",")
        # print (tamanho)
        
        try:
            found = re.search('st_size=(.+?),', infos).group(1)
            print(found)
        except AttributeError:
            pass
            
        count = count + 1
        b[count] = wx.Button(self, wx.NewId(), infos, (-1, -1), (-1, -1))