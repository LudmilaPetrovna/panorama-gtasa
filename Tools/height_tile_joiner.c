#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


float *map;
uint8_t *pic;


void main(){

map=malloc(6000*6000*sizeof(float));
pic=malloc(6000*6000);
float *tile=malloc(200*200*4);
FILE *f;
char filename[256];
int q,w,a,s;
for(w=0;w<30;w++){
for(q=0;q<30;q++){
sprintf(filename,"heightmap-%d-%d.bin",q,w);
printf("Reading file %s...\n",filename);
f=fopen(filename,"rb");
if(!f){continue;}
fread(tile,1,200*200*4,f);
fclose(f);

for(s=0;s<200;s++){
for(a=0;a<200;a++){
map[a+q*200+(w*200+s)*6000]=tile[a+s*200];
}
}


}
}
f=fopen("heightmap.bin","wb");
fwrite(map,1,6000*6000*4,f);
fclose(f);

float valmin=0,valmax=0,val;
for(w=0;w<6000;w++){
for(q=0;q<6000;q++){
val=map[q+w*6000];
if(val>valmax){valmax=val;}
if(val<valmin){valmin=val;}
}
}
float mult=256.0/(valmax-valmin);
printf("Height in %.5f .. %.5f, mult: %.5f\n",valmin,valmax,mult);
for(w=0;w<6000;w++){
for(q=0;q<6000;q++){
val=(map[q+w*6000]-valmin)*mult;
if(val>255){val=255;}
if(val<0){val=0;}
pic[q+w*6000]=(int)val;
}
}

f=fopen("heightmap.pic","wb");
fwrite(pic,1,6000*6000,f);
fclose(f);

system("ffmpeg -f rawvideo -s 6000x6000 -pix_fmt gray -i heightmap.pic -s 300x300 -pix_fmt rgb24 -y heightmap.png");
system("ffmpeg -f rawvideo -s 6000x6000 -pix_fmt grayf32le -i heightmap.bin -s 300x300 -pix_fmt rgb24 -y heightmap-0.png");
}
