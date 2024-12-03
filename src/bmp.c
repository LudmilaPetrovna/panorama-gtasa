#include <stdio.h>
#include <stdint.h>

#define BMP_HEADER_LENGTH 54
#define uint32_t_to_bytes(x) (uint8_t)(x&0xFF),(uint8_t)((x>>8)&0xFF),(uint8_t)((x>>16)&0xFF),(uint8_t)((x>>24)&0xFF)

void bmp_write_rgba(FILE *out, uint32_t *pixels, int width, int height){

int pixelscount=width*height;
int bitmapsize=pixelscount*4;
int filesize=bitmapsize+0x36;

height=-height;

uint8_t header[BMP_HEADER_LENGTH]={
'B','M',uint32_t_to_bytes(filesize),
0,0,0,0, // reserved, must be 0
0x36,0,0,0, // offset to bitmap
0x28,0,0,0, // header size
uint32_t_to_bytes(width),uint32_t_to_bytes(height),
1,0, // color planes, must be 1
0x20,0, // 32-bits per pixel
0,0,0,0, // no compression
uint32_t_to_bytes(bitmapsize),
0,0,0,0,0,0,0,0, // resolution
0,0,0,0,0,0,0,0 // palette size and important colors (0=all important)
};

fwrite(header,1,BMP_HEADER_LENGTH,out);
fwrite(pixels,1,bitmapsize,out);

}


void bmp_write_gray8(FILE *out, uint8_t *pixels, uint32_t *pal, int width, int height){

int pad=width%4;
int pixelscount=width*height;
int filesize=pixelscount+0x36+256*4;
int w;
uint32_t zero=0;

uint8_t header[BMP_HEADER_LENGTH]={
'B','M',uint32_t_to_bytes(filesize),
0,0,0,0, // reserved, must be 0
0x36,4,0,0, // offset to bitmap
0x28,0,0,0, // header size
uint32_t_to_bytes(width),uint32_t_to_bytes(-height),
1,0, // color planes, must be 1
0x8,0, // 8-bits per pixel
0,0,0,0, // no compression
uint32_t_to_bytes(pixelscount),
0,0,0,0,0,0,0,0, // resolution
0,0,0,0,0,0,0,0 // palette size and important colors (0=all important)
};

if(!pal){
uint32_t gray[256];
int q;
for(q=0;q<256;q++){
gray[q]=(q<<16)|(q<<8)|q;
}
pal=gray;
}

fwrite(header,1,BMP_HEADER_LENGTH,out);
fwrite(pal,1,256*4,out);
for(w=0;w<height;w++){
fwrite(pixels+w*width,1,width,out);
if(pad){
fwrite(&zero,1,4-pad,out);
}
}
}

