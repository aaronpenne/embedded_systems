## ARM

### two_track_recorder.c
Designed for the (STM32F0 Discovery Board)[http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF253215?sc=internet/evalboard/product/253215.jsp]. Uses a keypad and speaker to make Nintendo sounding songs on the fly. Allows for recording and looping while playing. This program has 4 modes, Control, Play, Record, Playback. Control waits for input to switch to another mode. You get to control mode from any other mode by pushing the B key. To access the other modes from	control push:
  1 - Play
	2 - Record
	3 - Playback
Play is a free play note mode. Record will record up to 100 notes. Playback allows you to play the song that was recorded back, plus lets you play other notes on top of it live.
