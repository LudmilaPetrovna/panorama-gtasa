#include <stdio.h>
#include <time.h>



void logme(char *msg){
static FILE *f=NULL;
if(!f){f=fopen("logs.txt","w");}

fprintf(f,"%d: %s\n",(int)time(0),msg);
fflush(f);

}

