#define FILTERSCRIPT
#define MAX_SPIKES   500 // Maximale Nagelbänder
#define MAX_SPERRUNG 500 // Maximale Straßensperren

#include <a_samp>

#define ERROR 0xC70000FF // Fehlermeldung Color (Standart: 0xC70000FF (ROT))
#define INFO  0x00FF35FF // Information Color (Standart: 0x00FF35FF (GRÜN))

new Float:x[MAX_SPIKES];
new Float:y[MAX_SPIKES];
new Float:z[MAX_SPIKES];
new Float:rz[MAX_SPIKES];
new spike[MAX_SPIKES];
new Spikeplatt[MAX_SPIKES];
new nbpid[MAX_SPIKES];
new Spikez = 0;
new string[100];

new Float:Ax;
new Float:Ay;
new Float:Az;
new Float:Arz;
new Absperrung[MAX_SPERRUNG];
new Absperrungz = 0;

#if defined FILTERSCRIPT

public OnFilterScriptExit()
{
	new i;
	for(;i<MAX_SPIKES;i++)
	{
	DestroyObject(spike[i]);
	}
	return 1;
}

#endif

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (!strcmp("/createsperre", cmdtext) || !strcmp("/csperre", cmdtext) )
	{
		if ( IsACop(playerid) == 1)
		{
		    if ( Absperrungz < MAX_SPERRUNG ) {
				Absperrungz ++;
		        GetPlayerPos(playerid,Ax,Ay,Az);
		        GetPlayerFacingAngle(playerid, Arz);
		        Absperrung[Absperrungz] = CreateObject(1228, Ax, Ay, Az-0.50, 0, 0, Arz+90);
		        format(string, sizeof(string), "Straßensperre %d wurde platziert!",Absperrungz);
		        SendClientMessage(playerid, INFO, string);
	        }
	        else { SendClientMessage(playerid, ERROR, "Die maximale Anzahl Absperrungen wurden erreicht."); }
        }
        else { SendClientMessage(playerid, ERROR, "Du bist kein Polizist"); }
        return 1;

	}
    if (!strcmp("/cnb", cmdtext) || !strcmp("/createnb", cmdtext) )
    {
        if ( IsACop(playerid) == 1 )
		{
	        if(Spikez < MAX_SPIKES)
	        {
	            Spikez ++;
		        GetPlayerPos(playerid,x[Spikez],y[Spikez],z[Spikez]);
		        GetPlayerFacingAngle(playerid, rz[Spikez]);
		        spike[Spikez] = CreateObject(2892, x[Spikez], y[Spikez], z[Spikez]-0.8, 0, 0, rz[Spikez]+90);
		        format(string, sizeof(string), "Nagelband Nr. %d wurde gelegt!",Spikez);
		        SendClientMessage(playerid, INFO, string);
		        Spikeplatt[Spikez] = 1;
		        nbpid[Spikez] = playerid;
	        }
	        else { SendClientMessage(playerid, ERROR, "Die maximale Anzahl der Nagelbänder wurden platziert."); }
		}
		else { SendClientMessage(playerid, ERROR, "Du bist kein Polizist"); }
        return 1;
    }
	if (!strcmp("/deleteall", cmdtext) || !strcmp("/dall", cmdtext))
	{
		if ( IsACop(playerid) == 1 )
		{
			format(string, sizeof(string), "Alle Nagelbänder (Insgesamt: %d) wurden entfernt!",Spikez);
			SendClientMessage(playerid, INFO, string);
			format(string, sizeof(string), "Alle Straßensperren (Insgesamt: %d) wurden entfernt!",Absperrungz);
			SendClientMessage(playerid, INFO, string);
			Absperrungz += 1;
			for(new i=0;i<MAX_SPIKES;i++) { DestroyObject(Absperrung[i]); }
			for(new i=0;i<MAX_SPERRUNG;i++) { DestroyObject(spike[i]); }
		}
		else { SendClientMessage(playerid, ERROR, "Du bist kein Polizist."); }
		return 1;
	}
	return 0;
}

encode_tires(tire1, tire2, tire3, tire4) {
	return tire1 | (tire2 << 1) | (tire3 << 2) | (tire4 << 3);
}

public OnPlayerUpdate(playerid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		for(new i;i<MAX_SPIKES;i++)
		{
			if ( spike[i] ) {
				if(IsPlayerInRangeOfPoint(playerid, 3.0, x[i], y[i], z[i]))
				{
					if (Spikeplatt[i] == 1)
					{
					    UpdateVehicleDamageStatus(GetPlayerVehicleID(playerid), 0, 0, 0, encode_tires(1, 1, 1, 1));
					    DestroyObject(spike[i]);
					    new name[MAX_PLAYER_NAME];
			    		GetPlayerName(playerid, name, sizeof(name));
					    format(string, sizeof(string), "%s ist durch dein Nagelband gefahren.",name);
						SendClientMessage(nbpid[i], INFO, string);
					    Spikeplatt[i] = 0;
					    break;
				    }
			    }
		    }
		}
    }
    return 1;
}

stock IsACop(playerid) {
	for (new id=280; id <= 288; id++) {
		if ( GetPlayerSkin(playerid) == id ) { return 1; }
	}

	return 0;
}
