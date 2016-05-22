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

#include <QtGui/QApplication>
#include <QTextCodec>

#include "Global.h"
#include "ValveGame.h"
#include "mainwindow.h"

QList<Game*> gameList;

void initGames()
{
	gameList.append( new ValveGame(100,	"Age of Chivalry",							"AoC",				QIcon("images/game-icons/age_of_chivalry.png"),				AGE_OF_CHIVALRY,			"ageofchivalry",					SOURCE) );
	gameList.append( new ValveGame(101,	"Alien Swarm",								"AlienSwarm",		QIcon("images/game-icons/alien_swarm.png"),					ALIEN_SWARM,				"swarm",							SOURCE) );
	gameList.append( new ValveGame(102,	"Counter Strike",							"cs",				QIcon("images/game-icons/counter_strike.png"),				COUNTER_STRIKE,				"cstrike",							GOLDSRC) );
	gameList.append( new ValveGame(103,	"Counter Strike: Source",					"css",				QIcon("images/game-icons/counter_strike_source.png"),		COUNTER_STRIKE_SOURCE,		"cstrike",							SOURCE) );
	gameList.append( new ValveGame(104,	"Counter Strike: Source Beta",				"cssbeta",			QIcon("images/game-icons/counter_strike_source.png"),		COUNTER_STRIKE_SOURCE_BETA,	"cstrike_beta",						SOURCE) );
	gameList.append( new ValveGame(105,	"D.I.P.R.I.P. Warm Up",						"DIPRIP",			QIcon("images/game-icons/diprip_warm_up.png"),				DIPRIP,						"diprip",							SOURCE) );
	gameList.append( new ValveGame(106,	"Day of Defeat",							"dod",				QIcon("images/game-icons/day_of_defeat.png"),				DAY_OF_DEFEAT,				"dod",								GOLDSRC) );
	gameList.append( new ValveGame(107,	"Day of Defeat: Source",					"dods",				QIcon("images/game-icons/day_of_defeat_source.png"),		DAY_OF_DEFEAT_SOURCE,		"dod",								SOURCE) );
	gameList.append( new ValveGame(108,	"Deathmatch Classic",						"dmc",				QIcon("images/game-icons/deathmatch_classic.png"),			DEATHMATCH_CLASSIC,			"dmc",								GOLDSRC) );
	gameList.append( new ValveGame(109,	"Dystopia",									"Dystopia",			QIcon("images/game-icons/dystopia.png"),					DYSTOPIA,					"dystopia",							SOURCE) );
	gameList.append( new ValveGame(110,	"Eternal-Silence",							"EternalSilence",	QIcon("images/game-icons/eternal_silence.png"),				ETERNAL_SILENCE,			"esmod",							SOURCE) );
	gameList.append( new ValveGame(111,	"Half-Life",								"hl",				QIcon("images/game-icons/half_life.png"),					HALF_LIFE,					"hl",								GOLDSRC) );
	gameList.append( new ValveGame(112,	"Half-Life 2",								"hl2",				QIcon("images/game-icons/half_life_2.png"),					HALF_LIFE_2,				"hl2",								SOURCE) );
	gameList.append( new ValveGame(113,	"Half-Life 2: Deathmatch",					"hl2dm",			QIcon("images/game-icons/half_life_2.png"),					HALF_LIFE_2_DM,				"hl2mp",							SOURCE) );
	gameList.append( new ValveGame(114,	"Insurgency",								"Insurgency",		QIcon("images/game-icons/insurgency.png"),					INSURGENCY,					"insurgency",						SOURCE) );
	gameList.append( new ValveGame(115,	"Left 4 Dead",								"l4d",				QIcon("images/game-icons/left4dead.png"),					LEFT4DEAD,					"left4dead",						SOURCE) );
	gameList.append( new ValveGame(116,	"Left 4 Dead 2",							"l4d2",				QIcon("images/game-icons/left4dead2.png"),					LEFT4DEAD2,					"left4dead2",						SOURCE) );
	gameList.append( new ValveGame(117,	"Killing Floor Beta",						"kfbeta",			QIcon("images/game-icons/killing_floor.png"),				KILLING_FLOOR_BETA,			"killingfloorbetadedicatedserver",	SOURCE) );
	gameList.append( new ValveGame(118,	"Moonbase Alpha",							"MoonbaseAlpha",	QIcon("images/game-icons/moonbase_alpha.png"),				MOONBASE_ALPHA,				"moonbasealpha",					SOURCE) );
	gameList.append( new ValveGame(119,	"Opposing Force",							"hlof",				QIcon("images/game-icons/opposing_force.png"),				OPPOSING_FORCE,				"gearbox",							GOLDSRC) );
	gameList.append( new ValveGame(120,	"Pirates, Vikings, and Knights II",			"pvkii",			QIcon("images/game-icons/pirates_vikings_knights_2.png"),	PIRATES_VIKINGS_KNIGHTS_2,	"pvkii",							SOURCE) );
	gameList.append( new ValveGame(121,	"Ricochet",									"Ricochet",			QIcon("images/game-icons/ricochet.png"),					RICOCHET,					"ricochet",							GOLDSRC) );
	gameList.append( new ValveGame(122,	"Serious Sam HD: The First Encounter Demo",	"SSHDdemo",			QIcon("images/game-icons/serious_sam_hd.png"),				SERIOUS_SAM_HD,				"serioussamhd",						SOURCE) );
	gameList.append( new ValveGame(123,	"Smashball",								"Smashball",		QIcon("images/game-icons/smashball.png"),					SMASHBALL,					"smashball",						SOURCE) );
	gameList.append( new ValveGame(124,	"Synergy",									"Synergy",			QIcon("images/game-icons/synergy.png"),						SYNERGY,					"synergy",							SOURCE) );
	gameList.append( new ValveGame(125,	"Team Fortress Classic",					"tfc",				QIcon("images/game-icons/team_fortress_classic.png"),		TEAM_FORTRESS,				"tf",								GOLDSRC) );
	gameList.append( new ValveGame(126,	"Team Fortress 2",							"tfc2",				QIcon("images/game-icons/team_fortress_2.png"),				TEAM_FORTRESS_2,			"tf",								SOURCE) );
	gameList.append( new ValveGame(127,	"Zombie Panic! Source",						"zps",				QIcon("images/game-icons/zombie_panic_source.png"),			ZOMBIE_PANIC_SOURCE,		"zps",								SOURCE) );
}

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

	QApplication a(argc, argv);

	initGames();

	MainWindow w;
	w.show();

	return a.exec();
}
