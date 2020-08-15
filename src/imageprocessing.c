
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "imageprocessing.h"

#include <FreeImage.h>

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

int x, y;

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } /*else {
    printf("Arquivo lido corretamente!\n");
   }*/

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  //I.r = malloc(sizeof(float) * x * y);
  I.r = (float*) mmap(NULL, sizeof(float) * x * y, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
  //I.g = malloc(sizeof(float) * x * y);
  I.g = (float*) mmap(NULL, sizeof(float) * x * y, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);
  //I.b = malloc(sizeof(float) * x * y);
  I.b = (float*) mmap(NULL, sizeof(float) * x * y, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, 0, 0);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = color.rgbRed;
      I.g[idx] = color.rgbGreen;
      I.b[idx] = color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  munmap(I->r, sizeof(float) * x * y);
  munmap(I->g, sizeof(float) * x * y);
  munmap(I->b, sizeof(float) * x * y);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  //printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = I->r[idx];
      color.rgbGreen = I->g[idx];
      color.rgbBlue = I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

