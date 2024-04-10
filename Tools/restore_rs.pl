

##$addrs=`grep -r GLOBAL_ADDRESS_BY_VERSION . | grep -Eio 0x[0-9a-f]+ | sort -u`;
$addrs=`cat all_addrs.txt`;

@addrs=();
while($addrs=~/0x([\da-f]+)/ig){
push(@addrs,$1);
}
$aaa=<<AAA;
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands0To99) = GLOBAL_ADDRESS_BY_VERSION(0x465E60, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands100To199) = GLOBAL_ADDRESS_BY_VERSION(0x466DE0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands200To299) = GLOBAL_ADDRESS_BY_VERSION(0x469390, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands300To399) = GLOBAL_ADDRESS_BY_VERSION(0x47C100, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands400To499) = GLOBAL_ADDRESS_BY_VERSION(0x47D210, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands500To599) = GLOBAL_ADDRESS_BY_VERSION(0x47E090, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands600To699) = GLOBAL_ADDRESS_BY_VERSION(0x47F370, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands700To799) = GLOBAL_ADDRESS_BY_VERSION(0x47FA30, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands800To899) = GLOBAL_ADDRESS_BY_VERSION(0x481300, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands900To999) = GLOBAL_ADDRESS_BY_VERSION(0x483BD0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1000To1099) = GLOBAL_ADDRESS_BY_VERSION(0x489500, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1100To1199) = GLOBAL_ADDRESS_BY_VERSION(0x48A320, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1200To1299) = GLOBAL_ADDRESS_BY_VERSION(0x48B590, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1300To1399) = GLOBAL_ADDRESS_BY_VERSION(0x48CDD0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1400To1499) = GLOBAL_ADDRESS_BY_VERSION(0x48EAA0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1500To1599) = GLOBAL_ADDRESS_BY_VERSION(0x490DB0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1600To1699) = GLOBAL_ADDRESS_BY_VERSION(0x493FE0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1700To1799) = GLOBAL_ADDRESS_BY_VERSION(0x496E00, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1800To1899) = GLOBAL_ADDRESS_BY_VERSION(0x46D050, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands1900To1999) = GLOBAL_ADDRESS_BY_VERSION(0x46B460, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2000To2099) = GLOBAL_ADDRESS_BY_VERSION(0x472310, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2100To2199) = GLOBAL_ADDRESS_BY_VERSION(0x470A90, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2200To2299) = GLOBAL_ADDRESS_BY_VERSION(0x474900, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2300To2399) = GLOBAL_ADDRESS_BY_VERSION(0x4762D0, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2400To2499) = GLOBAL_ADDRESS_BY_VERSION(0x478000, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2500To2599) = GLOBAL_ADDRESS_BY_VERSION(0x47A760, 0, 0, 0, 0, 0);
./CRunningScript.cpp:int gaddrof(CRunningScript::ProcessCommands2600To2699) = GLOBAL_ADDRESS_BY_VERSION(0x479DA0, 0, 0, 0, 0, 0);
AAA

while($aaa=~/(CRunningScript::ProcessCommand[\da-z]+)\)[^\(]+\(0x([\da-f]+)/mgi){
($name,$addr)=($1,$2);
foreach(@addrs){
if(hex($_)>hex($addr)){$end=$_;last;}
}
#print "$name=$addr\n";

$pre.=<<AAA;
del_items(0x$addr, DELIT_SIMPLE, 0x$end-0x$addr);
AAA

$post.=<<AAA;
add_func(0x$addr, BADADDR);
set_name(0x$addr, "$name",SN_AUTO);
AAA

}

print "$pre\n$post\n";

