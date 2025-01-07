
#ifndef WEBSERVER_H
#define WEBSERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
uint32_t addr;
int type;
uint32_t old;
}SUBS_ITEM;

typedef struct{
char label[16];
int type;
void *addr;
float min;
float max;
float val;
int streamable;
void *apply;
}MENUITEM;


void webserver_start(void*(*menu)(void));

#ifdef __cplusplus
}
#endif
#endif
