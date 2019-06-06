#include<stdio.h>
#include<stdint.h>

typedef struct Image{
  int width,height;
  int maxval;
  uint8_t* val;
}Image;

void readmodpgm(char* filename, Image* modified);
void readreconpgm(char* filename, Image* recosnt);
void addCb(Image* modified, Image* reconst, Image* blkval);
void addCr(Image* modified, Image* reconst, Image* inblkval);
void writeyuv(char* filename, Image* reconst, Image* blkval, Image* inblkval);
