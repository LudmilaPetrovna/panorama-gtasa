=pod
D3D9Transform=8D7390
_D3D9InverseTransform=8D73D0
nverseNormalizedTransform=008D7410
D3D9NormalizedTransform=8D7450
_RwD3D9D3D9ProjTransform=8E2458
_D3D9ViewProjTransform=c94c30
D3D9ActiveTransform=C94C70
_RwD3D9D3D9ViewTransform=C9BC80
D3D9ActiveNormalizedTransform=00C94C7C
=cut


$stru=<<CODE;
00000014 projectionType  dd ?
00000018 beginUpdate     dd ?                    ; offset
0000001C endUpdate       dd ?                    ; offset
00000020 viewMatrix      RwMatrixTag ?
00000060 frameBuffer     dd ?                    ; RwRaster *
00000064 zBuffer         dd ?                    ; offset
00000068 viewWindow      RwV2d ?
00000070 recipViewWindow RwV2d ?
00000078 viewOffset      RwV2d ?
00000080 nearPlane       dd ?
00000084 farPlane        dd ?
00000088 fogPlane        dd ?
0000008C zScale          dd ?
00000090 zShift          dd ?
00000094 frustrumPlanes  RwFrustumPlane 6 dup(?)
0000010C frustrumBoundBox RwBBox ?
00000124 frustrumCorners RwV3d 8 dup(?)
CODE

$addr=<<CODE;
theCamera=[0xB6F028]=0xd78
parallel mode=[0xC17038+4]+0x14=1
CODE

#RwEngineInstance->curCamera->object.object.parent


print <<TT;
<?xml version="1.0" encoding="utf-8"?>
<CheatTable>
  <CheatEntries>
TT

$id=0;
foreach $entry(split(/\r?\n\s*/,$stru)){
($title,$pointer,$len)=split(/=/,$entry);
($offset,$title,$type,$dups,$comment)=split(/\s+/,$entry);
$offset="0x".$offset;

if($type eq "RwV2d"){
printEntry("Scene-Cam-$title.x","[0xC17038+4]+$offset","Float");
printEntry("Scene-Cam-$title.y","[0xC17038+4]+$offset+4","Float");
next;
}
if($type eq "RwV3d"){
printEntry("Scene-Cam-$title.x","[0xC17038+4]+$offset","Float");
printEntry("Scene-Cam-$title.y","[0xC17038+4]+$offset+4","Float");
printEntry("Scene-Cam-$title.z","[0xC17038+4]+$offset+8","Float");
next;
}

if($type eq "dd"){
$type="4 Bytes";
$type="Float";
}

if($title eq "projectionType"){
$type="4 Bytes"
}

if($type eq "dw"){
$type="2 Bytes";
}
if($type eq "db"){
$type="Byte";
}
if($type eq "RwMatrixTag"){
$type=64;
}
if($type eq "RwBBox"){
$type=24;
}
if($type eq "RwFrustumPlane"){
$type=20;
}
printEntry("Scene-Cam-$title","[0xC17038+4]+$offset",$type);

}

print <<TT;
  </CheatEntries>
</CheatTable>
TT

sub printEntry{
my $title=shift;
my $addr=shift;
my $type=shift;
my $len=1;
my $ftype="";

if($type=~/^\d+$/){
$ftype=<<CODE;
      <VariableType>Array of byte</VariableType>
      <ByteLength>$type</ByteLength>
CODE
} else {
$ftype=<<CODE;
      <VariableType>$type</VariableType>
CODE
}

if($type ne "Float"){
$ftype.="      <ShowAsHex>1</ShowAsHex>\n";
}

$gid*=1;

print <<TT;

    <CheatEntry>
      <ID>$gid</ID>
      <Description>"$title"</Description>
      <ShowAsSigned>0</ShowAsSigned>
$ftype      <Address>$addr</Address>
    </CheatEntry>
TT
$gid++;


}
