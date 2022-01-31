from matplotlib import pyplot as plt
plt.style.use("ggplot")

grupos = ['A', 'B', 'C' , 'D', 'E', 'F', 'G' , 'H', 'I', 'J', 'K', 'L' ]
valores = [35, 77, 60, 43, 20, 36, 35, 38, 50, 51, 53, 35]
plt.bar(grupos, valores)

# Eixo_x, Eixo_y
plt.plot([-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12] , [40, 50, 50, 40, 40, 40, 40, 40, 40, 50, 50, 50, 50, 50])

plt.ylabel('Velocidade km/h')
plt.xlabel('Locais')



plt.show()

