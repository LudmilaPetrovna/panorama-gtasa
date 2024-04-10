CC=/usr/bin/i686-w64-mingw32-gcc
WINLINK=-lgdi32 -luser32 -lwinmm -ldxguid
OUTNAME=Panorama.asi
SRCFILES=ApiGtaSA.c Panorama.c EntryPoint.c

all:
	$(CC) $(WINLINK) -Wall -ggdb3 -shared -static-libgcc -static-libstdc++ $(SRCFILES) -o $(OUTNAME) -Wl,-gc-sections,-defsym=_addTwoStars=0x00438E90,-defsym=_MessageJumpQ=0x69F1E0,-defsym=_forceWeatherNow=0x72A4F0,-defsym=_requestCollision=0x410C00,-defsym=_findGroundZForCoord=0x569660,-defsym=_reportMissionAudioEvent=0x507340,-defsym=_setEffectsMasterVolume=0x506E10,-defsym=_CPlaceable__SetHeading=0x43E0C0,-defsym=_CPlaceable__GetHeading=0x441DB0,-defsym=_FindPlayerHeading=0x56E450,-defsym=_JPegCompressScreenToFile=0x5D0820,-defsym=_CGameLogic__RestorePlayerStuffDuringResurrection=0x442060,-defsym=_CStreaming__RemoveBuildingsNotInArea=0x4094B0