#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <windows.h>
#include <pthread.h>
#include <stdlib.h>
#include "ApiGtaSA.h"


HANDLE hDir;
OVERLAPPED overlap;
uint8_t direvents[2048];

/*
__attribute__((stdcall)) void on_file_changed(long unsigned int a,  long unsigned int b,  struct _OVERLAPPED *o){
printf("something changed\n");
}
*/

void *wait_for_events(void *unused){
FILE_NOTIFY_INFORMATION *e;
DWORD r;
char filename[256];
int filename_len;
char data[256];

float sx,sy,sz,tx,ty,tz,fov,sum,realsum;

while(1){
r=WaitForSingleObject(overlap.hEvent,0);
e=(FILE_NOTIFY_INFORMATION*)direvents;

if(r==WAIT_OBJECT_0){
      DWORD bytes_transfer;
      GetOverlappedResult(hDir, &overlap, &bytes_transfer, FALSE);
filename_len=wcstombs(filename,e->FileName,e->FileNameLength);

//printf("changed: %s\n",filename);
FILE *d=fopen("v:\\gta\\filerpc\\gamecam.txt","r");
data[fread(data,1,sizeof(data),d)]=0;
fclose(d);
sscanf(data,"%f;%f;%f;%f;%f;%f;%f;%f",&sx,&sy,&sz,&tx,&ty,&tz,&fov,&sum);
realsum=sx+sy+sz+tx+ty+tz+fov;
if(fabs(realsum-sum<1.0)){

setCameraFromToFov(sx,sy,sz,tx,ty,tz,fov);

float oz=findGroundZForCoordByFile(tx,ty);
CVector *player_pos=getPlayerVector();
player_pos->x=tx;
player_pos->y=ty;
player_pos->z=oz;


}
if(!ReadDirectoryChangesW(hDir, direvents, sizeof(direvents), FALSE, FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE,NULL,&overlap,NULL)){
exit(1);
}
}
}

}

void start_file_rpc(){

memset(&overlap,0,sizeof(overlap));
overlap.hEvent=CreateEvent(NULL,FALSE,0,NULL);

hDir=CreateFile("v:\\gta\\filerpc\\",FILE_LIST_DIRECTORY,FILE_SHARE_READ/*|FILE_SHARE_WRITE|FILE_SHARE_DELETE*/,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,NULL);
if(hDir==INVALID_HANDLE_VALUE){

printf("Can't open file rpc directory!\n");
exit(1);
}

if(!ReadDirectoryChangesW(hDir, direvents, sizeof(direvents), FALSE, FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SIZE,NULL,&overlap,NULL)){
printf("Can't monitor directory events\n");
exit(1);
}

pthread_t w;
pthread_create(&w,NULL,wait_for_events,NULL);
}
