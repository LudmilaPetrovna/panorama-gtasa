#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"
#include "Webserver.h"









float new_time;

void on_new_time(){

uint8_t hours=floor(new_time);
uint8_t minutes=(new_time-hours)*60.0;
uint16_t seconds=(new_time-hours)*3600.0-minutes*60;

*clockHours=hours;
*clockMinutes=minutes;
*clockSeconds=seconds;
}


void *menu(){
static MENUITEM menu[]={
{"World",'T',123},
{"Time",'f',&new_time,.apply=&on_new_time,.min=2,.max=27,.val=2,.streamable=1},
{"Time_apply",'t',&on_new_time},
{0,0,0}
};
return(&menu);
}

void webmenu_start(){
webserver_start(&menu);

}