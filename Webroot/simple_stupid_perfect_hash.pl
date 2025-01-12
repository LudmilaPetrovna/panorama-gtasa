@words=split(/\n+/,<<CODE);
GET /
POST /
Host:
Connection:
Pragma:
Cache-Control:
User-Agent:
Upgrade:
Origin:
Sec-WebSocket-Version:
Accept-Encoding:
Accept-Language:
Sec-WebSocket-Key:
Sec-WebSocket-Extensions:
Content-Length:
Upgrade-Insecure-Requests:
Content-Type:
Accept:
Referer:
application/x-www-form-urlencoded
memr
memw
memc
mems
exit
info
find
ping
sock
file
stor
func
subs
usub
menu
.txt	text/plain; charset=UTF-8
.html	text/html; charset=UTF-8
.htm	text/html; charset=UTF-8
.css	text/css; charset=UTF-8
.js	text/javascript; charset=UTF-8
.json	application/json
.png	image/png
.jpg	image/jpeg
.jpeg	image/jpeg
.gif	image/gif
.ico	image/vnd.microsoft.icon
.avif	image/avif
.bmp	image/bmp
.wav	audio/wav
.mp3	audio/mpeg
.mp4	video/mp4
.webm	video/webm
.ogg	application/ogg
.exe	application/octect-stream
.dll	application/octect-stream
.bin	application/octect-stream
CODE

%sums=();
foreach $word(@words){

if($word=~/\t/){
($word,$mime)=split(/\s+/,$word);
} else {
$mime="";
}

$name=uc($word);
$name=~s/[^A-Z0-9]+/_/sg;
$name="PH_".$name;
$sum=calc_hash(uc($word));

if(exists $sums{$sum}){
die "Hash not perfect!!! $name and $sums{$sum} are equal!";
}
$sums{$sum}=$name;
$out.=<<CODE;
#define $name $sum
CODE
if($mime){
print "case $name: t=\"$mime\";break;\n";
}
}
print $out;

sub calc_hash{
my $str=shift;
my $ret=0;
my $q;
my $len=length($str);
for($q=0;$q<$len;$q++){
$ret<<=5;
$ret^=ord(uc(substr($str,$q,1)));
$ret+=$q;
$ret&=0x7fff;
}
return($ret);
}
