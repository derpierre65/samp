#include <dini>
/*
native addFraktion(frakid, frakname[], color);
native makeFraktionLeader(playerid, id);
native getLeader(playerid);
native setFraktion(playerid, id);
native getFraktion(playerid);
native getFraktionName(playerid);

*/

stock addFraktion(id, frakname[], color) {
	if ( dini_Exists("pFrak/") ) {
	    new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", id);
	    if ( dini_Exists(frakfile) ) { dini_Remove(frakfile); }
		printf("[pFraktion] Created fraktion %s (%d) with colorcode.", frakname, id);
		dini_Create(frakfile);
		dini_Set(frakfile, "Frakname", frakname);
		dini_IntSet(frakfile, "Frakcolor", color);
	}
	else { printf("[pFraktion] Folder pFrak/ not found!"); }
	return 1;
}

stock setFraktion(playerid, id) {
	if ( IsPlayerConnected(playerid) ) {
		new playername[255];
		GetPlayerName(playerid, playername, 255);
		new playerfile[255]; format(playerfile, 255, "pFrak/%s.ini", playername);
	    new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", id);
		if ( dini_Exists(frakfile) ) {
			dini_Remove(playerfile);
			dini_Create(playerfile);
			dini_IntSet(playerfile, "Frak", id);
   			dini_IntSet(playerfile, "Leader", 0);
   			SetPlayerColor(playerid, dini_Int(frakfile, "Frakcolor"));
   			SetPlayerTeam(playerid, id);
		}
		else { return -1; }
	}
	else { return -1; }
	return 1;
}

stock makeFraktionLeader(playerid, id) {
	if ( IsPlayerConnected(playerid) ) {
		new playername[255];
		GetPlayerName(playerid, playername, 255);
		new playerfile[255]; format(playerfile, 255, "pFrak/%s.ini", playername);
	    new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", id);
		if ( dini_Exists(frakfile) ) {
		    if ( !dini_Exists(playerfile) ) {
				dini_Create(playerfile);
				dini_IntSet(playerfile, "Frak", id);
			}
			dini_IntSet(playerfile, "Frak", id);
   			dini_IntSet(playerfile, "Leader", 1);
   			SetPlayerColor(playerid, dini_Int(frakfile, "Frakcolor"));
   			SetPlayerTeam(playerid, id);
		}
		else { return -1; }
	}
	else { return -1; }
}

stock getFraktion(playerid) {
	if ( IsPlayerConnected(playerid) ) {
		new playername[255];
		GetPlayerName(playerid, playername, 255);
		new playerfile[255]; format(playerfile, 255, "pFrak/%s.ini", playername);
		if ( dini_Exists(playerfile) ) {
			new frakid = dini_Int(playerfile, "Frak");
	    	new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", frakid);
	    	if ( dini_Exists(frakfile) ) { return frakid; }
			else { return -1; }
		}
		else { return -1; }
	}
	else { return-10; }
}

stock getLeader(playerid) {
	if ( IsPlayerConnected(playerid) ) {
		new playername[255];
		GetPlayerName(playerid, playername, 255);
		new playerfile[255]; format(playerfile, 255, "pFrak/%s.ini", playername);
		if ( dini_Exists(playerfile) ) {
			new leader = dini_Int(playerfile, "Leader");
			new frakid = dini_Int(playerfile, "Frak");
	    	new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", frakid);
	    	if ( dini_Exists(frakfile) ) {
				return leader;
			}
			else { return -1; }
		}
		else { return -1; }
	}
	else { return -1; }
}

stock getFraktionName(playerid) {
    new playername[255], string[255], playerfile[255];
    format(string,sizeof string, "-1");
	if ( IsPlayerConnected(playerid) ) {
		GetPlayerName(playerid, playername, 255);
		format(playerfile, 255, "pFrak/%s.ini", playername);
		if ( dini_Exists(playerfile) ) {
			new frakid = dini_Int(playerfile, "Frak");
	    	new frakfile[255]; format(frakfile, 255, "pFrak/Frak%d.ini", frakid);
	    	if ( dini_Exists(frakfile) ) {
	    	    format(string,sizeof string, "%s", dini_Get(frakfile, "Frakname"));
				return string;
			}
			else { return string; }
		}
		else { return string; }
	}
	else { return string; }
}