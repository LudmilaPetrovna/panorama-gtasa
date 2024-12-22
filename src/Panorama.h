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
/*
int *sunChangeBrightnessImmediately=(int*)0xC3E034;
unsigned int *sunMoonSize=(unsigned int *)0x8D4B60;
float *sunLightsMult=(float*)0x8D4B5C;
char *sunBlockedByClouds=(char *)0xC3E030;
*CTimeCycle_m_FogReduction_=1024.0*drand();
*CTimeCycle_m_fShadowDisplacementY_=1.0*drand();
float *_CWeather_TrafficLightsBrightness=(float*)0xC812A8;
float *_CWeather_HeatHaze=(float*)0xC812DC;
float *_CWeather_SunGlare=(float*)0xC812E0;
float *_CWeather_Rainbow=(float*)0xC812E4;
float *_CWeather_Wavyness=(float*)0xC812E8;
float *_CWeather_WindClipped=(float*)0xC812EC;
float *_CWeather_Wind=(float*)0xC812F0;
float *_CWeather_Sandstorm=(float*)0xC812F4;
float *_CWeather_Rain=(float*)0xC81324;
float *_CWeather_InTunnelness=(float*)0xC81334;
float *_CWeather_WaterDepth=(float*)0xC81330;
float *_CWeather_UnderWaterness=(float*)0xC8132C;
float *_CWeather_ExtraSunnyness=(float*)0xC812F8;
float *_CWeather_Foggyness_SF=(float*)0xC812FC;
float *_CWeather_Foggyness=(float*)0xC81300;
float *_CWeather_CloudCoverage=(float*)0xC81304;
float *_CWeather_WetRoads=(float*)0xC81308;
*_CWeather_CurrentRainParticleStrength=1024.0*drand();
*_CWeather_Wind=0;
*_CWeather_LightningDuration=0;*/

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
