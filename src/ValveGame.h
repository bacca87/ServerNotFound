//
//  This file is part of Server Not Found (SNF), an advanced cross-platform
//  game server browser that works with dozens of online games.
//
//  Authors (Alphabetically ordered):
//	Copyright (C) 2010  Marco "SNaKE" Baccarani, <bacca87@gmail.com>
//	Copyright (C) 2010  Riccardo "vort3x" Pedrielli, <vort3x@ngi.it>
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

#ifndef VALVEGAME_H
#define VALVEGAME_H

#include "Game.h"

class ValveGame : public Game
{
	quint16 m_appid;
	QString m_gamedir;
	ValveEngine m_engine;

public:
	explicit ValveGame(quint32 uid, QString name, QString alias, QIcon icon, quint16 appid, QString gamedir, ValveEngine engine ) :
			m_appid(appid), m_gamedir(gamedir), m_engine(engine), Game(uid, STEAM, name, alias, icon) {}

	quint16 getAppid() { return m_appid; }
	QString getGameDir() { return m_gamedir; }
	ValveEngine getEngine() { return m_engine; }
};

#endif // VALVEGAME_H
