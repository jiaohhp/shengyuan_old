init before branch of only one capture.

STATE:
	electric/current start 	-> record start
	electric/current stop  	-> record stop
	
	acceleration start -> record start
	acceleration stop  -> record stop

	
	electirc/current ing and acceleration start -> nothing
	electric/current ing and acceleration end   -> nothing

	acceleration ing and electric/current start -> record stop and then record start
	acceleration ing and electric/current stop  -> Nerver
