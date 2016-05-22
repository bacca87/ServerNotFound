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

#ifndef FAVORITES_H
#define FAVORITES_H

#include <QObject>
#include <QFile>
#include <QXmlStreamReader>
#include <QDomDocument>
#include <QHostAddress>

#include "Global.h"
#include "Game.h"
//TODO: guardare se posso utilizzare un altra struttura simile o rendere questa standard
//struct Server
//{
//	quint32 ipv4addr;
//	quint16 port;
//};

typedef QList<Server> FavoritesList;

class Favorites : public QObject
{
	Q_OBJECT

	QXmlStreamReader reader;
	QDomDocument doc;
	FavoritesList list;
	QFile file;

	QDomNode getGameNode(Game game);
	bool findDuplicatedServer(Game game, quint32 ipv4addr, quint16 port);
	void save();

public:
	explicit Favorites(QObject *parent = 0);

	bool load(QString filePath);

	FavoritesList getList(Game game);
	void add(Game game, quint32 ipv4addr, quint16 port);
	void remove(Game game, quint32 ipv4addr, quint16 port);
};

#endif // FAVORITES_H
