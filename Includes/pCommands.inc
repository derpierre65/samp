/*
	native GetPlayerPoolSize();
	native GetServerTicks();
*/
#if !defined _samp_included
    #include <a_samp>
#endif
#if defined _pCommands_included
    #endinput
#endif
#define _pCommands_included
/*
	VARIABLE
*/
static _pCommands_maxPlayers = -1;
/*
	HOOKS
*/
forward pCommands_OnPlayerConnect(playerid);
forward pCommands_OnPlayerDisconnect(playerid, reason);
forward GetPlayerPoolSize();
/*
	OnPlayerConnect
*/
public OnPlayerConnect(playerid) {
	if ( playerid > _pCommands_maxPlayers ) { _pCommands_maxPlayers = playerid; }
	CallLocalFunction("pCommands_OnPlayerConnect","i", playerid);
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect pCommands_OnPlayerConnect
/*
	OnPlayerDisconnect
*/
public OnPlayerDisconnect(playerid, reason) {
	_pCommands_maxPlayers = -1;
	for (new i=0,c=GetMaxPlayers();i<c;i++) {
		if ( !IsPlayerConnected(i) ) { continue; }
		_pCommands_maxPlayers = i;
	}
	CallLocalFunction("pCommands_OnPlayerDisconnect","ii", playerid,reason);
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect pCommands_OnPlayerDisconnect
/*
	GetPlayerPoolSize (doesn't need it in 0.3.7+)
	return max playerid on the server + 1 (0 players = -1, 1 player (id:0) = 1,...)
	2015 by (der)pierre65
*/
public GetPlayerPoolSize() { return _pCommands_maxPlayers + 1; }
/*
	GetServerTicks()
	return ticks from server
	2012 by D0erfler
*/
stock GetServerTicks() {
	new network_str[401];
	GetNetworkStats(network_str, sizeof(network_str));
	new i = 14, number[5];
	while(network_str[i] >= 48 && network_str[i] <= 57) {
	    number[i-14] = network_str[i];
	    i++;
	}
	return strval(number);
}