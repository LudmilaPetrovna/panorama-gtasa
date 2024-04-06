

@dataset_sizes=qw/128x96 32x24 256x192/; # 32x26 32x28 32x30 32x32 128x96/; #128x72 128x82 32x32/;
@resolutions=qw/512x256 1024x512 2048x1024 4096x2048 8192x4096 16384x8192/;


open(dd,"pano.pto");
read(dd,$pto,-s(dd));



foreach $csize(@dataset_sizes){
($width,$height)=split(/x/,$csize);
$nsize="w$width h$height";
print "Dataset for $csize...\n";
if(!-d($csize)){
print "Dataset for $csize not found, create one...\n";
mkdir $csize,0777;
`ffmpeg -i frame-%7d.png -s $csize $csize/frame-%7d.jpg`;
}

foreach $resolution(@resolutions){
($rwidth,$rheight)=split(/x/,$resolution);
$panoname="pano-$csize-$resolution";
print "Creating $panoname...\n";
$file=$pto;
$file=~s/w128 h96/$nsize/gs;
$file=~s/p w2048 h1024/p w$rwidth h$rheight/gs;
#$file=~s/v20/v$fov/gs;

@lines=split(/[\r\n]+/,$file);

open(oo,">$panoname.pto");
foreach(@lines){
if(s/(frame-(\d+))\.(png|jpe?g)/FILENAMEHOLDER/si){
$num=$2;
$filename=$csize.'/'.sprintf("frame-%07d.jpg",$num*1+1);
$filesize=-s($filename);
if($filesize<300){next;}
s/FILENAMEHOLDER/$filename/gs;
}
print oo "$_\n";

}
close(oo);


open(oo,">>pano-build-all.bat");
print oo "echo timestamp > $panoname.stamp\n";
print oo "\"V:\\h\\Hugin-2023.0.0-win64\\nona\" -o $panoname $panoname.pto\n";
close(oo);


open(oo,">$panoname-build.bat");
print oo "echo timestamp > $panoname.stamp\n";
print oo "\"V:\\h\\Hugin-2023.0.0-win64\\nona\" -o $panoname $panoname.pto\n";
close(oo);

}
}





