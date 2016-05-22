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

#ifndef VALVEINTERFACE_H
#define VALVEINTERFACE_H

#include <QObject>

#include "ValveServerList.h"
#include "ValveServerQuery.h"
#include "Global.h"
#include "IpToCountry.h"

class ValveInterface : public QObject
{
	Q_OBJECT

	QList<ValveServerQuery*> vsq_list;
	ValveServerList *vsl;

	QMutex m_requestMutex;
	QMutex UIMutex;
	QMutex BGMutex;
	QQueue<Messaggio> UIQueue;
	QQueue<Messaggio> BGQueue;

	QList<Server> serverList;

	quint8 m_requestId;

	IpToCountry *itc;

	quint8 newRequestId();
	quint8 currentRequestId();
	bool checkRequestId(quint8 requestId);

public:
	explicit ValveInterface(int threads, QObject *parent = 0);
	~ValveInterface();

	void getServerList(ValveEngine engine, ServerType type, quint32 napp, QString gamedir, QString map, bool secure, bool isLinux, bool noEmpty, bool noFull, bool proxy, bool noPlayer, bool white);
	void getLANServerList();
	void refreshServerList();
	void stopList();
	void getServerInfo(ValveEngine engine, quint32 ipv4addr, quint16 port);
	void getPlayers(ValveEngine engine, quint32 ipv4addr, quint16 port);
	void getRules(ValveEngine engine, quint32 ipv4addr, quint16 port);
	void getPing(ValveEngine engine, quint32 ipv4addr, quint16 port);

signals:
	void serverListCompleted(quint32 server_count);
	void serverInfo(ServerInfo *info);
	void serverInfoUI(ServerInfo *info);
	void playerList(QList<PlayerInfo> players);
	void ruleList(QList<Rule> rules);
	void pingInfo(qint32 ping);

private slots:
	void newServer(ValveEngine engine, quint8 requestId, quint32 addr, quint16 port);
	void serverListCompleted();

	void chkServerInfo(quint8 requestId, ServerInfo *info);
	void chkServerInfoUI(quint8 requestId, ServerInfo *info);
	void chkPlayerList(quint8 requestId, QList<PlayerInfo> info);
	void chkRuleList(quint8 requestId, QList<Rule> info);
	void chkPingInfo(quint8 requestId, qint32 info);
};

#endif // VALVEINTERFACE_H
