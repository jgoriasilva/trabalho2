#include "imageprocessing.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int tmp;
pthread_t threads[3];
pthread_mutex_t trava;
int quantidade, rgb=0;
int media[3], count[3];
char tipo;
char *nome;
int N;
pid_t filhos[3]; 

/* Criar area de memoria compartilhada */
imagem *p_img;

void *thread(void *args){
	pthread_mutex_lock(&trava);
  //printf("rgb = %d\n", rgb);
	switch(rgb){
	case 0:
    rgb=1;
    pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
	  	for (int j=0; j<(p_img->height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[0] = 0;
	     	count[0] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(p_img->width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(p_img->height)) continue; // Retira bordas
	       		media[0] += p_img->r[l*p_img->width+k];
	          count[0]++;
	       	}
	     	}
				media[0] = media[0]/count[0];
				tmp = media[0];
				if(tmp>255) tmp = 255;
				p_img->r[j*p_img->width + i] = tmp;
	    }
	  }
		break;
	case 1:
    rgb=2;
		pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
	  	for (int j=0; j<(p_img->height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[1] = 0;
	     	count[1] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(p_img->width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(p_img->height)) continue; // Retira bordas
	       		media[1] += p_img->g[l*p_img->width+k];
	          count[1]++;
	       	}
	     	}
				media[1] = media[1]/count[1];
				tmp = media[1];
				if(tmp>255) tmp = 255;
				p_img->g[j*p_img->width + i] = tmp;
	    }
	  }
		break;
	case 2:
    rgb=0;
		pthread_mutex_unlock(&trava);
	 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
	  	for (int j=0; j<(p_img->height); j++) { // percorre linhas
	    /* Zera algumas variaveis de interesse */
	    	media[2] = 0;
	     	count[2] = 0;

	     	for(int k=i-N; k<=i+N; k++){ // percorre colunas
	       	if(k<0||k>(p_img->width)) continue; // Retira bordas
	       	for(int l=j-N; l<=j+N; l++){ // percorre linhas
	         	if(l<0 || l>(p_img->height)) continue; // Retira bordas
	       		media[2] += p_img->b[l*p_img->width+k];
	          count[2]++;
	       	}
	     	}
				media[2] = media[2]/count[2];
				tmp = media[2];
				if(tmp>255) tmp = 255;
				p_img->b[j*p_img->width + i] = tmp;
	    }
	  }
		break;
  }
}

void *tipo_none(void *args){
  *p_img = abrir_imagem("data/paisagem.jpg");
  for (int i=0; i<(p_img->width); i++) { // percorre colunas
    for (int j=0; j<(p_img->height); j++) { // percorre linhas
      /* Zera algumas variaveis de interesse */
      for(char aux=0; aux<3; aux++) media[aux] = 0;
      for(char aux=0; aux<3; aux++) count[aux] = 0;

      for(int k=i-N; k<=i+N; k++){ // percorre colunas
        if(k<0||k>(p_img->width)) continue; // Retira bordas
        for(int l=j-N; l<=j+N; l++){ // percorre linhas
          if(l<0 || l>(p_img->height)) continue; // Retira bordas
          media[0] += p_img->r[l*p_img->width+k];
          media[1] += p_img->g[l*p_img->width+k];
          media[2] += p_img->b[l*p_img->width+k];
          for(char aux=0; aux<3; aux++) count[aux]++;
        }
      }
      for(char aux=0; aux<3; aux++){
        media[aux] = media[aux]/count[aux];
        tmp = media[aux];
        if(tmp>255) tmp = 255;
        switch(aux){
          case 0: p_img->r[j*p_img->width + i] = tmp; break;
          case 1: p_img->g[j*p_img->width + i] = tmp; break;
          case 2: p_img->b[j*p_img->width + i] = tmp; break;
        }
      }
    }
  }
  salvar_imagem("data/paisagem-out-none.jpg", p_img);
  liberar_imagem(p_img);
}

void *tipo_thread(void *args){
  *p_img = abrir_imagem("data/paisagem.jpg");
  /* Gera N_THREADS threads para processamento dos números */
  for(char num_thread=0; num_thread<3; num_thread++){
	  pthread_mutex_lock(&trava);
	  //printf("Gerando thread %d...\n", num_thread);
	  pthread_create(&threads[num_thread], NULL, thread, &num_thread);
	  pthread_mutex_unlock(&trava);
  }
  /* Espera finalização de todos os processos filhos */
  //printf("Todos as threads foram geradas. Esperando...\n");
  for (char num_thread=0; num_thread<3; num_thread++){
	  pthread_join(threads[num_thread], NULL);
	  //printf("Thread %d finalizou\n", num_thread);
  }
  salvar_imagem("data/paisagem-out-thread.jpg", p_img);
  liberar_imagem(p_img);
}

void *tipo_process(void *args){
	*p_img = abrir_imagem("data/paisagem.jpg");
  //printf("p_img->r(65,67) = %f\n", p_img->r[65*p_img->width+67]);
  //printf("p_img->g(65,67) = %f\n", p_img->g[65*p_img->width+67]);
  //printf("p_img->b(65,67) = %f\n", p_img->b[65*p_img->width+67]);
	for(char num_process=0; num_process<3; num_process++){
		//printf("Gerando filho %d...\n", num_process);
		filhos[num_process] = fork();
		//printf("PID do filho %d = %d\n", num_process, filhos[num_process]);
		if(filhos[num_process] == 0){
      //printf("Filho %d\n", num_process);
			switch(num_process){
				case 0:
          //printf("RED\n");
				 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
						for (int j=0; j<(p_img->height); j++) { // percorre linhas
						/* Zera algumas variaveis de interesse */
							media[0] = 0;
						 	count[0] = 0;

						 	for(int k=i-N; k<=i+N; k++){ // percorre colunas
							 	if(k<0||k>(p_img->width)) continue; // Retira bordas
							 	for(int l=j-N; l<=j+N; l++){ // percorre linhas
								 	if(l<0 || l>(p_img->height)) continue; // Retira bordas
							 		media[0] += p_img->r[l*p_img->width+k];
									count[0]++;
							 	}
						 	}
							media[0] = media[0]/count[0];
							tmp = media[0];
							if(tmp>255) tmp = 255;
							p_img->r[j*p_img->width + i] = tmp;
              //printf("p_img->r(%d,%d) = %f\n", j,i, p_img->r[j*p_img->width+i]);
						}
					}
					break;
				case 1:
          //printf("GREEN\n");
				 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
						for (int j=0; j<(p_img->height); j++) { // percorre linhas
						/* Zera algumas variaveis de interesse */
							media[1] = 0;
						 	count[1] = 0;

						 	for(int k=i-N; k<=i+N; k++){ // percorre colunas
							 	if(k<0||k>(p_img->width)) continue; // Retira bordas
							 	for(int l=j-N; l<=j+N; l++){ // percorre linhas
								 	if(l<0 || l>(p_img->height)) continue; // Retira bordas
							 		media[1] += p_img->g[l*p_img->width+k];
									count[1]++;
							 	}
						 	}
							media[1] = media[1]/count[1];
							tmp = media[1];
							if(tmp>255) tmp = 255;
							p_img->g[j*p_img->width + i] = tmp;
              //printf("p_img->g(%d,%d) = %f\n", j,i, p_img->g[j*p_img->width+i]);
						}
					}
					break;
				case 2:
          //printf("BLUE\n");
				 	for (int i=0; i<(p_img->width); i++) { // percorre colunas
						for (int j=0; j<(p_img->height); j++) { // percorre linhas
						/* Zera algumas variaveis de interesse */
							media[2] = 0;
						 	count[2] = 0;

						 	for(int k=i-N; k<=i+N; k++){ // percorre colunas
							 	if(k<0||k>(p_img->width)) continue; // Retira bordas
							 	for(int l=j-N; l<=j+N; l++){ // percorre linhas
								 	if(l<0 || l>(p_img->height)) continue; // Retira bordas
							 		media[2] += p_img->b[l*p_img->width+k];
									count[2]++;
							 	}
						 	}
							media[2] = media[2]/count[2];
							tmp = media[2];
							if(tmp>255) tmp = 255;
							p_img->b[j*p_img->width + i] = tmp;
              //printf("p_img->b(%d,%d) = %f\n", j,i, p_img->b[j*p_img->width+i]);
						}
					}
					break;
				}
			//printf("Saindo do processo %d\n", num_process); 
      //printf("p_img->r(65,67) = %f\n", p_img->r[65*p_img->width+67]);
      //printf("p_img->g(65,67) = %f\n", p_img->g[65*p_img->width+67]);
      //printf("p_img->b(65,67) = %f\n", p_img->b[65*p_img->width+67]);
			exit(0);
		}
		//printf("Pai de numero %d\n", num_process);
	}

	/* Espera finalização de todos os processos filhos */
	//printf("Todos os filhos foram gerados. Esperando...\n");
	for (char num_process=0; num_process<3; num_process++) {
		waitpid(filhos[num_process], NULL, 0);
	}
  //printf("Todos os filhos terminaram.\n");
  //printf("p_img->r(65,67) = %f\n", p_img->r[65*p_img->width+67]);
  //printf("p_img->g(65,67) = %f\n", p_img->g[65*p_img->width+67]);
  //printf("p_img->b(65,67) = %f\n", p_img->b[65*p_img->width+67]);
	
	salvar_imagem("data/paisagem-out-process.jpg", p_img);
  liberar_imagem(p_img);
}

int main() {
  printf("Tipo de execução [(N)one/(T)hread/(P)rocess]: ");
  scanf("%c", &tipo);
 
  printf("Quantidade de execuções: ");
  scanf("%d", &quantidade);

  printf("N: ");
  scanf("%d", &N);

	p_img = (imagem*) mmap(NULL, sizeof(imagem), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);

  switch(tipo){
    case 'N':
			for(int run=0; run<quantidade; run++){
      	printf("run = %d\n", run);
        medir_tempo(tipo_none);
      }
		  break; 
  
    case 'T':
			for(int run=0; run<quantidade; run++){     
      	printf("run = %d\n", run);
				medir_tempo(tipo_thread);
      }
      break;

    case 'P':
			for(int run=0; run<quantidade; run++){
      	printf("run = %d\n", run);
      	medir_tempo(tipo_process);
      }
    	break;
	}
  munmap(p_img, sizeof(imagem));
  return 0;
}
