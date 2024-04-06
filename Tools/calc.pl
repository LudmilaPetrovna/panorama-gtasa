$aaa=<<AAA;
// teleport

02CE: 2@ = ground_z_at 0@ 1@ 22000500.0
04E4: refresh_game_renderer_at 0@ 1@
03CB: set_rendering_origin_at 0@ 1@ 2@
    IF
00DF:   actor $PLAYER_ACTOR driving 
   THEN
03C0: 3@ = actor $PLAYER_ACTOR car
00AB: put_car 3@ at 0@ 1@ 2@
   ELSE
00A1: put_actor $PLAYER_ACTOR at 0@ 1@ 2@
   END

AAA


$aaa=~s/([\da-f]{4}):/$1." (".hex($1).")"/gsei;

print $aaa;