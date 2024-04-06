use GD;
use Data::Dumper;




opendir(dd,".");
@files=map{[$_,-s($_)]}grep{/frame.+jpg/}readdir(dd);
closedir(dd);


@files=sort{$b->[1] <=> $a->[1]}@files;
#@files=sort{rand()>.5?1:-1}@files;
splice(@files,1024);


$count=@files;
$tile_x=int(sqrt($count)+.9);
print "Create mosaic ($tile_x x $tile_x) for $count files\n";

$width=32;
$height=24;
$filepos=0;
$mosaic=GD::Image->new($tile_x*$width,$tile_x*$height,1);


for($w=0;$w<$tile_x;$w++){
for($q=0;$q<$tile_x;$q++){
if(!$files[$filepos]){next;}
if($files[$filepos]->[1]<500){next;}
print "Processing at $q x $w, $files[$filepos]->[1]...\n";
$tile=GD::Image->newFromJpeg($files[$filepos++]->[0],1);
if(!$tile){next;}

$mosaic->copyResampled($tile,$q*$width,$w*$height,0,0,$width,$height,$tile->getBounds());

}
}

open(dd,">mosaic.png");
binmode(dd);
print dd $mosaic->png(9);
close(dd)
