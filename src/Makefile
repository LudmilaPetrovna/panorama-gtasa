CC=/usr/bin/i686-w64-mingw32-gcc
CXX=/usr/bin/i686-w64-mingw32-g++
CFLAGS=-Wall
WINLINK=-lgdi32 -luser32 -lwinmm -ldxguid -ld3dx9
OUTNAME=Panorama.asi
C_SRCFILES=ApiGtaSA.c Panorama.c EntryPoint.c Freeze.c hilbert.c toolbox.c SpawnObject.c bmp.c
CXX_SRCFILES=Screenshot.cpp

all:
	$(CXX) $(CFLAGS) -c $(CXX_SRCFILES)
	$(CC) $(CFLAGS) Screenshot.o  $(WINLINK) -Wall -ggdb3 -shared -static-libgcc -static-libstdc++ $(C_SRCFILES) -o $(OUTNAME) -Wl,-gc-sections,-defsym=_addTwoStars=0x00438E90,-defsym=_MessageJumpQ=0x69F1E0,-defsym=_forceWeatherNow=0x72A4F0,-defsym=_requestCollision=0x410C00,-defsym=_findGroundZForCoord=0x569660,-defsym=_reportMissionAudioEvent=0x507340,-defsym=_setEffectsMasterVolume=0x506E10,-defsym=_CPlaceable__SetHeading=0x43E0C0,-defsym=_CPlaceable__GetHeading=0x441DB0,-defsym=_FindPlayerHeading=0x56E450,-defsym=_JPegCompressScreenToFile=0x5D0820,-defsym=_CGameLogic__RestorePlayerStuffDuringResurrection=0x442060,-defsym=_CStreaming__RemoveBuildingsNotInArea=0x4094B0,-defsym=_CWorld__FindGroundZForCoord=0x569660,-defsym=_CWorld__TestSphereAgainstWorld=0x569E20,-defsym=_CStreaming__StreamZoneModels=0x40A560,-defsym=_CPointLights__AddLight=0x7000E0,-defsym=_CTheScripts__AddScriptSearchLight=0x493000,-defsym=_AddAnExtraDirectionalLight=0x735840,-defsym=_CCoronas__RegisterCorona=0x6FC180,-defsym=_CShadows__StoreStaticShadow=0x70BA00,-defsym=_CPickups__RemovePickUp=0x4573D0,-defsym=_CStreaming__RequestModel=0x4087E0,-defsym=_CStreaming__LoadAllRequestedModels=0x40EA10,-defsym=_CStreaming__SetMissionDoesntRequireModel=0x409C90,-defsym=_CTimer__Resume=0x561A00,-defsym=_CTimer__Suspend=0x5619D0,-defsym=_CObject__Create=0x5A1F60,-defsym=_CWorld__Add=0x563220,-defsym=_CPool_CObject___AtHandle=0x465040,-defsym=_CPlaceable__setPosition=0x4241C0,-defsym=_CPlayerInfo__MakePlayerSafe=0x56E870,-defsym=_DIReleaseMouse=0x746F70,-defsym=_CPad__GetPad=0x53FB70


