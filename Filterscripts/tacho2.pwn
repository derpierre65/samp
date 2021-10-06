#include <a_samp>
#include <dini>

#define error 0xC30000FF
#define info  0x00C000FF
#define FILTERSCRIPT
#define dcmd(%1,%2,%3) if ((strcmp((%3)[1], #%1, true, (%2)) == 0) && ((((%3)[(%2) + 1] == 0) && (dcmd_%1(playerid, "")))||(((%3)[(%2) + 1] == 32) && (dcmd_%1(playerid, (%3)[(%2) + 2]))))) return 1
#define PREIS 				4	// Preis Pro liter
#define MAXFULL 			30	// Maximale Literanzahl für jedes Fahrzeug.
#define SECONDS 			60	// Nach wieviele Sekunden verbraucht ein Fahrzeug zusätzlich 0.1 Liter.
#define TANKDAUER			300 // Wielange dauert das Tanken PRO Liter. (0 = Sofort voll) (Millisekunden)
#define STANDARD			10	// StandarD Menge (Standard 10)
#define FOLDER				"pcars" // Ordner in dem die Cars gespeichert werden (StandarD: pcars). (Ordner muss in Scriptfiles erstellt werden)
#define START_MOTOR_KEY		132 // Taste bei dem der Motor startet (http://wiki.sa-mp.com/wiki/Keys) | Zeile entfernen oder auskommentieren um Funktion zu deaktivieren.
#define MOTOR_OFF_KMH       50 // Ab diese km/h Anzahl kann der Spieler den Motor !NICHT! mehr ausschalten, sondern muss langsamer werden.
#define LITER_PRO_KM        2 // Literverbrauch (für alle Autos) pro gefahrener Kilometer

forward Speedometer(playerid);
forward FillGas(i, playerid,price);
forward Gas();
forward checkGas();
forward IsAtGasStation(playerid);
enum vehicleData {
	vehicleSpawnID,
	vehicleColor1,
	vehicleColor2,
	Float:vehicleKm,
	vehicleTank,

	Float:vLastX,
	Float:vLastY,
	Float:vLastZ
};
new vehicles[MAX_VEHICLES][vehicleData];

new Text:Tacho[MAX_PLAYERS];
new timer[MAX_PLAYERS];
new Float:Tank[MAX_VEHICLES];
new engine,lights,alarm,doors,bonnet,boot,objective;
new bool:Motor[MAX_VEHICLES]=false;
new PlayerVehicle[212][] = {
"Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial", "Sentinel", "Dumper", "Firetruck", "Trashmaster", "Stretch", "Manana",
"Infernus", "Voodoo", "Pony", "Mule", "Cheetah", "Ambulance", "Leviathan", "Moonbeam", "Esperanto", "Taxi", "Washington", "Bobcat",
"Whoopee", "BF Injection", "Hunter", "Premier", "Enforcer", "Securicar", "Banshee", "Predator", "Bus", "Rhino", "Barracks", "Hotknife",
"Trailer 1", "Previon", "Coach", "Cabbie", "Stallion", "Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral", "Squalo",
"Seasparrow", "Pizzaboy", "Tram", "Trailer 2", "Turismo", "Speeder", "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair",
"Berkley's RC Van", "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale", "Oceanic", "Sanchez", "Sparrow",
"Patriot", "Quad", "Coastguard", "Dinghy", "Hermes", "Sabre", "Rustler", "ZR-350", "Walton", "Regina", "Comet", "BMX", "Burrito",
"Camper", "Marquis", "Baggage", "Dozer", "Maverick", "News Chopper", "Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring",
"Sandking", "Blista Compact", "Police Maverick", "Boxvillde", "Benson", "Mesa", "RC Goblin", "Hotring Racer A", "Hotring Racer B",
"Bloodring Banger", "Rancher", "Super GT", "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster","Stunt",  "Tanker",
"Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra", "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune",
"Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer", "Remington", "Slamvan", "Blade", "Freight", "Streak",
"Vortex", "Vincent", "Bullet", "Clover", "Sadler", "Firetruck LA", "Hustler", "Intruder", "Primo", "Cargobob", "Tampa", "Sunrise", "Merit",
"Utility", "Nevada", "Yosemite", "Windsor", "Monster A", "Monster B", "Uranus", "Jester", "Sultan", "Stratum", "Elegy", "Raindance",
"RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito", "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper", "Broadway",
"Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400", "News Van", "Tug", "Trailer 3", "Emperor", "Wayfarer", "Euros", "Hotdog",
"Club", "Freight Carriage", "Trailer 4", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car (LSPD)", "Police Car (SFPD)",
"Police Car (LVPD)", "Police Ranger", "Picador", "S.W.A.T", "Alpha", "Phoenix", "Glendale", "Sadler", "Luggage Trailer A",
"Luggage Trailer B", "Stairs", "Boxville", "Tiller", "Utility Trailer"
};

public OnFilterScriptInit()
{
    ManualVehicleEngineAndLights();
	for(new i =0; i<MAX_VEHICLES;i++) {
		Tank[i] = STANDARD;
	    loadCar(i);
		Motor[i]=false;
		GetVehicleParamsEx(i,engine,lights,alarm,doors,bonnet,boot,objective);
		SetVehicleParamsEx(i,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
	}
	timer[1] = SetTimer("Speedometer",1000, 1);
	timer[2] = SetTimer("Gas", SECONDS * 1000, 1);
	print("\n---------------------------------------------");
	print("   Tank, Motor und Tacho System bei pierre65   ");
	new file[255]; format(file,sizeof file, "%s",FOLDER);
	if ( !dini_Exists(file) ) {
		printf("Fehler: Der Ordner >%s< wurde nicht gefunden.",FOLDER);
	}
	print("-----------------------------------------------\n");
	for (new i=0; i<MAX_PLAYERS; i++) {
		Tacho[i] = TextDrawCreate(460.000000, 381.500000, " ");
		TextDrawBackgroundColor(Tacho[i], 255);
		TextDrawFont(Tacho[i], 1);
		TextDrawLetterSize(Tacho[i], 0.32, 0.97);
		TextDrawColor(Tacho[i], -1);
		TextDrawSetOutline(Tacho[i], 0);
		TextDrawSetProportional(Tacho[i], 1);
		TextDrawSetShadow(Tacho[i], 1);
		TextDrawHideForAll(Tacho[i]);
	}
	return 1;
}

public OnFilterScriptExit() {
	for (new i=0; i<MAX_PLAYERS; i++) { TextDrawDestroy(Tacho[i]); }
	for (new i=0; i<MAX_VEHICLES; i++) {
		saveCar(i);
	}
	return 1;
}

public OnVehicleSpawn(vehicleid) {
	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	SetVehicleParamsEx(vehicleid,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
	Motor[vehicleid] = false;
    return 0;
}

#if defined START_MOTOR_KEY
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if ( newkeys == (START_MOTOR_KEY) ) { toggleMotor(playerid); }
	return 1;
}
#endif

public OnPlayerConnect(playerid) {
	timer[playerid] = SetTimerEx("Speedometer",1000, 1,"i",playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
	KillTimer(timer[playerid]);
	return 1;
}

public Speedometer(playerid) {
    TextDrawHideForPlayer(playerid, Tacho[playerid]);
	if ( IsPlayerConnected(playerid) ) {
        if( IsPlayerInAnyVehicle(playerid) ) {
            TextDrawShowForPlayer(playerid, Tacho[playerid]);
        	new Float:chealth, speed_string[255], kmh, vehicleid = GetPlayerVehicleID(playerid), modelid = GetVehicleModel(vehicleid);
			GetVehicleHealth(vehicleid, chealth);
			kmh = getKmh(playerid, true);
			
			if ( GetPlayerState(playerid) == PLAYER_STATE_DRIVER ) {
				new Float:x2, Float:y2, Float:z2, Float:output;
		        GetVehiclePos(vehicleid, x2, y2, z2);
				output = floatsqroot(floatpower(floatabs(floatsub(x2, vehicles[vehicleid][vLastX])), 2)+floatpower(floatabs(floatsub(y2, vehicles[vehicleid][vLastY])), 2)+floatpower(floatabs(floatsub(z2, vehicles[vehicleid][vLastZ])), 2));
				new Float:rtank = output / 1000 * LITER_PRO_KM;
				Tank[vehicleid] -= rtank;
				vehicles[vehicleid][vehicleKm] += output;
			    vehicles[vehicleid][vLastX] = x2; vehicles[vehicleid][vLastY] = y2; vehicles[vehicleid][vLastZ] = z2;
			}
			format(speed_string,255,"~b~%s: ~w~%d%%~n~~b~Km/h: ~w~%d km/h~n~~b~Motor: %s~n~~b~Kilometer:~w~ %.2f~n~~b~Benzin: ~w~%.1f/%dl", PlayerVehicle[modelid - 400], floatround(chealth, floatround_round)/10,kmh, Motor[vehicleid]?("~g~An"):("~r~Aus"),(vehicles[vehicleid][vehicleKm]/1000), Tank[vehicleid], MAXFULL);
			TextDrawSetString(Tacho[playerid], speed_string);
        }
    }
    return 1;
}

public Gas() {
	new vehicleid;
	for (new i=0; i < MAX_VEHICLES;i++) {
	    for ( new p=0; p < MAX_PLAYERS; p++ ) {
		    if ( IsPlayerConnected(p) ) {
		        vehicleid = GetPlayerVehicleID(p);
	            vehicleid = GetPlayerVehicleID(p);
	            if ( vehicleid == i ) {
	                if( Tank[i] <= 4 && Tank[i] >= 1 ) { PlayerPlaySound(p, 1085, 0.0, 0.0, 0.0); }
				}
		    }
		}
		if ( Motor[i] == true ) {
			Tank[i] -= 0.1;
			if ( Tank[i] <= 0 ) { Motor[i] = false; SetVehicleParamsEx(i, VEHICLE_PARAMS_OFF, lights,alarm,doors,bonnet,boot,objective); }
	    }
	}
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate) {
    if( newstate == PLAYER_STATE_DRIVER) {
        new vehicle = GetPlayerVehicleID(playerid);
		GetVehicleParamsEx(vehicle,engine,lights,alarm,doors,bonnet,boot,objective);
        if(GetVehicleModel(vehicle) == 509 || GetVehicleModel(vehicle) == 481 || GetVehicleModel(vehicle) == 510) { SetVehicleParamsEx(vehicle,VEHICLE_PARAMS_ON,lights,alarm,doors,bonnet,boot,objective); }
        else { SendClientMessage(playerid, info, "Schalte den Motor mit an oder aus."); }
        if ( Motor[vehicle]==false ) { SetVehicleParamsEx(vehicle,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective); }
    	if( Tank[vehicle] <= 0 ) {
       	    Motor[vehicle]=false;
       	    Tank[vehicle] = 0;
       	    SetVehicleParamsEx(vehicle,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
        	GameTextForPlayer(playerid,"~r~~n~~n~~n~~n~~n~~n~~n~~n~Der Tank ist leer!",3000,3);
		}
    }
    return 1;
}

public IsAtGasStation(playerid) {
    if(IsPlayerConnected(playerid))	{
		if(IsPlayerInRangeOfPoint(playerid,15.0,1004.0070,-939.3102,42.1797) || IsPlayerInRangeOfPoint(playerid,15.0,1944.3260,-1772.9254,13.3906)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,-90.5515,-1169.4578,2.4079) || IsPlayerInRangeOfPoint(playerid,15.0,-1609.7958,-2718.2048,48.5391)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,-2029.4968,156.4366,28.9498) || IsPlayerInRangeOfPoint(playerid,15.0,-2408.7590,976.0934,45.4175)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,-2243.9629,-2560.6477,31.8841) || IsPlayerInRangeOfPoint(playerid,6.0,-1676.6323,414.0262,6.9484)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,2202.2349,2474.3494,10.5258) || IsPlayerInRangeOfPoint(playerid,15.0,614.9333,1689.7418,6.6968)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,-1328.8250,2677.2173,49.7665) || IsPlayerInRangeOfPoint(playerid,15.0,70.3882,1218.6783,18.5165)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,2113.7390,920.1079,10.5255) || IsPlayerInRangeOfPoint(playerid,15.0,-1327.7218,2678.8723,50.0625)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,2146.6143,2748.4758,10.3852)||IsPlayerInRangeOfPoint(playerid,15.0,2639.0022,1108.0353,10.3852)) { return 1; }
		else if(IsPlayerInRangeOfPoint(playerid,15.0,1598.2035,2198.6448,10.3856)) { return 1; }
	}
	return 0;
}

stock getKmh(playerid,bool:kmh) {
    new Float:x,Float:y,Float:z,Float:rtn;
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid),x,y,z); else GetPlayerVelocity(playerid,x,y,z);
    rtn = floatsqroot((x*x)+(y*y)+(z*z));
    return kmh?floatround(rtn * 85 * 1.61):floatround(rtn * 50);
}

public FillGas(i, playerid,price) {
	new str[50];
	if ( floatround(Tank[i],floatround_ceil) < MAXFULL ) {
	    if (IsAtGasStation(playerid) && Motor[i] == false ) {
			format(str, sizeof str, "Du hast nicht genug Geld für ein weiteren Liter. (Kosten: %d$)", PREIS);
	        if ( GetPlayerMoney(playerid) >= PREIS ) {
				Tank[i] ++;
				SetTimerEx("FillGas", TANKDAUER * 1, 0, "iii", i, playerid,price + PREIS);
				GivePlayerMoney(playerid, -PREIS);
				GameTextForPlayer(playerid, "~w~~n~~n~~n~~n~~n~~n~~n~~n~Dein Fahrzeug wird betankt...",TANKDAUER,3);
			}
			else { SendClientMessage(playerid,error, str); }
		}
		else { format(str, sizeof str, "Das Tanken wurde beendet."); SendClientMessage(playerid,error, str); }
	}
	else { format(str, sizeof str, "Dein Fahrzeug wurde für %d$ betankt!",price); SendClientMessage(playerid, info, str); }
	return 1;
}

forward loadCar(carid);
public loadCar(carid) {
	new file[255]; format(file,sizeof file, "%s/pcar_%d.cfg",FOLDER,carid);
	if ( !dini_Exists(file) ) { return 0; }
	new model = dini_Int(file,"model");
	vehicles[carid][vehicleKm] = dini_Float(file,"km");
	vehicles[carid][vehicleColor1] = dini_Int(file,"color1");
	vehicles[carid][vehicleColor2] = dini_Int(file,"color2");
	Tank[carid] = dini_Float(file,"tank");
	if ( Tank[carid] > MAXFULL ) { Tank[carid] = MAXFULL; }
	Tank[carid] -= 5.0;
	new Float:x = dini_Float(file,"float_x");
	new Float:y = dini_Float(file,"float_y");
	new Float:z = dini_Float(file,"float_z");
	new Float:r = dini_Float(file,"float_r");
	vehicles[carid][vehicleSpawnID] = CreateVehicle(model,x,y,z,r,vehicles[carid][vehicleColor1],vehicles[carid][vehicleColor2],0);
	GetVehiclePos(vehicles[carid][vehicleSpawnID],x,y,z);
    vehicles[carid][vLastX] = x; vehicles[carid][vLastY] = y; vehicles[carid][vLastZ] = z;
	return 1;
}

forward saveCar(carid);
public saveCar(carid) {
	if ( GetVehicleModel(carid) > 0 ) {
		new file[255]; format(file,sizeof file, "%s/pcar_%d.cfg",FOLDER,carid);
		if ( !dini_Exists(file) ) { dini_Create(file); }
		dini_IntSet(file,"model", GetVehicleModel(carid));
		dini_IntSet(file,"color1", vehicles[carid][vehicleColor1]);
		dini_IntSet(file,"color2", vehicles[carid][vehicleColor2]);
		dini_FloatSet(file,"tank", Tank[carid]);
		dini_FloatSet(file,"km", vehicles[carid][vehicleKm]);
		new Float:x,Float:y,Float:z,Float:r;
		GetVehiclePos(carid, x,y,z);
		GetVehicleZAngle(carid,r);
		dini_FloatSet(file,"float_x",x);
		dini_FloatSet(file,"float_y",y);
		dini_FloatSet(file,"float_z",z);
		dini_FloatSet(file,"float_r",r);
		DestroyVehicle(carid);
	}
	return 1;
}

forward toggleMotor(playerid);
public toggleMotor(playerid) {
	if(GetPlayerVehicleSeat(playerid) == 0) {
		new car = GetPlayerVehicleID(playerid);
		if(GetVehicleModel(car) == 509 || GetVehicleModel(car) == 481 || GetVehicleModel(car) == 510) { SendClientMessage(playerid, error, "Ein Fahrrad hat kein Motor."); }
		else if (Motor[car] == false) {
			GetVehicleParamsEx(car,engine,lights,alarm,doors,bonnet,boot,objective);
			if ( Tank[car] >= 1 ) { Motor[car] = true; SetVehicleParamsEx(car,VEHICLE_PARAMS_ON,lights,alarm,doors,bonnet,boot,objective); }
			else { Motor[car] = false; SetVehicleParamsEx(car,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective); GameTextForPlayer(playerid,"~w~~n~~n~~n~~n~~n~~n~~n~~n~Der Tank ist leer!",3000,3); }
		}
		else {
		    #if defined MOTOR_OFF_KMH
	        if ( getKmh(playerid,true) > MOTOR_OFF_KMH ) { return SendClientMessage(playerid, error, "Du fährst zu schnell um den Motor abzuschalten."); }
		    #endif
			GetVehicleParamsEx(car,engine,lights,alarm,doors,bonnet,boot,objective);
			SetVehicleParamsEx(car,VEHICLE_PARAMS_OFF,lights,alarm,doors,bonnet,boot,objective);
			Motor[car] = false;
		}
	}
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[]) {
    dcmd(tanken,6,cmdtext);
    dcmd(motor, 5,cmdtext);
	return 0;
}

dcmd_tanken(playerid, params[]) {
    #pragma unused params
	if(IsPlayerInAnyVehicle(playerid)) {
	    if(IsAtGasStation(playerid)) {
			new vehicle = GetPlayerVehicleID(playerid);
			new liter = MAXFULL; liter -= floatround(Tank[vehicle],floatround_floor);
			printf("%d %f",liter,liter);
			if ( Motor[vehicle] == true ) { SendClientMessage(playerid, error, "Schalte den Motor vor dem Tanken bitte aus."); }
			else {
	            if(GetPlayerMoney(playerid) > liter*PREIS) {
					TogglePlayerControllable(playerid, 1);
					SetTimerEx("FillGas", 1000, 0, "iii", vehicle, playerid,0);
					GameTextForPlayer(playerid, "~w~~n~~n~~n~~n~~n~~n~~n~~n~Dein Fahrzeug wird betankt...",TANKDAUER,3);
	      		}
	  			else  { SendClientMessage(playerid, error, "Du hast nicht genug Geld."); }
  			}
		}
		else { SendClientMessage(playerid, error, "Du bist an keiner Tankstelle."); }
	}
	return 1;
}

dcmd_motor(playerid, params[]) {
	#pragma unused params
	toggleMotor(playerid);
	return 1;
}
