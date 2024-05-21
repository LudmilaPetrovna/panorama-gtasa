print "from ghidra.program.model.symbol import SourceType\n";

opendir(dd,".");
@files=grep{/\.(cpp|h)/i}readdir(dd);
closedir(dd);

%addrs=();

foreach $filename(@files){
print STDERR "Reading $filename...\n";
open(dd,$filename);
read(dd,$file,-s(dd));
close(dd);

#// Converted from thiscall void C3dMarker::Render(void) 0x7223D

while($file=~/Converted from .+?(\S+)\([^\)]+\) (0x[\da-f]{6})/img){
$name=$1;
$addr=$2;
print <<AAA;
addr=toAddr($addr);createFunction(addr,"$name");disassemble(addr);getFunctionAt(addr).setName("$name",SourceType.USER_DEFINED);
AAA
}

# addrof(CCheat::AllCarsAreGreatCheat) = ADDRESS_BY_VERSION(0x4394E0, 0, 0, 0, 0, 0);
while($file=~/addrof\(([^\)]+)\) = ADDRESS_BY_VERSION\((0x[\da-f]{6}),/img){
$name=$1;
$addr=$2;
print <<AAA;
addr=toAddr($addr);createFunction(addr,"$name");disassemble(addr);getFunctionAt(addr).setName("$name",SourceType.USER_DEFINED);
AAA
}

#define FUNC_CText__Get 0x6A0050
while($file=~/#define FUNC_(\S+) (0x[\da-f]{6})/img){
$name=$1;
$addr=$2;
print <<AAA;
addr=toAddr($addr);createFunction(addr,"$name");disassemble(addr);getFunctionAt(addr).setName("$name",SourceType.USER_DEFINED);
AAA
}

#float &CWeather::Foggyness = *(float *)0xC81300;
#unsigned int& CTimer::m_snTimeInMilliseconds = *(unsigned int*)0xB7CB84;
while($file=~/([a-z][^\(\&\*\) ]+) = .+?(0x[\da-f]{6})/img){
$name=$1;
$addr=$2;
print <<AAA;
addr=toAddr($addr);createLabel(addr,"$name", True, SourceType.USER_DEFINED);
AAA
}


}
