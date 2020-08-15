import glob
import os
import numpy as np
import math

media = 0
desvio = 0
dados = []
data = []
matrix_N = [[0 for i in range(10)] for j in range(10)]
count_N = 0
matrix_T = [[0 for i in range(10)] for j in range(10)]
count_T = 0
matrix_P = [[0 for i in range(10)] for j in range(10)]
count_P = 0

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
      if(data[0] == 'N'):
        for i in range(0,len(data)):
          print(i)
          matrix_N[count_N][i] = data[i]
          count_N += 1
      if(data[0] == 'T'):
        for i in range(1,len(data)):
          matrix_T[count_T][i] = data[i]
          count_T += 1
      if(data[0] == 'P'):
        for i in range(1,len(data)):
          matrix_P[count_P][i] = data[i]
          count_P += 1
      #print(data)
      media = 0
      desvio = 0
      dados.clear()
      data.clear()
      continue
  arq.close() 
print(matrix_N)
print(matrix_T)
print(matrix_P)
