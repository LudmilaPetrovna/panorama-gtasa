#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "ApiGtaSA.h"
#include "log.h"

#define MIN_DIST 0.1
int is_cam_recoding=0;
FILE *cam_rec_file=NULL;

static float lastpos[3]={-5000,-5000,-5000};

void on_frame_camrecord(){
float dist,dx,dy,dz;

if(!is_cam_recoding){
if(cam_rec_file){
fclose(cam_rec_file);
cam_rec_file=NULL;
}
return;
}

if(!cam_rec_file){
char filename[256];
sprintf(filename,"camrec-%d.txt",(int)time(0));
cam_rec_file=fopen(filename,"w");
}

if(cam_rec_file){
CVector *cj=getPlayerVector();
dx=lastpos[0]-cj->x;
dy=lastpos[1]-cj->y;
dz=lastpos[2]-cj->z;
dist=sqrt(dx*dx+dy*dy+dz*dz);
if(dist<MIN_DIST){
return;
}
fprintf(cam_rec_file,"%.3f,%.3f,%.3f\n",cj->x,cj->y,cj->z);
lastpos[0]=cj->x;
lastpos[1]=cj->y;
lastpos[2]=cj->z;
}

}

