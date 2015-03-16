#if !defined _samp_included
    #include <a_samp>
#endif
#if defined _pInfo
    #endinput
#endif
#define _pInfo
#define pInfo_MAX_MESSAGE_BOXES 5
#define pInfo_MAX_MESSAGE_LENGTH 128
#define pInfo_PlaySound 1058

enum pInfo_InfoEnum {
	PlayerText:pInfo_InfoBoxPID,
	PlayerText:pInfo_InfoMsgPID,
	Float:pInfo_InfoPosX,
	pInfo_InfoMessage[pInfo_MAX_MESSAGE_LENGTH],
	pInfo_InfoCreated
};
enum pInfo_PlayerInfoEnum { Float:pInfo_PlayerInfoLastXPos };
static pInfo_Info[MAX_PLAYERS][pInfo_MAX_MESSAGE_BOXES][pInfo_InfoEnum];
static pInfo_PlayerInfo[MAX_PLAYERS][pInfo_PlayerInfoEnum];


forward pInfo_reCreatePlayerTextDraw(playerid, messageID);
forward pInfo_DestroyMessage(playerid, messageID);
forward pInfo_createPlayerTextDraw(playerid, messageID, Float:x, message[]);
forward pInfo_Init();
forward pInfo_Exit();
forward pInfo_PlayerMessage(playerid, message[], hideMessage);
/*
	native pInfo_PlayerMessage(playerid, message[], hideMessage);
*/

// HOOKS
forward pInfo_OnPlayerConnect(playerid);
forward pInfo_OnGameModeInit();
forward pInfo_OnGameModeExit();
forward pInfo_OnFilterScriptInit();
forward pInfo_OnFilterScriptExit();

// OnPlayerConnect
public OnPlayerConnect(playerid) {
	for (new i=0; i<pInfo_MAX_MESSAGE_BOXES;i++) { pInfo_Info[playerid][i][pInfo_InfoCreated] = 0; }
	pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos] = 335.0;
	CallLocalFunction("pInfo_OnPlayerConnect","i", playerid);
	return 1;
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect pInfo_OnPlayerConnect

// OnFilterScriptInit
public OnFilterScriptInit() {
	pInfo_Init(), CallLocalFunction("pInfo_OnFilterScriptInit","");
	return 1;
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit pInfo_OnFilterScriptInit

// OnGameModeInit
public OnGameModeInit() {
	pInfo_Init(), CallLocalFunction("pInfo_OnGameModeInit","");
	return 1;
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit pInfo_OnGameModeInit

// OnGameModeExit
public OnGameModeExit() {
	pInfo_Exit(), CallLocalFunction("pInfo_OnGameModeExit","");
	return 1;
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit pInfo_OnGameModeExit

// OnFilterScriptExit
public OnFilterScriptExit() {
	pInfo_Exit(), CallLocalFunction("pInfo_OnFilterScriptExit","");
	return 1;
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit pInfo_OnFilterScriptExit

// Functions

public pInfo_PlayerMessage(playerid, message[], hideMessage) {
	if ( !IsPlayerConnected(playerid) ) { return -1; }
	new pInfo_hideTime = hideMessage * 1000;
	for (new i=0;i<pInfo_MAX_MESSAGE_BOXES;i++) {
		if ( pInfo_Info[playerid][i][pInfo_InfoCreated] == 0 ) {
			pInfo_Encode(message);
			pInfo_Info[playerid][i][pInfo_InfoCreated] = gettime();
			format(pInfo_Info[playerid][i][pInfo_InfoMessage], pInfo_MAX_MESSAGE_LENGTH, message);
			pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos] -= 25.0;
			pInfo_createPlayerTextDraw(playerid, i, pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos], message);
			#if defined pInfo_PlaySound
				PlayerPlaySound(playerid, pInfo_PlaySound, 0,0,0);
			#endif
			SetTimerEx("pInfo_DestroyMessage", pInfo_hideTime, 0, "ii", playerid, i);
			return 1;
		}
	}
	return 0;
}

public pInfo_Exit() {
	for (new i=0,m=GetMaxPlayers();i<m;i++) {
		if ( IsPlayerConnected(i) ) {
			for (new x=0; x<pInfo_MAX_MESSAGE_BOXES;x++) { pInfo_DestroyMessage(i, x); }
		}
	}
	return 1;
}
public pInfo_Init() {
	for (new i=0,m=GetMaxPlayers();i<m;i++) { pInfo_PlayerInfo[i][pInfo_PlayerInfoLastXPos] = 335.0; }
	return 1;
}
public pInfo_createPlayerTextDraw(playerid, messageID, Float:x, message[]) {
	pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos] = pInfo_Info[playerid][messageID][pInfo_InfoPosX] = x;
	pInfo_Info[playerid][messageID][pInfo_InfoBoxPID] = CreatePlayerTextDraw(playerid, 20.0, x, "_");
	PlayerTextDrawBackgroundColor(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 255);
	PlayerTextDrawFont(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 1);
	PlayerTextDrawLetterSize(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 0.5, 2.0);
	PlayerTextDrawColor(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], -1);
	PlayerTextDrawSetOutline(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 0);
	PlayerTextDrawSetProportional(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 1);
	PlayerTextDrawSetShadow(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 1);
	PlayerTextDrawUseBox(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 1);
	PlayerTextDrawBoxColor(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 150);
	PlayerTextDrawTextSize(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID], 155.0, 0.0);
	PlayerTextDrawShow(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID]);
	
	pInfo_Info[playerid][messageID][pInfo_InfoMsgPID] = CreatePlayerTextDraw(playerid,22.0, x + 1.0, message);
	PlayerTextDrawBackgroundColor(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 255);
	PlayerTextDrawFont(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 1);
	PlayerTextDrawLetterSize(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 0.20, 0.8);
	PlayerTextDrawColor(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], -1);
	PlayerTextDrawSetOutline(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 0);
	PlayerTextDrawSetProportional(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 1);
	PlayerTextDrawSetShadow(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID], 1);
	PlayerTextDrawShow(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID]);
	return 1;
}

public pInfo_reCreatePlayerTextDraw(playerid, messageID) {
	PlayerTextDrawDestroy(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID]);
	PlayerTextDrawDestroy(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID]);
	pInfo_createPlayerTextDraw(playerid, messageID, pInfo_Info[playerid][messageID][pInfo_InfoPosX], pInfo_Info[playerid][messageID][pInfo_InfoMessage]);
	return 1;
}

public pInfo_DestroyMessage(playerid, messageID) {
	if ( !IsPlayerConnected(playerid) ) { return 1; }
	if ( pInfo_Info[playerid][messageID][pInfo_InfoCreated] > 0 ) {
		PlayerTextDrawDestroy(playerid, pInfo_Info[playerid][messageID][pInfo_InfoBoxPID]);
		PlayerTextDrawDestroy(playerid, pInfo_Info[playerid][messageID][pInfo_InfoMsgPID]);
		pInfo_Info[playerid][messageID][pInfo_InfoCreated] = 0;
		pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos] += 25.0;
		new pInfo_fix = 0, pInfo_fixid = -1, pInfo_found[500];
		for (new i=0;i<pInfo_MAX_MESSAGE_BOXES;i++) {
			if ( pInfo_Info[playerid][i][pInfo_InfoCreated] > 0 ) {
				if ( pInfo_Info[playerid][i][pInfo_InfoPosX] < 300.0 ) { pInfo_Info[playerid][i][pInfo_InfoPosX] += 25.0; }
				pInfo_found[floatround(pInfo_Info[playerid][i][pInfo_InfoPosX])] ++;
				if ( pInfo_found[floatround(pInfo_Info[playerid][i][pInfo_InfoPosX])] >= 2 ) { pInfo_fix = 1; pInfo_fixid = i; }
				pInfo_reCreatePlayerTextDraw(playerid, i);
			}
		}
		if ( pInfo_fix == 1 ) {
			new Float:last =pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos];
			pInfo_Info[playerid][pInfo_fixid][pInfo_InfoPosX] -= 25.0;
			pInfo_reCreatePlayerTextDraw(playerid, pInfo_fixid);
			pInfo_PlayerInfo[playerid][pInfo_PlayerInfoLastXPos] = last;
		}
	}
	return 1;
}

stock pInfo_Encode(string[]) {
	new original[50] = {192,193,194,196,198,199,200,201,202,203,204,205,206,207,210,211,212,214,217,218,219,220,223,224,225,226,228,230,231,232,233,234,235,236,237,238,239,242,243,244,246,249,250,251,252,209,241,191,161,176};
	new fixed[50] = {128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,94,124};
	new len = strlen(string);
	for (new i; i < len; i++) {
		for(new j;j < 50;j++) {
			if(string[i] == original[j]) {
				string[i] = fixed[j];
				break;
			}
		}
	}
}