#include <a_samp>
#include <pFraktion>
#include <dini>

main()
{
	print("\n----------------------------------");
	print(" Blank Gamemode by your name here");
	print("----------------------------------\n");
}


public OnGameModeInit()
{
	addFraktion(0, "Police", 255);
	addFraktion(1, "Feuerwehr", 255);
	addFraktion(2, "Arzt", 255);
	AddStaticVehicle(541,2040.6577,1341.2526,10.6719,267.8891,7,7);
	AddStaticVehicle(541, 2051.8657,1340.8397,10.6719,267.8891, 5,5);
	// Don't use these lines if it's a filterscript
	SetGameModeText("Blank Script");
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);
	return 1;
}

public OnPlayerConnect(playerid) {
	GivePlayerMoney(playerid, 1500);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraPos(playerid, 1958.3783, 1343.1572, 15.3746);
	SetPlayerCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746);
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp("/get", cmdtext, true, 10) == 0) {
	    new string[255]; new leader[255];
	    if ( getLeader(playerid) == 1 ) { format(leader,sizeof leader, "Yes"); } else { format(leader,sizeof leader, "No"); }
		format(string, sizeof string, "You Fraktion %d, name: %s, Leader: %s", getFraktion(playerid), getFraktionName(playerid), leader);
		SendClientMessage(playerid, 0x00FF64FF, string);
		return 1;
	}
	if (strcmp("/setto", cmdtext, true, 10) == 0) {
	    new string[255], newfrak;
		newfrak = random(2);
		setFraktion(playerid, newfrak);
		format(string, sizeof string, "You now in Fraktion %d, name: %s", newfrak, getFraktionName(playerid));
		SendClientMessage(playerid, 0x00FF64FF, string);
		return 1;
	}
	return 0;
}

public OnPlayerUpdate(playerid) {
	SetPlayerScore(playerid, GetPlayerMoney(playerid));
	return 1;
}
