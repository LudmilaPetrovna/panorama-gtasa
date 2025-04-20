#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "ApiGtaSA.h"
#include "log.h"


int is_cam_recoding=0;
FILE *cam_rec_file=NULL;

void on_frame_camrecord(){
logme("on_frame_");

if(!is_cam_recoding){
if(cam_rec_file){
fclose(cam_rec_file);
cam_rec_file=NULL;
}
return;
}

logme("on_frame_pre file");
if(!cam_rec_file){
char filename[256];
sprintf(filename,"camrec-%d.txt",(int)time(0));
cam_rec_file=fopen(filename,"w");
}

logme("on_frame_pre afile");

if(cam_rec_file){
CVector *cj=getPlayerVector();
fprintf(cam_rec_file,"%.3f,%.3f,%.3f\n",cj->x,cj->y,cj->z);
}

}

