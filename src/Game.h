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

#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QIcon>

#include "Global.h"

class Game
{
	QString m_name;
	QString m_alias;
	quint32 m_uid;
	QIcon m_icon;
	GameProtocol m_protocol;

public:
	explicit Game(quint32 uid, GameProtocol protocol, QString name, QString alias, QIcon icon) :
			m_uid(uid), m_protocol(protocol), m_name(name), m_alias(alias), m_icon(icon) {}

	QString getName() { return m_name; }
	QString getAlias() { return m_alias; }
	quint32 getUID() { return m_uid; }
	GameProtocol getProtocol() { return m_protocol; }
	QIcon getIcon() { return m_icon; }
};

#endif // GAME_H
