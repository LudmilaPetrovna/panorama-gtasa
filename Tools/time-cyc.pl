$cols=<<AAA;
Static ambience color	TColor	The color of ambient light on map objects (see IPL).
Dynamic ambience color	TColor	The color of ambient light on dynamically created objects like pedestrians and vehicles.
Direct light color	TColor	The color of direct light on dynamicly created objects like pedestriants and vehicles. Present in the timecyc.dat of GTA SA, but not loaded and unused by the game.
Sky top color	TColor	The color of the top of the skysphere.
Sky bottom color	TColor	The color of the bottom of the skysphere.
Sun core color	TColor	The color of the core sprite of the sun.
Sun corona color	TColor	The color of the corona sprite of the sun.
Sun core size	FLOAT	The size of the core sprite of the sun.
Sun corona size	FLOAT	The size of the corona sprite of the sun.
Sprite brightness	FLOAT	The the brightness of the above mentioned sprites.
Shadow intensity	INT32	The intensity of the shadows of objects like pedestriants, vehicles or poles.
Light shading value	INT32	Value used for the shading algorithm on pedestirants and vehicles [?].
Pole shading value	INT32	Shading value for map objects with flag 32768.
Far clipping offset	FLOAT	The distance of the far clipping plane from the camera.
Fog start offset	FLOAT	The range of the distance fog from the camera.
Light on ground	FLOAT	Amount of light on the ground of the sea [?].
Lower clouds color	TColor	Color of the lower clounds.
Upper clouds bottom color	TColor	Color of the bottom of the upper clouds.
Water color	TColor	Color of the water vertices.
Water alpha level	UINT8	Alpha level for water vertices.
Color correction 1 alpha	UINT8	Alpha level for first color correction value.
Color correction 1	TColor	First color correction value.
Color correction 2 alpha	UINT8	Alpha level for second color correction value.
Color correction 2	TColor	Second color correction value.
Lower clouds alpha level	UINT8	Alpha level for lower clouds.
Highlight min intensity	UINT8	Intensity limit for PS2 radiosity effect.
Water fog alpha.	UINT8	Water fading to white, amount of white light on water. Usually 0.
Directional multiplier	FLOAT	Multiplier for the main directional light. Always 1.0, but missing in PC timecyc.dat
AAA

$types='%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f %f %f %d %d %d %f %f %f %d %d %d %d %d %d %f %f %f %f  %f %f %f %f  %f %f %f %f %f %d %d %f';



@fields2=map{s/\s+/ /gs;s/;//s;s/ *=.+//s;$_}grep{!/\&v\d/}split(/\n[\n\t ]*/s,<<AAA);
&v94,
&v96,
&v72,
CTimeCyc::AMBRedPool[v2] = v94;
CTimeCyc::AMBGreenPool[v2] = v5;
CTimeCyc::AMBBluePool[v2] = v6;

        &v117,
        &v74,
        &v98,
CTimeCyc::AMB_OBJRedPool
CTimeCyc::AMB_OBJGreenPool[v2] = v9;
CTimeCyc::AMB_OBJBluePool[v2] = v10;


       &v121,???
        &v120,???
        &v119,???
C???
C???
C???

	  &v76,
        &v110,
        &v78,
CTimeCyc::SkyTopRedPool[v2] = v11;
CTimeCyc::SkyTopGreenPool[v2] = v12;
CTimeCyc::SkyTopBluePool[v2] = v13;

    &v100,
        &v80,
        &v116,
 CTimeCyc::SkyBotRedPool[v2] = v14;
 CTimeCyc__SkyBotGreenPool[v2] = v15;
 CTimeCyc::SkyBotBluePool[v2] = v16;


 &v82,
    &v102,
    &v84,
     CTimeCyc::SuncoreRedPool[v2] = v17;
      CTimeCyc::SuncoreGreenPool[v2] = v18;
      CTimeCyc::SuncoreBluePool[v2] = v19;

 &v112,
        &v86,
        &v104,

      CTimeCyc::SuncoronaRedPool[v2] = v20;
      CTimeCyc::SuncoronaGreenPool[v2] = v21;
      CTimeCyc::SuncoronaBluePool[v2] = v22;

   &v88,
        &v118,
        &v90,

CTimeCyc::SunSzPool[v2] =  v88 * 10.0 + 0.5;
CTimeCyc::SprSzPool[v2] = v118 * 10.0+ 0.5;
CTimeCyc::SprBghtPool[v2]=v90 * 10.0+.5;

 &v106,
        &v92,
        &v114,

CTimeCyc::ShdwPool[v2] = v106;
CTimeCyc::LightShdPool[v2] = v27;
CTimeCyc::PoleShdPool[v2]=114

  &v69,
        &v108,
        &v70,

*(CTimeCyc::FarClipPool + v1) = v69;
*(CTimeCyc::FogStPool + v1) = v108;
CTimeCyc::LightOnGroundPool[v2] = v70*10.0+.5;

   &v71,
        &v73,
        &v75,
CTimeCyc::LowCloudsRedPool[v2] = v71;
CTimeCyc::LowCloudsGreenPool[v2] = v35;
CTimeCyc::LowCloudsBluePool[v2] 


       &v77,
        &v79,
        &v81,
CTimeCyc::BottomCloudBluePool[v2] = v33;
CTimeCyc::BottomCloudGreenPool[v2] = v36;
CTimeCyc::BottomCloudRedPool[v2] = BYTE4(v33);

        &v83,
        &v85,
        &v87,

CTimeCyc::WaterRedPool[v2] = v37;
CTimeCyc::WaterGreenPool[v2] = v39;
CTimeCyc::WaterBluePool[v2] = v41;

        &v89,
        &v103,
        &v91,

CTimeCyc::WaterAlphaPool[v2] = v43;
CTimeCyc::Alpha1Pool[v2] =v103*2.0 
CTimeCyc::RGB1RedPool[v2] = v45;

        &v93,
        &v95,
        &v105,
CTimeCyc::RGB1GreenPool[v2] = v47;
CTimeCyc::RGB1BluePool[v2] = v49;
CTimeCyc::Alpha2Pool[v2] = v5*2.0


        &v97,
        &v99,
        &v101,
CTimeCyc::RGB2RedPool[v2] = v51;
CTimeCyc::RGB2GreenPool[v2] = v53;
CTimeCyc::RGB2BluePool[v2] = v55;

        &v107,
        &v109,
        &v111,
CTimeCyc::Alpha1Pool[v2] = v57;
CTimeCyc::CloudAlpha2Pool[v2] = v109;
CTimeCyc::CloudAlpha3Pool[v2] = v63;

        &v113);
CTimeCyc::IlluminationPool[v2] = (v115 * 100.0);
AAA

@regs=map{s/\s+/ /gs;s/;//s;s/ *=.+//s;$_}split(/\n[\n\t ]*/s,<<AAA);
    &v94,
        &v96,
        &v72,
        &v117,
        &v74,
        &v98,
        &v121,
        &v120,
        &v119,
        &v76,
        &v110,
        &v78,
        &v100,
        &v80,
        &v116,
        &v82,
        &v102,
        &v84,
        &v112,
        &v86,
        &v104,
        &v88,
        &v118,
        &v90,
        &v106,
        &v92,
        &v114,
        &v69,
        &v108,
        &v70,
        &v71,
        &v73,
        &v75,
        &v77,
        &v79,
        &v81,
        &v83,
        &v85,
        &v87,
        &v89,
        &v103,
        &v91,
        &v93,
        &v95,
        &v105,
        &v97,
        &v99,
        &v101,
        &v107,
        &v109,
        &v111,
        &v113
AAA



@colors=(0,3,6,9,12,15,18,30,33,36,40,44);




@fields=map{[split(/\t/,$_,3)]}split(/[\r\n]+/,$cols);

open(ii,$ARGV[0]||"timecyc-orig.dat");
open(oo2,">timecyc.dat");
open(oo,">time.csv");
print oo join(";",(0..100))."\n";
print oo join(";",@fields2)."\n";
print oo join(";",split(/\s+/,$types))."\n";
print oo join(";",@regs)."\n";

while(<ii>){
if(/^\/\//){
print oo $_;
next;
}
@vals=split(/\s+/,$_);

print oo join(";",@vals)."\n";

$vals[27]="8000.0";
$vals[28]="0.0";

=pod
foreach $color_offset(@colorsaa){
$luma=int(rand()*256);

$vals[$color_offset+0]=$luma;
$vals[$color_offset+1]=$luma;
$vals[$color_offset+2]=$luma;
}

$vals[40]=0;
$vals[41]=0;
$vals[42]=0;
$vals[43]=0;

$vals[44]=0;
$vals[45]=0;
$vals[46]=0;
$vals[47]=0;


$vals[51]=rand()*rand()*rand()*100.0;
=cut

for($q=0;$q<50;$q++){
if($vals[$q]==int($vals[$q]) && $vals[$q]>=0 && $vals[$q]<256){
#$vals[$q]=128;
}
}

print oo2 join(" ",@vals)."\n";


}

