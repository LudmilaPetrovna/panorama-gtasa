opendir(dd,".");
@sources=sort grep{/\.(c|cpp|h|hpp)$/}readdir(dd);
closedir(dd);

=pod
./CShadows.cpp:float &MAX_DISTANCE_PED_SHADOWS = *(float *)0x8D5240;
./CMirrors.cpp:float *Screens8Track = (float *)0x8D5DD8;
./CDraw.cpp:float &CDraw::ms_fFOV = *(float *)0x8D5038;
./CClouds.cpp:float &CClouds::m_fVolumetricCloudDensity = *(float *)0x8D5388;
./CClouds.cpp:bool &CClouds::m_bVolumetricCloudHeightSwitch = *(bool *)0x8D538C;
./CClouds.cpp:float &CClouds::m_fVolumetricCloudWindMoveFactor = *(float *)0x8D5390;
./CClouds.cpp:unsigned char *RAINBOW_LINES_COLOR_RED = (unsigned char *)0x8D5370;
./CClouds.cpp:unsigned char *RAINBOW_LINES_COLOR_GREEN = (unsigned char *)0x8D5378;
./CClouds.cpp:unsigned char *RAINBOW_LINES_COLOR_BLUE  = (unsigned char *)0x8D5380;
./CClouds.cpp:float *LOW_CLOUDS_X_COORDS = (float *)0x8D5394;
./CClouds.cpp:float *LOW_CLOUDS_Y_COORDS = (float *)0x8D53C4;
./CClouds.cpp:float *LOW_CLOUDS_Z_COORDS = (float *)0x8D53F4;
./CClouds.cpp:float *STARS_Y_POSITIONS = (float *)0x8D55EC;
./CClouds.cpp:float *STARS_Z_POSITIONS = (float *)0x8D5610;
./CClouds.cpp:float *STARS_SIZES = (float *)0x8D5634;
./CClouds.cpp:float &CurrentFogIntensity = *(float *)0x8D5798;
./CBirds.cpp:float *BIRD_CREATION_COORS_X = (float *)0x8D5250;
./CBirds.cpp:float *BIRD_CREATION_COORS_Y = (float *)0x8D5268;
./CBirds.cpp:float *BIRD_CREATION_COORS_Z = (float *)0x8D5280;
float& CTimer::ms_fOldTimeStep = *(float*)0xB7CB54;
float& CTimer::ms_fSlowMotionScale = *(float*)0xB7CB60;

<?xml version="1.0" encoding="utf-8"?>
<CheatTable>
  <CheatEntries>
    <CheatEntry>
      <ID>10</ID>
      <Description>"CurrentFogIntensity"</Description>
      <LastState Value="500" RealAddress="008D5798"/>
      <ShowAsSigned>0</ShowAsSigned>
      <VariableType>Float</VariableType>
      <Address>0x8D5798</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>11</ID>
      <Description>"1byte"</Description>
      <LastState Value="0" RealAddress="056F1C8C"/>
      <ShowAsSigned>0</ShowAsSigned>
      <VariableType>Byte</VariableType>
      <Address>056F1C8C</Address>
    </CheatEntry>
    <CheatEntry>
      <ID>12</ID>
      <Description>"2byte"</Description>
      <LastState Value="65424" RealAddress="0327656C"/>
      <ShowAsSigned>0</ShowAsSigned>
      <VariableType>2 Bytes</VariableType>
      <Address>0327656C</Address>
    </CheatEntry>
  </CheatEntries>
</CheatTable>


=cut

print <<AAA;
<?xml version="1.0" encoding="utf-8"?>
<CheatTable>
  <CheatEntries>
AAA

foreach $sourcefile(@sources){
open(dd,$sourcefile);
read(dd,$file,-s(dd));
close(dd);


while($file=~/^\s*((signed |unsigned |static ))*(char|bool|float|short|int)\s*([\&\*])\s*(\S+)\s*=\s*\*?\([^\)]+\)0x([\da-f]{6});/gmi){
($mods,$type,$link,$name,$addr)=($1,$3,$4,$5,$6);

$signed=0;
if($mods=~/signed/){
$signed=1;
}
if($mods=~/unsigned/){
$signed=0;
}


if($type eq "char" || $type eq "bool"){
$type="Byte"
}

if($type eq "short"){
$type="2 Bytes"
}

if($type eq "int"){
$type="4 Bytes"
}

if($type eq "float"){
$type="Float"
}

$name=~s/[^A-Za-z0-9_]+/_/gs;

print <<AAA;
    <CheatEntry>
      <ID>12</ID>
      <Description>"$name"</Description>
      <ShowAsSigned>$signed</ShowAsSigned>
      <VariableType>$type</VariableType>
      <Address>$addr</Address>
    </CheatEntry>
AAA


}

}

print <<AAA;
  </CheatEntries>
</CheatTable>
AAA
