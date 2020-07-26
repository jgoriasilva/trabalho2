
#include <imageprocessing.h>
#define N 15

int main() {
  imagem img;
  img = abrir_imagem("data/cachorro.jpg");
  unsigned int tmp;
  int media[3], count[3];

  for (int i=0; i<(img.width); i++) { // percorre colunas
    for (int j=0; j<(img.height); j++) { // percorre linhas
      
      /* Zera algumas variaveis de interesse */
      media[0] = 0;
      media[1] = 0;
      media[3] = 0;
      count[0] = 0;
      count[1] = 0;
      count[2] = 0;

      for(int k=i-N; k<=i+N; k++){ // percorre colunas
        if(k<0||k>(img.width)) continue; // Retira bordas
        for(int l=j-N; l<=j+N; l++){ // percorre linhas
          if(l<0 || l>(img.height)) continue; // Retira bordas
          media[0] += img.r[l*img.width+k];
          media[1] += img.g[l*img.width+k];
          media[2] += img.b[l*img.width+k];
          count[0]++;       
          count[1]++;       
          count[2]++;       
        }
      }
      media[0] = media[0]/count[0]; // Calcula efetivamente a media para o canal R
      tmp = media[0];
      if (tmp > 255) tmp=255;
      img.r[j*img.width + i] = tmp;
      media[1] = media[1]/count[1]; // Calcula efetivamente a media para o canal G
      tmp = media[1];
      img.g[j*img.width + i] = tmp;
      if (tmp > 255) tmp=255;
      media[2] = media[2]/count[2]; // Calcula efetivamente a media para o canal B
      tmp = media[2];
      if (tmp > 255) tmp=255;
      img.b[j*img.width + i] = tmp;
    }
  }
  salvar_imagem("data/cachorro-out.jpg", &img);
  liberar_imagem(&img);
  return 0;
}
