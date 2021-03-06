import glob
import os
import numpy as np
import math
import matplotlib.pyplot as plt
data = []

ans = input('Gerar Resultados.txt - (s)im (n)ao: ')

if (ans == 's'):
	media = 0
	desvio = 0
	dados = []
	out = open("Resultados.txt", "w")
	for f in glob.glob('[!Resultados]*.txt'):
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
dp = np.array(list(map(float, data[4:len(data):5])))

'''
print(tipo)
print(size)
print(N)
print(media)
print(deviation)
'''

for i in range(0,len(tipo),3):
  plt.figure()
  #ax = fig.add_axes([0,0,1,1])
  plt.bar([tipo[i], tipo[i+1], tipo[i+2]],[media[i], media[i+1], media[i+2]], color=['b', 'r', 'g'], yerr = [dp[i], dp[i+1], dp[i+2]], align='center', alpha=0.5, ecolor='black', capsize=10)
  plt.ylabel('Tempo [s]')
  plt.title('Tamanho '+size[i]+' e N = '+N[i])
  plt.xlabel('Variação: (N)one (T)hread (P)rocess')
  plt.plot()
  plt.savefig(size[i]+'_'+N[i])
plt.show()
