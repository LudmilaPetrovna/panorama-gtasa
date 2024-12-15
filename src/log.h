#ifndef LOGME_H
#define LOGME_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

FILE *logme_init();
#define logme(x...) {FILE *f=logme_init();fprintf(f,"%d: ",(int)time(0));fprintf(f,x);fprintf(f,"\n");fflush(f);}

#ifdef __cplusplus
}
#endif
#endif
