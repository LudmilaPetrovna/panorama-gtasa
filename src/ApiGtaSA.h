
#ifndef API_GTA_SAN_ANDREAS
#define API_GTA_SAN_ANDREAS


typedef struct _CVector CVector;
typedef struct _CMatrix CMatrix;
typedef struct _CSimpleTransform CSimpleTransform;
typedef struct _CPlaceable CPlaceable;
typedef struct _CQueuedMode CQueuedMode;
typedef struct _CCam CCam;
typedef struct _CCamera CCamera;
typedef struct _CEntity CEntity;
typedef struct _FxManager_c FxManager_c;
#include "convCVector.h"
#include "convCMatrix.h"
#include "convCSimpleTransform.h"
#include "convCPlaceable.h"
#include "convCQueuedMode.h"
#include "convCCam.h"
#include "convCCamera.h"
#include "convCEntity.h"
#include "convFxManager_c.h"


// my useful funcs
void setVolume(int vol);
void playSoundId(int id, CVector *pos);
void rollTime();
void flyTo(float tx, float ty, float tz, float heading);
void setCameraFromToFov(float sx, float sy, float sz, float tx, float ty, float tz, float fov);
void *getPlayerCped();
void cpedSetHeading(void *cped, float angle);
void setAspectRatio(float aspect);
void setGameFPSLimit(int fps);
void setWindynessForCurrentWeather(float val);
void setDrawingDistance(float dist);
void waitNFrames(int n);



// game functions
extern void addTwoStars();
extern void MessageJumpQ(char *text, int time, unsigned short flag, bool addToPreviousBrief);
extern void forceWeatherNow(short weatherType);
extern void __thiscall reportMissionAudioEvent(void *t, int id, CVector *coords);
extern void __thiscall setEffectsMasterVolume(void *t, char volume);

extern void __cdecl CGameLogic__RestorePlayerStuffDuringResurrection(void *player, float x, float y, float z, float angle);
extern void __cdecl CStreaming__RemoveBuildingsNotInArea(int AreaCode);

extern void __thiscall CPlaceable__SetHeading(CPlaceable *this, float angle);
extern float __thiscall CPlaceable__GetHeading();
extern double __cdecl FindPlayerHeading(int playerId);

extern __cdecl void requestCollision(const CVector *vec, int where);
extern __cdecl float findGroundZForCoord(float x, float y);

extern unsigned char* sunCoreBlue;
extern unsigned char* sunCoreGreen;
extern unsigned char* sunCoreRed;

extern float *CRenderer_ms_lodDistScale;
extern float *CRenderer_ms_fCameraHeading;



extern char *CTheScripts__bDisplayHud;
extern char *CHud__bScriptDontDisplayRadar;



// don't work for me
extern void __cdecl JPegCompressScreenToFile(void *rwcamera, const char *filename);

extern float *weatherInterpolationValue;
extern short *weatherForcedType;
extern short *weatherNewType;
extern short *weatherOldType;

extern float *cameraFov; // by Vital (vitalrus) at plugin-sdk-ru (https://discord.gg/QEesDGb)
extern char *cameraFovLock;
extern int *brightness; // by CE
extern int *darkness;
extern char *darknessEnable;

extern int *LastScreenWidth;
extern int *LastScreenHeight;
extern int *screenHudMenuWidth;
extern int *screenHudMenuHeight;

extern float *timeScale;
extern float *timeStep;
extern char *codePause;
extern char *userPause;

extern unsigned short *clockSeconds;
extern unsigned char *clockMinutes;
extern unsigned char *clockHours;
extern CCamera *theCamera;
extern float *GAME_GRAVITY;


#endif
