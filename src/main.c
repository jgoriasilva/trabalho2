#include "imageprocessing.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define N 3

pthread_t threads[3];
pthread_mutex_t trava;
int quantidade, rgb=0;
imagem img;
unsigned int tmp;
int media[3], count[3];
char tipo;

void *thread(void *args){
	char* p_j  = (char*) args;
	char j  = *p_j;
	unsigned long long n;
	
	pthread_mutex_lock(&trava);
	switch(rgb){
	case 0:
		rgb++;
		pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(img.width); i++) { // percorre colunas
	  	for (int j=0; j<(img.height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[0] = 0;
	     	count[0] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(img.width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(img.height)) continue; // Retira bordas
	       		media[0] += img.r[l*img.width+k];
	          count[0]++;
	       	}
	     	}
				media[0] = media[0]/count[0];
				tmp = media[0];
				if(tmp>255) tmp = 255;
				img.r[j*img.width + i] = tmp;
	    }
	  }
		break;
	case 1:
		rgb++;
		pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(img.width); i++) { // percorre colunas
	  	for (int j=0; j<(img.height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[1] = 0;
	     	count[1] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(img.width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(img.height)) continue; // Retira bordas
	       		media[1] += img.g[l*img.width+k];
	          count[1]++;
	       	}
	     	}
				media[1] = media[1]/count[1];
				tmp = media[1];
				if(tmp>255) tmp = 255;
				img.g[j*img.width + i] = tmp;
	    }
	  }
		break;
	case 2:
		rgb=0;
		pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(img.width); i++) { // percorre colunas
	  	for (int j=0; j<(img.height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[2] = 0;
	     	count[2] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(img.width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(img.height)) continue; // Retira bordas
	       		media[2] += img.b[l*img.width+k];
	          count[2]++;
	       	}
	     	}
				media[2] = media[2]/count[2];
				tmp = media[2];
				if(tmp>255) tmp = 255;
				img.b[j*img.width + i] = tmp;
	    }
	  }
		break;
  }
}

int main() {
  printf("Tipo de execução [(N)one/(T)hread/(P)rocess]: ");
  scanf("%c", &tipo);
 
  printf("Quantidade de execuções: ");
  scanf("%d", &quantidade);

  // Imagem?
  
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
			for(int run=0; run<quantidade; run++){
        img = abrir_imagem("data/cachorro.jpg");
			  /* Gera N_THREADS threads para processamento dos números */
			  for(char j=0; j<3; j++){
				  pthread_mutex_lock(&trava);
				  //printf("Gerando thread %d...\n", j);
				  pthread_create(&threads[j], NULL, thread, &j);
				  pthread_mutex_unlock(&trava);
			  }
			  /* Espera finalização de todos os processos filhos */
			  //printf("Todos as threads foram geradas. Esperando...\n");
  		  for (char j=0; j<3; j++){
				  pthread_join(threads[j], NULL);
				  //printf("Thread %d finalizou\n", j);
			  }
			  salvar_imagem("data/cachorro-out.jpg", &img);
        liberar_imagem(&img);
      }
      break;

    case 'P':
      break;
  } 
  return 0;
}
