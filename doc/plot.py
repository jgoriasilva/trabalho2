import glob
import os
import numpy as np
import math

media = 0
desvio = 0
dados = []
data = []
matrix = []

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
      data = [N] + [size] + [tipo] + [media] + [desvio]
      matrix = matrix + data
      media = 0
      desvio = 0
      dados.clear()
      data.clear()
      continue
  arq.close() 
print(matrix) 
