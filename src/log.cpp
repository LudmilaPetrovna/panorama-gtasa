#include <stdio.h>
#include <time.h>

#include "log.h"


FILE *logme_init(){
static FILE *f=NULL;
static char filename[256];

if(!f){
sprintf(filename,"logs-%d.txt",(int)time(0));
f=fopen(filename,"w");
}

return(f);
}




