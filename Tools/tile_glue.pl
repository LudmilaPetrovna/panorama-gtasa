use GD;
use Data::Dumper;

$level=6;$tile_size=128;

# screens 800x600
$level=7;$tile_size=577;
$world=2**$level;

$scaled_tile=50;

$pano=GD::Image->new($scaled_tile*$world,$scaled_tile*$world,1);


sub glue_worker{
my $part_id=shift;
my $parts=shift;
print "Worker started, part_id: $part_id / $parts\n";
my $part_size=int($world/$parts);
my $start=$part_id*$part_size;
my $end=$start+$part_size;
my($q,$w,$yy);
for($w=$start;$w<$end;$w++){
for($q=0;$q<$world;$q++){
$yy=$w;
$filename="tile-${q}x${yy}.jpg";
$filename2="tile-${q}x${yy}.jpg";
#if(-e($filename) && !-e($filename2)){
#`convert $filename $filename2`;
#}

$size=-s($filename2);
print "Processing at $q x $w, $filename ($size)...\n";
if(!-e($filename2)){next;}
$tile=GD::Image->new($filename2);
if(!$tile){
print "Can't load $filename\n";
next;
}

($tile_w,$tile_h)=$tile->getBounds();

$offset_x=int(($tile_w-$tile_size)/2);
$offset_y=int(($tile_h-$tile_size)/2);

$pano->copyResampled($tile,$q*$scaled_tile,$w*$scaled_tile,$offset_x,$offset_y,$scaled_tile,$scaled_tile,$tile_size,$tile_size);
#$pano->string(gdTinyFont,$q*$scaled_tile,$w*$scaled_tile,"$q x $w",0xFFFFFF);


}
}
}

glue_worker(0,1);

open(dd,">glued.png");
binmode(dd);
print dd $pano->png(9);
close(dd)
