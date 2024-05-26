#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


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
typedef struct _CQuaternion CQuaternion;
typedef struct _CPhysical CPhysical;
#include "convCVector.h"
#include "convCMatrix.h"
#include "convCSimpleTransform.h"
#include "convCPlaceable.h"
#include "convCQueuedMode.h"
#include "convCCam.h"
#include "convCCamera.h"
#include "convCEntity.h"
#include "convFxManager_c.h"
#include "convCQuaternion.h"
#include "convCPhysical.h"
#include "convCObject.h"


// my useful funcs
double drand_num(double max);
char getVolumeSfx();
void setVolumeSfx(int vol);
void playSoundId(int id, CVector *pos);
void rollTime();
void flyTo(float tx, float ty, float tz, float heading, int interior, int need_detect_land);
void setCameraFromToFov(float sx, float sy, float sz, float tx, float ty, float tz, float fov);
void setCameraFromState(CCamera *state);
CVector *getPlayerVector();
void *getPlayerCped();
void cpedSetHeading(void *cped, float angle);
void cpedSetVisibility(void *cped, int state);
void cpedSetHealth(void *cped, float h);
void setAspectRatio(float aspect);
void setGameFPSLimit(int fps);
void setWindynessForCurrentWeather(float val);
void setDrawingDistance(float dist);
void waitNFrames(int n);
void setGravity(float n);
void restoreGravity();



// game functions
extern void addTwoStars();
extern void MessageJumpQ(char *text, int time, unsigned short flag, bool addToPreviousBrief);
extern void forceWeatherNow(short weatherType);
extern void __thiscall reportMissionAudioEvent(void *t, int id, CVector *coords);
extern void __thiscall setEffectsMasterVolume(void *t, char volume);

extern void __cdecl CPointLights__AddLight(char type,
        float x,
        float y,
        float z,
        float dir_x,
        float dir_y,
        float dir_z,
        float radius,
        float r,
        float g,
        float b,
        char fogType,
        char generateExtraShadows,
        int attachedTo);

extern int CTheScripts__AddScriptSearchLight(float startX, float startY, float startZ, CEntity *entity, float targetX, float targetY, float targetZ, float targetRadius, float baseRadius);
extern void __cdecl AddAnExtraDirectionalLight(void *RwWorld, float x, float y, float z, float red, float green, float blue);
void __cdecl CCoronas__RegisterCorona(
        int id,
        void *entity,
        unsigned __int8 r,
        unsigned __int8 g,
        unsigned __int8 b,
        unsigned __int8 alpha,
        CVector *pos,
        float radius,
        float farClip,
        void *texture,
        char flare,
        char enableReflection,
        char checkObstacles,
        int notUsed,
        float angle,
        char longDistance,
        float nearClip,
        char fadeState,
        float fadeSpeed,
        char onlyFromBelow,
        char reflectionDelay);


extern int __cdecl CShadows__StoreStaticShadow(unsigned int id, unsigned char type, void* texture, CVector* posn,
float frontX, float frontY, float sideX, float sideY, short intensity,
unsigned char red, unsigned char green, unsigned char blue, float zDistance,
float scale, float drawDistance, char temporaryShadow, float upDistance);

extern void __cdecl CPickups__RemovePickUp(uint16_t handle);

extern void __cdecl CGameLogic__RestorePlayerStuffDuringResurrection(void *player, float x, float y, float z, float angle);
extern void __cdecl CStreaming__RemoveBuildingsNotInArea(int AreaCode);

extern void __thiscall CPlaceable__SetHeading(CPlaceable *this, float angle);
extern float __thiscall CPlaceable__GetHeading();
extern double __cdecl FindPlayerHeading(int playerId);

extern __cdecl void requestCollision(const CVector *vec, int where);
extern __cdecl float findGroundZForCoord(float x, float y);

extern double __cdecl CWorld__FindGroundZForCoord(float x, float y);
extern int __cdecl CWorld__TestSphereAgainstWorld(CVector sphereCenter, float sphereRadius, CEntity *ignoreEntity, char buildings, char vehicles, char peds, char objects, char dummies, char doIgnoreCameraCheck);
extern void __cdecl CStreaming__StreamZoneModels(const CVector *posn);

extern unsigned char* sunCoreBlue;
extern unsigned char* sunCoreGreen;
extern unsigned char* sunCoreRed;

extern float *CRenderer_ms_lodDistScale;
extern float *CRenderer_ms_fCameraHeading;


extern void __cdecl CStreaming__RequestModel(signed int dwModelId, int flags);
extern void __cdecl CStreaming__LoadAllRequestedModels(char bOnlyPriorityRequests);
extern void __cdecl CStreaming__SetMissionDoesntRequireModel(int index);

extern void __cdecl CTimer__Resume();
extern void __cdecl CTimer__Suspend();
extern void* __cdecl CObject__Create(int modelId);
extern void __cdecl CWorld__Add(CEntity *entity);
extern CObject* __stdcall CPool_CObject___AtHandle(int handle);
extern void* __thiscall CPlaceable__setPosition(CPlaceable *this, CVector *pos);

volatile extern int *CTimer_m_FrameCounter;
volatile extern float *CTimer_game_FPS;

extern char *CTheScripts__bDisplayHud;
extern char *CHud__bScriptDontDisplayRadar;
extern float *CHud__textBoxNumLines;
extern int *CHud__helpMessageState;


extern uint32_t *CStreaming__memoryAvailable;

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
extern float *timeStepMin;
extern float *timeStepNonClippedMin;

extern char *codePause;
extern char *userPause;

volatile extern unsigned short *clockSeconds;
volatile extern unsigned char *clockMinutes;
volatile extern unsigned char *clockHours;
extern CCamera *theCamera;
extern float *GAME_GRAVITY;


#endif
