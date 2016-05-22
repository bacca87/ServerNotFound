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

#ifndef VALVESERVERQUERY_H
#define VALVESERVERQUERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QQueue>
#include <QMutex>
#include <QThread>

#include "Global.h"

#define READ_MESSAGE_QUEUE      1
#define READ_UDP                2
#define DATA_HANDLING           3

#define VALVESQ_TIMEOUT         3000

#define PLAYER_REQUEST          "\xFF\xFF\xFF\xFF\x55\x00\x00\x00\x00"
#define PLAYER_REQUEST_SIZE     9
#define RULES_REQUEST           "\xFF\xFF\xFF\xFF\x56\x00\x00\x00\x00"
#define RULES_REQUEST_SIZE      9
#define PING_REQUEST            "\xFF\xFF\xFF\xFF\x69"
#define PING_REQUEST_SIZE       5
#define INFO_REQUEST            "\xFF\xFF\xFF\xFF\x54Source Engine Query\x00"
#define INFO_REQUEST_SIZE       25

class ValveServerQuery : public QThread
{
	Q_OBJECT

	QQueue<Messaggio> *m_queue;
	QMutex *m_mutex;
	QUdpSocket *m_socket;
	qint8 m_state;
	quint8 m_requestId;

	//splitted datagrams handling
	qint32 m_lastSplitID;
	quint8 m_lastSplitTot;
	QList<QByteArray> m_datagrams;

	void bindPort();
	bool readQueue(Messaggio &msg);
	bool readUdp(QByteArray &datagram, QHostAddress &sender, quint16 &senderPort);

protected:
	void run();

public:
	explicit ValveServerQuery(QQueue<Messaggio> *queue, QMutex *mutex, QObject *parent = 0);
	~ValveServerQuery();

signals:
	void a2s_info(quint8 requestId, ServerInfo *info);
	void a2s_player(quint8 requestId, QList<PlayerInfo> info);
	void a2s_rules(quint8 requestId, QList<Rule> rules);
	void a2a_ping(quint8 requestId, qint32 msec);
};

#endif // VALVESERVERQUERY_H
