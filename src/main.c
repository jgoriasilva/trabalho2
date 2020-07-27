
#include "imageprocessing.h"
#include <stdio.h>

#define N 3

int main() {
  unsigned int tmp;
  int media[3], count[3], quantidade;
  char tipo;
  printf("Tipo de execução [(N)one/(T)hread/(P)rocess]: ");
  scanf("%c", &tipo);
 
  printf("Quantidade de execuções: ");
  scanf("%d", &quantidade);

  // Imagem?
  imagem img;
  
  switch(tipo){
    case 'N':
      for(int run=0; run<quantidade; run++){
        img = abrir_imagem("data/cachorro.jpg");
		    for (int i=0; i<(img.width); i++) { // percorre colunas
		      for (int j=0; j<(img.height); j++) { // percorre linhas
		        /* Zera algumas variaveis de interesse */
		        for(char aux=0; aux<3; aux++) media[aux] = 0;
		        for(char aux=0; aux<3; aux++) count[aux] = 0;

		        for(int k=i-N; k<=i+N; k++){ // percorre colunas
		          if(k<0||k>(img.width)) continue; // Retira bordas
		          for(int l=j-N; l<=j+N; l++){ // percorre linhas
		            if(l<0 || l>(img.height)) continue; // Retira bordas
		            media[0] += img.r[l*img.width+k];
		            media[1] += img.g[l*img.width+k];
		            media[2] += img.b[l*img.width+k];
		            for(char aux=0; aux<3; aux++) count[aux]++;
		          }
		        }
		        for(char aux=0; aux<3; aux++){
		          media[aux] = media[aux]/count[aux];
		          tmp = media[aux];
		          if(tmp>255) tmp = 255;
		          switch(aux){
		            case 0: img.r[j*img.width + i] = tmp; break;
		            case 1: img.g[j*img.width + i] = tmp; break;
		            case 2: img.b[j*img.width + i] = tmp; break;
		          }
		        }
		      }
		    }
        salvar_imagem("data/cachorro-out.jpg", &img);
        liberar_imagem(&img);
      }
		  break;   
    case 'T':
      break;
    case 'P':
      break;
  } 
  return 0;
}
