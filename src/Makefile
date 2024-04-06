CC=/usr/bin/i686-w64-mingw32-gcc
WINLINK=-lgdi32 -luser32 -lwinmm -ldxguid
OUTNAME=Panorama.asi
SRCFILES=ApiGtaSA.c Panorama.c EntryPoint.c

all:
	$(CC) $(WINLINK) -Wall -ggdb3 -shared -static-libgcc -static-libstdc++ $(SRCFILES) -o $(OUTNAME) -Wl,-gc-sections,-defsym=_addTwoStars=0x00438E90,-defsym=_MessageJumpQ=0x69F1E0,-defsym=_forceWeatherNow=0x72A4F0,-defsym=_requestCollision=0x410C00,-defsym=_findGroundZForCoord=0x569660,-defsym=_reportMissionAudioEvent=0x507340,-defsym=_setEffectsMasterVolume=0x506E10
