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

#ifndef VALVESERVERLIST_H
#define VALVESERVERLIST_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QUdpSocket>
#include <QTime>
#include <QFile>
#include <QStringList>
#include <QThread>

#include "Global.h"

#define LAN_REQUEST	    "\xFF\xFF\xFF\xFF\x69" //A2A_PING
#define LAN_REQUEST_SIZE    5

#define READ_COMMAND	    1
#define READ_UDP	    2
#define END		    3

#define VALVEMSQ_TIMEOUT    3000    //default steam timeout 3000

class ValveServerList : public QThread
{
	Q_OBJECT

	//TODO: da rimuovere una volta implementati i settings globali
	struct ServerAddress
	{
	quint32 ipv4addr;
	quint16 port;
	};
	QList<ServerAddress> m_serverList;
	quint32 m_currentMS;
	//

	struct Messaggio2
	{
	ValveEngine engine;
	int type;
	quint8 requestId;
	ServerType serverType;
	quint32 napp;
	QString gamedir;
	QString map;
	bool secure;
	bool isLinux;
	bool noEmpty;
	bool noFull;
	bool proxy;
	bool noPlayer;
	bool white;
	};

	QMutex m_stopMutex;
	QMutex m_commandMutex;

	QUdpSocket *m_socket;
	qint8 m_state;
	QTime m_timeout;

	bool m_stop;

	Messaggio2 m_command;
	Messaggio2 *m_new_command;

	//build Master Server Query
	QByteArray buildVMSQ();
	QByteArray buildVMSQ(QString ip, quint16 port);

	void loadMSData();
	void bindPort();
	bool isStopped();
	bool newCommand();
	void nextMasterServer();
	bool readUdp(QByteArray &datagram, QHostAddress &sender, quint16 &senderPort);
	void getIpPort(QByteArray rawData, QString &addr, quint16 &port);
	ServerAddress getMSAddress();

protected:
	void run();

public:
	explicit ValveServerList(QObject *parent = 0);
	~ValveServerList();
	void getServerList(ValveEngine engine, quint8 requestId, ServerType type, quint32 napp, QString gamedir, QString map, bool secure, bool isLinux, bool noEmpty, bool noFull, bool proxy, bool noPlayer, bool white);
	void getLANServerList(quint8 requestId);
	void stopList();

signals:
	void server(ValveEngine engine, quint8 requestId, quint32 ipv4addr, quint16 port);
	void endOfList();
};

#endif // VALVESERVERLIST_H
