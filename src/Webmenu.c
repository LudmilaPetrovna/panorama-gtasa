#include <stdint.h>
#include <math.h>

#include "ApiGtaSA.h"
#include "Webserver.h"









float clock_update=6;

void clock_apply(){

uint8_t hours=floor(clock_update);
uint8_t minutes=(clock_update-hours)*60.0;
uint16_t seconds=(clock_update-hours)*3600.0-minutes*60;

*clockHours=hours;
*clockMinutes=minutes;
*clockSeconds=seconds;
}


typedef struct{
int next;
int old;
float transition;
}WEATHER;

WEATHER weather_update={0,0,0};

void weather_apply(){
*weatherInterpolationValue=weather_update.transition;
//*weatherForcedType=(short *)0xC81318;
*weatherNewType=weather_update.next;
*weatherOldType=weather_update.old;
}


float move_map_x,move_map_y;
void move_CJ(){
float oz=findGroundZForCoordByFile(move_map_x,move_map_y);
CVector *player_pos=getPlayerVector();
player_pos->x=move_map_x;
player_pos->y=move_map_y;
player_pos->z=oz;
}


void *menu(){
static MENUITEM menu[]={
{"World",'T',123},
{"Clock",'f',&clock_update,.apply=&clock_apply,.min=0,.max=24,.val=2,.streamable=1},
{"Weather_old",'I',&weather_update.old,.apply=&weather_apply,.min=0,.max=24},
{"Weather_next",'I',&weather_update.next,.apply=&weather_apply,.min=0,.max=24},
{"Weather_tran",'f',&weather_update.transition,.apply=&weather_apply,.min=0,.max=1},
{"FunnyCheats",'T',123},
{"Weapon_Set_1",'b',(void*)0x969130},
{"Weapon_Set_2",'b',(void*)0x969131},
{"Weapon_Set_3",'b',(void*)0x969132},
{"Helth+Arm+250K",'b',(void*)0x969133},
{"EveryoneisArm",'b',(void*)0x969140},
{"AllGreenLights",'b',(void*)0x96914E},
{"AggressDrivers",'b',(void*)0x96914F},
{"Pink_Traffic",'b',(void*)0x969150},
{"CJ_is_Fat",'b',(void*)0x969154},
{"Max_Muscle",'b',(void*)0x969155},
{"Slut_Magnet",'b',(void*)0x96915D},
{"Inf_Health",'b',(void*)0x96916D},
{"Inf_Oxygen",'b',(void*)0x96916E},
{"Inf_AmmoNoRel",'b',(void*)0x969178},
{"AimingDriving",'b',(void*)0x969179},
{"MaxSexAppeal",'b',(void*)0x969180},
{"MaxStamina",'b',(void*)0x969181},
{"HitmanInAll",'b',(void*)0x969183},
{"Map",'T',12345},
{"move_map_x",'f',&move_map_x,.min=-3000,.max=3000,.apply=&move_CJ},
{"move_map_y",'f',&move_map_y,.min=-3000,.max=3000,.apply=&move_CJ},
{0,0,0}
};
return(&menu);
}

void webmenu_start(){
webserver_start(&menu);

}