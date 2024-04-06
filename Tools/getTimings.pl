
opendir(dd,".");
@files=grep{/pano-\d+.+?stamp$/}readdir(dd);
closedir(dd);

foreach $stampfile(@files){
$pngfile=$stampfile;
$pngfile=~s/\.stamp/.png/gs;
$started=(stat($stampfile))[9];
$finished=(stat($pngfile))[9];
$time=int(($finished-$started)/60+.9);
print "$pngfile = $time minutes\n";

}