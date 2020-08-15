import glob
import os

media = 0
dados[]

for f in glob.glob('*.txt'):
  arq = open(f, 'r')
  for line in arq:
    if(line.startswith("Tipo")):
      tipo = line[-2]
      continue
    if(line.startswith("Quantidade")):
      quantidade = int(line.split()[-1])
      continue
    if(line.startswith("Tempo real")):
      valor = float(line.split()[-2])
      media += valor
      continue
    if(line.startswith('\n')):
      media = media/quantidade
      [N, size] = f.replace('.txt', '').replace('resultados_','').split('_')
      print(N + ' '+ size +' ' + tipo + ' ' + str(quantidade) + ' ' + str(media))
      media = 0
      continue
  arq.close()  
