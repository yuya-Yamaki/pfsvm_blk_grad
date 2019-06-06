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

void addCb(Image* modified,Image* reconst,Image* blkval)
{

  int i,j,x,y,w,h,cux,cuy,bx,by;
  char tmp[1];
  FILE* TUinfo;
  int width;
  int u,v;
  int z;

  width = reconst->width;
  blkval->val = (uint8_t*)calloc(reconst->width * reconst->height,sizeof(uint8_t));

  TUinfo = fopen("TUinfo1.log","rb");

  while(fscanf(TUinfo,"%s%d%d%d%d",tmp,&x,&y,&w,&h) != EOF){
    if(tmp[0] == 'C'){
      cux = x;
      cuy = y;
    }
    else{
      bx = cux + x;
      by = cuy + y;
      //block境界
      for(i = by; i < by + h; i++){
        for(j = bx; j < bx + w; j++){
          u = j - bx;
          v = i - by;
          z = (width*by)+bx+(width*v)+u;
          if(  (j == bx + w - 1 && j != reconst->width-1)
             ||(j == bx && j != 0)
             ||(i == by + h - 1 && i != reconst->height-1)
             ||(i == by && i != 0))
             {
               blkval->val[z] = 50 * (modified->val[z] - reconst->val[z]);
               if(blkval->val[z] == 0)blkval->val[z] = 128;
             }//if
             else{
               blkval->val[z] = 128;
             }//else
        }
      }//blockscan
    }//else
  }//while
  fclose(TUinfo);
}//addCb

void addCr(Image* modified,Image* reconst,Image* inblkval){

  int i,j,x,y,w,h,cux,cuy,bx,by;
  char tmp[1];
  FILE* TUinfo;
  int width;
  int u,v;
  int z;

  width = reconst->width;
  inblkval->val = (uint8_t*)calloc(reconst->width * reconst->height,sizeof(uint8_t));

  TUinfo = fopen("TUinfo1.log","rb");

  while(fscanf(TUinfo,"%s%d%d%d%d",tmp,&x,&y,&w,&h) != EOF){
    if(tmp[0] == 'C'){
      cux = x;
      cuy = y;
    }
    else{
      bx = cux + x;
      by = cuy + y;
      //block内
      for(i = by; i < by + h; i++){
        for(j = bx; j < bx + w; j++){
          u = j - bx;
          v = i - by;
          z = (width*by)+bx+(width*v)+u;
          if( ((j != bx + w - 1)
             &&(j != bx)
             &&(i != by + h - 1)
             &&(i != by))
             ||(j == 0 && i == 0)
             ||(j == reconst->width-1 && i == 0)
             ||(j == 0 && i == reconst->height-1)
             ||(j == reconst->width-1 && i == reconst->height-1)
             ||(j == 0 && i != by + h -1 && i != by)
             ||(i == 0 && j != bx + w -1 && j != bx)
             ||(j == reconst->width-1 && i != by + h - 1 && i != by)
             ||(i == reconst->height-1 && j != bx + w - 1 && j != bx))
             {
               inblkval->val[z] = 50 * (modified->val[z] - reconst->val[z]);
               if(inblkval->val[z] == 0)inblkval->val[z] = 128;
             }//if
             else{
               inblkval->val[z] = 128;
             }//else
        }
      }//blockscan
    }//else
  }//while
  fclose(TUinfo);
}//addCr

void writeyuv(char* filename, Image* reconst, Image* blkval, Image* inblkval)
{
  FILE* fp = fopen(filename,"wb");
  int i;
  int zero = 128;

  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&reconst->val[i],sizeof(uint8_t),1,fp);
    }
  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&blkval->val[i],sizeof(uint8_t),1,fp);
    }
  for(i = 0; i < reconst->width * reconst->height; i++){
      fwrite(&inblkval->val[i],sizeof(uint8_t),1,fp);
    }


  fclose(fp);
}
