#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include "yuv.h"

int main(int argc,char* argv[])
{
  int i = atoi(argv[4]);
  char filename[100];

  if(argc != 5){
    fprintf(stderr,"Usage:%s modified.pgm reconst.pgm outputname QP\n",argv[0]);
    return (1);
  }
  sprintf(filename,"%s%d.yuv", argv[3], i);
  Image* modified = (Image*)calloc(1,sizeof(Image));
  Image* reconst = (Image*)calloc(1,sizeof(Image));
  Image* blkval = (Image*)calloc(1,sizeof(Image));
  Image* inblkval = (Image*)calloc(1,sizeof(Image));
  readmodpgm(argv[1],modified);
  readreconpgm(argv[2],reconst);
  addCb(modified,reconst,blkval);
  addCr(modified,reconst,inblkval);
  writeyuv(filename,reconst,blkval,inblkval);
  return 0;
}
