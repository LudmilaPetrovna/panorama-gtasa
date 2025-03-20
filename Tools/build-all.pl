use File::Find;
use File::Path qw(make_path remove_tree);
use File::Basename;

find({no_chdir=>0,follow=>1,wanted=>sub{
if(-d($File::Find::name)){return;}
if($File::Find::name=~/\.bat$/i){
$base=basename($File::Find::name);
print "Invoke base\n";
system($base);
}
}},".");

