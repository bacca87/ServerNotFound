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

#include "ValveInterface.h"

ValveInterface::ValveInterface(int threads, QObject *parent) :
	QObject(parent)
{
	m_requestId = 0;

	vsl = new ValveServerList();
	vsl->start();

	qRegisterMetaType<ValveEngine>("ValveEngine");

	connect(vsl, SIGNAL(server(ValveEngine, quint8, quint32, quint16)), this, SLOT(newServer(ValveEngine, quint8, quint32, quint16)));
	connect(vsl, SIGNAL(endOfList()), this, SLOT(serverListCompleted()));

	itc = new IpToCountry("IpToCountry.csv");

//TODO: progettare una gestione piu furba dei threads
	qRegisterMetaType<ServerInfo>("ServerInfo");
	qRegisterMetaType<QList<PlayerInfo> >("QList<PlayerInfo>"); //lasciare lo spazio dopo QList<PlayerInfo> se no non compila
	qRegisterMetaType<QList<Rule> >("QList<Rule>");

	for(int i = 0; i < threads; i++)
	{
		ValveServerQuery *p = new ValveServerQuery(&BGQueue, &BGMutex);
		connect(p, SIGNAL(a2s_info(quint8, ServerInfo*)), this, SLOT(chkServerInfo(quint8, ServerInfo*)));
		p->start();
		vsq_list.append(p);
	}

	for(int i = 0; i < 3; i++)
	{
		ValveServerQuery *p = new ValveServerQuery(&UIQueue, &UIMutex);

		connect(p, SIGNAL(a2s_info(quint8, ServerInfo*)), this, SLOT(chkServerInfoUI(quint8, ServerInfo*)));
		connect(p, SIGNAL(a2s_player(quint8, QList<PlayerInfo>)), this, SLOT(chkPlayerList(quint8, QList<PlayerInfo>)));
		connect(p, SIGNAL(a2s_rules(quint8, QList<Rule>)), this, SLOT(chkRuleList(quint8, QList<Rule>)));
		connect(p, SIGNAL(a2a_ping(quint8, qint32)), this, SLOT(chkPingInfo(quint8, qint32)));

		p->start();
		vsq_list.append(p);
	}
}

ValveInterface::~ValveInterface()
{
	delete vsl;

	for(int i = 0; i< vsq_list.length(); i++)
	{
		vsq_list.at(i)->terminate();
		delete vsq_list.at(i);
	}
}

quint8 ValveInterface::newRequestId()
{
	quint8 tmp;

	m_requestMutex.lock();
	if(m_requestId == 255)
		m_requestId = 0;
	else
		m_requestId++;

	tmp = m_requestId;
	m_requestMutex.unlock();

	return tmp;
}

quint8 ValveInterface::currentRequestId()
{
	quint8 tmp;

	m_requestMutex.lock();
	tmp = m_requestId;
	m_requestMutex.unlock();

	return tmp;
}

bool ValveInterface::checkRequestId(quint8 requestId)
{
	m_requestMutex.lock();
	bool ok = m_requestId == requestId;
	m_requestMutex.unlock();

	return ok;
}

void ValveInterface::getLANServerList()
{
	stopList();
	serverList.clear();

	vsl->getLANServerList(currentRequestId());
}

void ValveInterface::getServerList(ValveEngine engine, ServerType type, quint32 napp, QString gamedir, QString map, bool secure, bool isLinux, bool noEmpty, bool noFull, bool proxy, bool noPlayer, bool white)
{
	stopList();
	serverList.clear();

	vsl->getServerList(engine, currentRequestId(), type, napp, gamedir, map, secure, isLinux, noEmpty, noFull, proxy, noPlayer, white);
}

void ValveInterface::refreshServerList()
{
	stopList();

	BGMutex.lock();
	for(int i = 0; i < serverList.length(); i++)
	{
		Messaggio tmpMsg = {serverList.at(i).engine, currentRequestId(), A2S_INFO, serverList.at(i).ipv4addr, serverList.at(i).port};
		BGQueue.enqueue(tmpMsg);
	}
	BGMutex.unlock();
}

void ValveInterface::stopList()
{
	newRequestId();

	vsl->stopList();

	BGMutex.lock();
	BGQueue.clear();
	BGMutex.unlock();
}

void ValveInterface::newServer(ValveEngine engine, quint8 requestId, quint32 addr, quint16 port)
{
	if(checkRequestId(requestId))
	{
		//check temporaneo
		/*for(int i = 0; i < serverList.length(); i++)
		{
			if(serverList.at(i).ipv4addr == addr && serverList.at(i).port == port)
			{
				//stopList();
				qDebug()<< "SERVER GIA ESISTENTE IN LISTA!!!";
				//qDebug()<< QHostAddress(addr).toString() + ":" + QString::number(port);
				//return;
			}
		}*/
		//qDebug()<< QHostAddress(addr).toString() + ":" + QString::number(port);
		//

		//Messaggio msg = {engine, requestId, (engine == SOURCE ? A2S_INFO : A2S_INFO_GOLDSRC), addr, port};
		Messaggio msg = {engine, requestId, A2S_INFO, addr, port};
		Server srv = {engine, addr, port};
		serverList.append(srv);

		BGMutex.lock();
		BGQueue.enqueue(msg);
		BGMutex.unlock();
	}
}

void ValveInterface::serverListCompleted()
{
	emit serverListCompleted(serverList.count());
}

void ValveInterface::chkPingInfo(quint8 requestId, qint32 info)
{
	if(checkRequestId(requestId))
		emit pingInfo(info);
}

void ValveInterface::chkPlayerList(quint8 requestId, QList<PlayerInfo> info)
{
	if(checkRequestId(requestId))
		emit playerList(info);
}

void ValveInterface::chkRuleList(quint8 requestId, QList<Rule> info)
{
	if(checkRequestId(requestId))
		emit ruleList(info);
}

void ValveInterface::chkServerInfo(quint8 requestId, ServerInfo *info)
{
	if(checkRequestId(requestId))
	{
		info->country = itc->getIPv4Info(info->ipv4addr);
		emit serverInfo(info);
	}
}

void ValveInterface::chkServerInfoUI(quint8 requestId, ServerInfo *info)
{
	if(checkRequestId(requestId))
	{
		info->country = itc->getIPv4Info(info->ipv4addr);
		emit serverInfoUI(info);
	}
}

void ValveInterface::getServerInfo(ValveEngine engine, quint32 ipv4addr, quint16 port)
{
	//Messaggio msg = {engine, currentRequestId(), (engine == SOURCE ? A2S_INFO : A2S_INFO_GOLDSRC), ipv4addr, port};
	Messaggio msg = {engine, currentRequestId(), A2S_INFO, ipv4addr, port};

	UIMutex.lock();
	UIQueue.enqueue(msg);
	UIMutex.unlock();
}

void ValveInterface::getPlayers(ValveEngine engine, quint32 ipv4addr, quint16 port)
{
	Messaggio msg = {engine, currentRequestId(), A2S_PLAYER, ipv4addr, port};

	UIMutex.lock();
	UIQueue.enqueue(msg);
	UIMutex.unlock();
}

void ValveInterface::getRules(ValveEngine engine, quint32 ipv4addr, quint16 port)
{
	Messaggio msg = {engine, currentRequestId(), A2S_RULES, ipv4addr, port};

	UIMutex.lock();
	UIQueue.enqueue(msg);
	UIMutex.unlock();
}

void ValveInterface::getPing(ValveEngine engine, quint32 ipv4addr, quint16 port)
{
	Messaggio msg = {engine, currentRequestId(), A2A_PING, ipv4addr, port};

	UIMutex.lock();
	UIQueue.enqueue(msg);
	UIMutex.unlock();
}
