use File::Find;
use File::Path qw(make_path remove_tree);
use File::Basename;
use Data::Dumper;

# find and glue pano--2254x427x52.png

@pics=();

find({no_chdir=>1,follow=>1,wanted=>sub{
if(-d($File::Find::name)){return;}
if(basename($File::Find::name)=~/^pano.+png$/i){
push(@pics,$File::Find::name);
}
}},"gta-micro/fly/");

@pics=sort{$a->[2] <=> $b->[2]}map{/pano-([\d\-]+)x([\d\-]+)x([\d\-]+)\.png/;[$_,$1,$2,$3]}@pics;
print Dumper(\@pics);

@frames=();
for($q=0;$q<@pics;$q++){
$tmpfile="tmp-fly-frame-$q.gif";
print "Converting $pics[$q]->[0] ---> $tmpfile\n";
if(!-s($tmpfile)){
`convert $pics[$q]->[0] -resize 500x500 $tmpfile`
}
push(@frames,$tmpfile);
}

$frames_list=join(" ",@frames);

unlink("fly0.gif");
unlink("fly.gif");
`convert $frames_list fly0.gif`;
`gifsicle -O9 -o fly.gif --lossy=99 -l0 fly0.gif`;

map{unlink($_)}@frames;
