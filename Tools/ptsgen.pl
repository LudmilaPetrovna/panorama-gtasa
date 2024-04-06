open(oo,">pano.pts");
print oo <<AAA;
# ptGui project file

#-encoding utf8
#-pathseparator \
#-fileversion 49
#-vfov 180
#-resolution 300

# Panorama settings:
p w2048 h1024 f2 v360 u0 n"JPEG g0 q95"
m g0 i6

# input images:
#-dummyimage
AAA

$fov=125.0;
print oo <<AAA;
# The following line contains a 'dummy image' containing some global parameters for the project
o w1 h1 y0 r0 p0 v$fov a-0.0 b-0.0 c0.0 f0 d0 e0 g0 t0
AAA
#-imgfile $cwidth $cheight "dummy.jpg"

mkdir "corrected",0777;

open(ii,"pano.pto");
while(<ii>){
if(/^i f0 w(\d+) h(\d+) r([\d\.\-]+) p([\d\.\-]+) y([\d\.\-]+) v([\d\.\-]+) n\"([^\"]+)\"/){
($width,$height,$roll,$pitch,$yaw,$fov,$filename)=($1,$2,$3,$4,$5,$6,$7);
$width=1920;
$height=1080;
$cwidth=1440;
$cheight=1080;

$nfile="corrected/".$filename;
if(!-e($nfile)){
`ffmpeg -v 0 -i "$filename" -s 1440x1080 -y "$nfile"`;
}
print oo <<AAA;
#-imgfile $cwidth $cheight "corrected/$filename"
o f0 y$yaw r$roll p$pitch v$fov
AAA

}
}
