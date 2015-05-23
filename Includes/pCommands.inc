/*
	native GetServerTicks();
	natvie GetVehicleName(modelid);
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
new _pCommands_VehicleNames[212][] = {"Landstalker", "Bravura", "Buffalo", "Linerunner", "Perrenial","Sentinel", "Dumper", "Feuerwehr", "Trashmaster", "Stretch",
"Manana","Infernus", "Voodoo", "Pony", "Mule", "Cheetah", "Krankenwagen", "Leviathan", "Moonbeam", "Esperanto","Taxi", "Washington", "Bobcat", "Eiswagen", "BF Injection", "Hunter", "Premier", "Enforcer", "Securicar", "Banshee",
"Predator", "Bus", "Rhino", "Barracks", "Hotknife", "Trailer 1", "Previon", "Coach", "Cabbie", "Stallion","Rumpo", "RC Bandit", "Romero", "Packer", "Monster", "Admiral", "Squalo", "Seasparrow", "Pizzaboy", "Tram",
"Trailer 2", "Turismo", "Speeder", "Reefer", "Tropic", "Flatbed", "Yankee", "Caddy", "Solair","Topfun Van", "Skimmer", "PCJ-600", "Faggio", "Freeway", "RC Baron", "RC Raider", "Glendale", "Oceanic", "Sanchez",
"Sparrow","Patriot", "Quad", "Coastguard", "Dinghy", "Hermes", "Sabre", "Rustler", "ZR-350", "Walton","Regina", "Comet", "BMX", "Burrito", "Camper", "Marquis", "Baggage", "Dozer", "Maverick",
"News Chopper", "Rancher", "FBI Rancher", "Virgo", "Greenwood", "Jetmax", "Hotring", "Sandking", "Blista Compact", "Police Maverick","Boxville", "Benson", "Mesa", "RC Goblin", "Hotring Racer A", "Hotring Racer B",
"Bloodring Banger", "Rancher", "Super GT", "Elegant", "Journey", "Bike", "Mountain Bike", "Beagle", "Cropduster","Stunt",  "Tanker","Roadtrain", "Nebula", "Majestic", "Buccaneer", "Shamal", "Hydra", "FCR-900", "NRG-500", "HPV1000", "Cement Truck", "Tow Truck", "Fortune","Cadrona", "FBI Truck", "Willard", "Forklift", "Tractor", "Combine", "Feltzer", "Remington", "Slamvan", "Blade", "Freight", "Streak",
"Vortex", "Vincent", "Bullet", "Clover", "Sadler", "Firetruck LA", "Hustler", "Intruder", "Primo", "Cargobob", "Tampa", "Sunrise", "Merit","Utility", "Nevada", "Yosemite", "Windsor", "Monster A", "Monster B", "Uranus", "Jester", "Sultan", "Stratum", "Elegy", "Raindance","RC Tiger", "Flash", "Tahoma", "Savanna", "Bandito", "Freight Flat", "Streak Carriage", "Kart", "Mower", "Dune", "Sweeper", "Broadway",
"Tornado", "AT-400", "DFT-30", "Huntley", "Stafford", "BF-400", "News Van", "Tug", "Trailer 3", "Emperor", "Wayfarer", "Euros", "Hotdog","Club", "Freight Carriage", "Trailer 4", "Andromada", "Dodo", "RC Cam", "Launch", "Police Car (LSPD)", "Police Car (SFPD)","Police Car (LVPD)", "Police Ranger", "Picador", "S.W.A.T", "Alpha", "Phoenix", "Glendale Shit", "Sadler", "Luggage Trailer A","Luggage Trailer B", "Stairs", "Boxville", "Tiller", "Utility Trailer"};
/*
	HOOKS
*/
forward pCommands_OnPlayerConnect(playerid);
forward pCommands_OnPlayerDisconnect(playerid, reason);
/*
	OnPlayerConnect
*/
public OnPlayerConnect(playerid) {
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
	GetVehicleName(modelid)
	return name of modelid
	2015 by derpierre65
*/
stock GetVehicleName(modelid) {
	new ret[32];
	strcat(ret, _pCommands_VehicleNames[modelid-400]);
	return ret;
}
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