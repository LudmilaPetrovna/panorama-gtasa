%sizes=();
open(dd,"sizes.txt");
while(<dd>){
if(/VALIDATE_SIZE\(([^,]+), (0x)?([\da-f]+)\);/i){
($name,$hex,$value)=($1,$2,$3);
$sizes{$name}=$hex?hex($3):$3;
}

}

$sizes{char}=1;
$sizes{short}=2;
$sizes{int}=4;
$sizes{float}=4;
$sizes{TList_c}=12;



@need=qw/FxManager_c CVector CMatrix CSimpleTransform CPlaceable CQueuedMode CCam CCamera CEntity CPlayerData/;



open(dd,$ARGV[0]);

while(<dd>){
if(/^class (PLUGIN_API )?([a-z0-9_]+)/i){
$structName=$2;
open(oo,">conv".$structName.".h");
print oo "typedef struct _$structName".'{'."\n"; #}
$pause=0;
$offset=0;
if(/: ?(public )?(C\S+)/){
$subclass=$2;
print "Sub of $subclass\n";
if(in_array($subclass,@need)){
print oo "$subclass PARENT;\n";
} else {
print oo "char PADDING[$sizes{$subclass}]\n";
}
}
$offset=$sizes{$subclass}*1;

next
}

if(/^\};/){
$needpad=$sizes{$structName}-$offset;
if($needpad){
print oo "char PADDING_END[$needpad]; /* must be ".$sizes{$structName}." as size of $structName */ \n";
}
print oo "}$structName;\n";
close(oo);
$structName="";
$pause=1;
next;
}

if(/public:|private:|protected:/){
next;
}

s/\/\/.+/\n/gs;
s/\/\*.+\*\///gs;
s/bool /char /gs;
s/<[^>]+>//gs;


$fc=$_;
$fc=~s/(SUPPORTED_10US|static|unsigned|signed)//gs;
$type="";
if($fc=~/^\s*(\S+)\s/){
$type=$1;
}

$array=1;
if($fc=~/\[(\d+)\]/){
$array=$1*1;
}


$size=0;
if(exists $sizes{$type}){
$size=$sizes{$type};
}

$is_pointer=0;
if($fc=~/\*/){
$size=4;
$is_pointer=1;
}

$bits=1;
if($fc=~/\s*:\s*(\d+)/){
$bits=$1/8;
$size=1;
}

$size*=$array*$bits;
if($is_pointer && !in_array($type,@need)){
$_=~s/\Q$type\E/void/s;
}


if(in_array($type,@need)){
#$_=~s/\Q$type\E/struct _$type/s;
}

if(!$pause && /[()]/){
print "BAD:$_";
$pause=1;next;
}

if(!$pause){
print oo "/*(".sprintf("0x%04x",$offset)." (+$size) $bits,type=$type) */ $_";
$offset+=$size;
}


}


open(oo,">tt.c");

print oo map{"typedef struct _$_ $_;\n"}@need;

print oo map{"#include \"conv$_.h\"\n"}@need;

sub in_array{
my $needle=shift;
my @haystack=@_;
my $tt;
foreach $tt(@haystack){
if($needle eq $tt){return 1;}
}
return 0;
}



