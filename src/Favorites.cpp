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

#include "Favorites.h"

Favorites::Favorites(QObject *parent) :
	QObject(parent)
{
}

bool Favorites::load(QString filePath)
{
	QString errorStr;
	int errorLine;
	int errorColumn;

	file.setFileName(filePath);

	if(!file.open(QFile::ReadOnly | QFile::Text))
	{
		qDebug() << "Error: Cannot read the favorites file " + filePath + ": " + file.errorString();
		return false;
	}

	if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
	{
		qDebug() << "XML Error: Parse error at line " + QString::number(errorLine) + ", column " + QString::number(errorColumn) + ": " + errorStr;
		file.close();
		return false;
	}

	file.close();

	QDomElement root = doc.documentElement();

	if(root.tagName() != "favorites")
	{
		qDebug() << "Error: invalid favorites file";
		return false;
	}

	return true;
}

void Favorites::save()
{
	file.open(QFile::WriteOnly);
	QTextStream out(&file);

	doc.save(out, 4);
	file.close();
}

QDomNode Favorites::getGameNode(Game game)
{
	QDomNode gameNode = doc.documentElement().firstChild();

	while(!gameNode.isNull())
	{
		if(gameNode.toElement().tagName() == "game" && gameNode.toElement().attribute("id").toInt() == game.getUID())
			return gameNode;

		gameNode = gameNode.nextSibling();
	}

	QDomElement newGameNode = doc.createElement("game");
	newGameNode.setAttribute("id", game.getUID());
	newGameNode.setAttribute("name", game.getName());
	doc.documentElement().appendChild(newGameNode);

	return newGameNode;
}

bool Favorites::findDuplicatedServer(Game game, quint32 ipv4addr, quint16 port)
{
	QDomNode gameNode = doc.documentElement().firstChild();

	while(!gameNode.isNull())
	{
		if(gameNode.toElement().tagName() == "game" && gameNode.toElement().attribute("id").toInt() == game.getUID())
		{
			QDomNode serverNode = gameNode.firstChild();

			while(!serverNode.isNull())
			{
				if(serverNode.toElement().tagName() == "server" &&
				   serverNode.toElement().attribute("ip") == QHostAddress(ipv4addr).toString() &&
				   serverNode.toElement().attribute("port").toInt() == port)
					return true;

				serverNode = serverNode.nextSibling();
			}
			return false;
		}

		gameNode = gameNode.nextSibling();
	}

	return false;
}

FavoritesList Favorites::getList(Game game)
{
	QDomNode gameNode = doc.documentElement().firstChild();

	list.clear();

	while(!gameNode.isNull())
	{
		if(gameNode.toElement().tagName() == "game" && gameNode.toElement().attribute("id").toInt() == game.getUID())
		{
			QDomNode serverNode = gameNode.firstChild();

			while(!serverNode.isNull())
			{
				if(serverNode.toElement().tagName() == "server")
				{
					Server tmpSrv;
					tmpSrv.ipv4addr = QHostAddress(serverNode.toElement().attribute("ip")).toIPv4Address();
					tmpSrv.port = serverNode.toElement().attribute("port").toInt();

					list.append(tmpSrv);
				}

				serverNode = serverNode.nextSibling();
			}
			return list;
		}

		gameNode = gameNode.nextSibling();
	}

	return list;
}

void Favorites::add(Game game, quint32 ipv4addr, quint16 port)
{
	if(findDuplicatedServer(game, ipv4addr, port))
		return;

	QDomElement server = doc.createElement("server");
	server.setAttribute("ip", QHostAddress(ipv4addr).toString());
	server.setAttribute("port", port);

	getGameNode(game).appendChild(server);

	save();
}

void Favorites::remove(Game game, quint32 ipv4addr, quint16 port)
{
	QDomNode gameNode = doc.documentElement().firstChild();

	while(!gameNode.isNull())
	{
		if(gameNode.toElement().tagName() == "game" && gameNode.toElement().attribute("id").toInt() == game.getUID())
		{
			QDomNode serverNode = gameNode.firstChild();

			while(!serverNode.isNull())
			{
				if(serverNode.toElement().tagName() == "server" &&
				   serverNode.toElement().attribute("ip") == QHostAddress(ipv4addr).toString() &&
				   serverNode.toElement().attribute("port").toInt() == port)
				{
					gameNode.removeChild(serverNode);
					save();
					return;
				}
				serverNode = serverNode.nextSibling();
			}
			return;
		}
		gameNode = gameNode.nextSibling();
	}
	return;
}
