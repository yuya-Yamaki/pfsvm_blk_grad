#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include "yuv.h"

void readmodpgm(char* filename,Image* modified)
{
  int i;
  int width,height,maxval;
  char comment[256];
  char header[3];
  FILE* fp = fopen(filename,"rb");

  fread(header,sizeof(char),3,fp);
  if(header[0] != 'P' || header[1] != '5'){
    printf("Please input pgm file\n");
  }

  fread(comment,sizeof(char),1,fp);
  if(comment[0] == '#'){
    for(i = 0; i < 256; i++){
      fread(&comment[i],sizeof(char),1,fp);
      if(comment[i] == '\n')break;
    }
  }
  else{
    fclose(fp);
    fp = fopen(filename,"rb");
    fread(header,sizeof(char),3,fp);
  }

  fscanf(fp,"%d %d",&width,&height);
  fscanf(fp,"%d",&maxval);

  modified->width = width;
  modified->height = height;
  modified->maxval = maxval;
  modified->val = (uint8_t*)calloc(modified->width * modified->height,sizeof(uint8_t));

  fread(modified->val,sizeof(uint8_t),modified->width * modified->height,fp);

  fclose(fp);
}

void readreconpgm(char* filename,Image* reconst)
{
  int i;
  int width,height,maxval;
  char comment[256];
  char header[3];
  FILE* fp = fopen(filename,"rb");

  fread(header,sizeof(char),3,fp);
  if(header[0] != 'P' || header[1] != '5'){
    printf("Please input pgm file\n");
  }

  fread(comment,sizeof(char),1,fp);
  if(comment[0] == '#'){
    for(i = 0; i < 256; i++){
      fread(&comment[i],sizeof(char),1,fp);
      if(comment[i] == '\n')break;
    }
  }
  else{
    fclose(fp);
    fp = fopen(filename,"rb");
    fread(header,sizeof(char),3,fp);
  }

  fscanf(fp,"%d %d",&width,&height);
  fscanf(fp,"%d",&maxval);

  reconst->width = width;
  reconst->height = height;
  reconst->maxval = maxval;
  reconst->val = (uint8_t*)calloc(reconst->width * reconst->height,sizeof(uint8_t));

  fread(reconst->val,sizeof(uint8_t),reconst->width * reconst->height,fp);
  fclose(fp);
}

void addCr(Image* modified, Image* reconst, Image* subval)
{
  int i;
  subval->val = (uint8_t*)calloc(reconst->width * reconst->height,sizeof(uint8_t));

  for(i = 0; i < reconst->width * reconst->height; i++){
    subval->val[i] = 50 * (modified->val[i] - reconst->val[i]);
    if(subval->val[i] == 0)subval->val[i] = 128;
  }
}

void writeyuv(char* filename, Image* reconst, Image* subval)
{
  FILE* fp = fopen(filename,"wb");
  int i;
  int zero = 128;

  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&reconst->val[i],sizeof(uint8_t),1,fp);
    }
  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&zero,sizeof(uint8_t),1,fp);
    }
  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&subval->val[i],sizeof(uint8_t),1,fp);
    }


  fclose(fp);
}
