


#float &CWeather::WindClipped = *(float *)0xC812EC;


open(dd,$ARGV[0]);
read(dd,$aaa,-s(dd));


$fuzz="";

foreach(split(/[\r\n]+/,$aaa)){


if(/(((unsigned|signed) )*(float|char|int))[&\*]? (\S+).*?(0x[\dA-Fa-f]{6})/s){
$type=$1;
$name=$5;
$addr=$6;
$name=~s/[^A-Za-z0-9]+/_/gs;

$type=~s/bool/char/gs;

print "$type *$name=($type*)$addr;\n";

$mult=1;
if($type=~/float/){$mult="1.0";}
if($type=~/char/){$mult="255.0";}
if($type=~/int/){$mult="1024.0";}

$fuzz.="*$name=$mult*drand();\n";


}


}

print "\n\n$fuzz\n";