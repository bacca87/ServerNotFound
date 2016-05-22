//
//  This file is part of Server Not Found (SNF), an advanced cross-platform
//  game server browser that works with dozens of online games.
//
//  Authors (Alphabetically ordered):
//	Copyright (C) 2010  Marco "SNaKE" Baccarani, <bacca87@gmail.com>
//	Copyright (C) 2010  Riccardo "vortex" Pedrielli, <vortex@ngi.it>
//
//  Server Not Found is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Server Not Found is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Server Not Found.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMetaType>

//Steam Games						//AppID		//Description
#define AGE_OF_CHIVALRY					17510		//Age of Chivalry
#define ALIEN_SWARM						630			//Alien Swarm
#define COUNTER_STRIKE					10			//Counter Strike
#define COUNTER_STRIKE_SOURCE			240			//Counter Strike: Source
#define COUNTER_STRIKE_SOURCE_BETA		260			//Counter Strike: Source Beta
#define DIPRIP							17530		//D.I.P.R.I.P. Warm Up
#define DAY_OF_DEFEAT					30			//Day of Defeat
#define DAY_OF_DEFEAT_SOURCE			300			//Day of Defeat: Source
#define DEATHMATCH_CLASSIC				40			//Deathmatch Classic
#define DYSTOPIA						17580		//Dystopia
#define ETERNAL_SILENCE					17550		//Eternal-Silence
#define HALF_LIFE						70			//Half-Life
#define HALF_LIFE_2						220			//Half-Life 2
#define HALF_LIFE_2_DM					320			//Half-Life 2: Deathmatch
#define INSURGENCY						17700		//Insurgency
#define LEFT4DEAD						500			//Left 4 Dead
#define LEFT4DEAD2						550			//Left 4 Dead 2
#define KILLING_FLOOR_BETA				1270		//Killing Floor Beta
#define MOONBASE_ALPHA					39000		//Moonbase Alpha
#define OPPOSING_FORCE					50			//Opposing Force
#define PIRATES_VIKINGS_KNIGHTS_2		17570		//Pirates, Vikings, and Knights II
#define RICOCHET						60			//Ricochet
#define SERIOUS_SAM_HD					41000		//Serious Sam HD: The First Encounter Demo
#define SMASHBALL						17730		//Smashball
#define SYNERGY							17520		//Synergy
#define TEAM_FORTRESS_2					440			//Team Fortress 2
#define TEAM_FORTRESS					20			//Team Fortress Classic
#define ZOMBIE_PANIC_SOURCE				17500		//Zombie Panic! Source

#define A2S_INFO                        0x49
#define A2S_INFO_GOLDSRC                0x6D
#define A2A_PING                        0x6A
#define A2S_SERVERQUERY_GETCHALLENGE    0x41
#define A2S_PLAYER                      0x44
#define A2S_PLAYER_REQUEST              0x55
#define A2S_RULES                       0x45
#define A2S_RULES_REQUEST               0x56

#define GLOBAL_LIST			0
#define LOCAL_LIST			1

enum GameProtocol { STEAM };
enum ServerType { ANY = 0, LISTEN = 0x6C, DEDICATED = 0x64, HLTV = 0x70 };
enum ValveEngine { GOLDSRC, SOURCE };
enum OSType { LINUX = 0x6C, WINDOWS = 0x77 };

struct CountryInfo
{
	QString ctry;
	QString cntry;
	QString country;
};

struct ServerInfo
{
	CountryInfo country;
	qint32 ping;
	quint32 ipv4addr;
	quint16 port;
	quint8 version;
	QString name;
	QString map;
	QString gameDirectory;
	QString gameDescription;
	quint16 appID;
	quint8 players;
	quint8 maxPlayers;
	quint8 bots;
	quint8 dedicated;		//'l' for listen, 'd' for dedicated, 'p' for SourceTV
	quint8 os;				//Host operating system. 'l' for Linux, 'w' for Windows
	bool password;
	/*
	goldsrc mod info
	quint8 isMod;			//If set to 0x01, this byte is followed by ModInfo
	QString URLInfo;		//URL containing information about this mod
	QString URLDL;			//URL to download this mod
	quint32 ModVersion;		//Version of the installed mod
	quint32 ModSize;		//The download size of this mod
	quint8 SvOnly;			//If 1 this is a server side only mod
	quint8 ClDLL;			//If 1 this mod has a custom client dll
	*/
	bool secure;
	QString gameVersion;

	quint8 extraDataFlag;

	quint16 serverGamePort;
	quint64 steamID;
	quint16 tvPort;
	QString tvName;
	QString tags;
	quint16 extraAppID;

	ServerInfo()
	{
		//cleaning the optional properties
		serverGamePort = 0;
		steamID = 0;
		tvPort = 0;
		extraAppID = 0;
	}
};

Q_DECLARE_METATYPE(ServerInfo)

struct PlayerInfo
{
	QString name;
	qint32 kills;
	qreal time_s;   //the time in seconds this player has been connected
};

Q_DECLARE_METATYPE(PlayerInfo)

struct Rule
{
	QString name;
	QString value;
};

Q_DECLARE_METATYPE(Rule)

struct Server
{
	ValveEngine engine;
	quint32 ipv4addr;
	quint16 port;
};

Q_DECLARE_METATYPE(Server)

struct Messaggio
{
	ValveEngine engine;
	quint8 requestId;
	int type;
	quint32 ip;
	quint16 port;
};

#endif // GLOBAL_H
