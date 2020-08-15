import glob
import os
import numpy as np
import math
import matplotlib.pyplot as plt

media = 0
desvio = 0
dados = []
data = []
out = open("Resultados.txt", "w")

for f in glob.glob('*.txt'):
  arq = open(f, 'r')
  for line in arq:
    if(line.startswith("Tipo")):
      tipo = line[-2]
      continue
    if(line.startswith("Tempo real")):
      dados += [float(line.split()[-2])]
      #print(dados)
      continue
    if(line.startswith('*')):
      media = sum(dados)/len(dados)
      for element in dados:
        desvio += (element-media)**2
      desvio = math.sqrt(desvio/len(dados))
      [N, size] = f.replace('.txt', '').replace('resultados_','').split('_')
      data = [tipo] + [size] + [N] + [media] + [desvio]
      #print(data)
      out.write(str(data).replace('[', '').replace('\'','').replace(',','').replace(']','') + '\n')
      media = 0
      desvio = 0
      dados.clear()
      data.clear()
  arq.close()
out.close() 

f = open('Resultados.txt', 'r')
for line in f:
  dados = line.split()
  data = data + dados
f.close()

tipo = data[0:len(data):5]
size = data[1:len(data):5]
N = data[2:len(data):5]
media = np.array(list(map(float, data[3:len(data):5])))
deviation = np.array(list(map(float, data[4:len(data):5])))

print(tipo)
print(size)
print(N)
print(media)
print(deviation)

X = np.arange(24)
fig = plt.figure()
ax = fig.add_axes([0,0,1,1])
ax.bar(X + 0.00, data[0], color = 'b', width = 0.25)
ax.bar(X + 0.25, data[1], color = 'g', width = 0.25)
ax.bar(X + 0.50, data[2], color = 'r', width = 0.25)
ax.legend(labels=['N', 'T', 'P'])
