#include <windows.h>



typedef struct{
char vidcam;
char liftcam;
char grain;
char cutscene;
char nightvision;
char infravision;
char heat;
char darkness;
char cctv;
char rain;
char smokey;
char underwater;
//_CWeather_TrafficLightsBrightness
}FILTERS_PARAMS;


typedef struct{
double fps_on_screenshots;
double fps_on_precache;
double fps_on_game;
int memory_available;
int sound_on_finish;
int exit_on_finish;
} SESSION_PARAMS;

DWORD WINAPI MyASIThread(LPVOID lpParam);
double drand();
