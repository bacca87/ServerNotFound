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

#include "ValveServerQuery.h"
#include "Utility.h"

ValveServerQuery::ValveServerQuery(QQueue<Messaggio> *queue, QMutex *mutex, QObject *parent) :
	QThread(parent), m_queue(queue), m_mutex(mutex)
{
}

ValveServerQuery::~ValveServerQuery()
{
	delete m_socket;
}

void ValveServerQuery::run()
{
	Messaggio command;
	QByteArray datagram;
	QHostAddress sender;
	quint16 senderPort;
	bool splitted = false;
	QTime ping;
	QTime timeout;
	quint8 lastChallengeRequest;
	QList<PlayerInfo> playerList;
	QList<Rule> ruleList;

	ServerInfo *info;

	m_socket = new QUdpSocket();
	bindPort();

	m_state = READ_MESSAGE_QUEUE;

	while(true)
	{
		int next = 0;
		PlayerInfo player;
		Rule rule;

		switch(m_state)
		{
		case READ_MESSAGE_QUEUE:
			if(readQueue(command))
			{
				//qDebug()<< "NUOVO SERVER";
				m_requestId = command.requestId;

				//sending query
				switch(command.type)
				{
				case A2A_PING:
					if(m_socket->writeDatagram(PING_REQUEST, PING_REQUEST_SIZE, QHostAddress(command.ip), command.port) == -1)
					{
						qDebug() << m_socket->error();
						qDebug() << m_socket->errorString();
						qDebug() << m_socket->state();
					}
					ping.start();
					break;
				case A2S_INFO_GOLDSRC:
				case A2S_INFO:
					if(m_socket->writeDatagram(INFO_REQUEST, INFO_REQUEST_SIZE, QHostAddress(command.ip), command.port) == -1)
					{
						qDebug() << m_socket->error();
						qDebug() << m_socket->errorString();
						qDebug() << m_socket->state();
					}
					ping.start();
					break;
				case A2S_PLAYER:
					if(command.engine == GOLDSRC)
					{
						if(m_socket->writeDatagram(INFO_REQUEST, INFO_REQUEST_SIZE, QHostAddress(command.ip), command.port) == -1)
						{
							qDebug() << m_socket->error();
							qDebug() << m_socket->errorString();
							qDebug() << m_socket->state();
						}
					}
					else //SOURCE
					{
						lastChallengeRequest = A2S_PLAYER_REQUEST; //TODO: provare a gestire tenendo conto anche dell'ip e della porta da cui arriva il pacco
						if(m_socket->writeDatagram(PLAYER_REQUEST, PLAYER_REQUEST_SIZE, QHostAddress(command.ip), command.port) == -1)
						{
							qDebug() << m_socket->error();
							qDebug() << m_socket->errorString();
							qDebug() << m_socket->state();
						}
					}
					break;
				case A2S_RULES:
					lastChallengeRequest = A2S_RULES_REQUEST;
					if(m_socket->writeDatagram(RULES_REQUEST, RULES_REQUEST_SIZE, QHostAddress(command.ip), command.port) == -1)
					{
						qDebug() << m_socket->error();
						qDebug() << m_socket->errorString();
						qDebug() << m_socket->state();
					}
					break;
				}

				m_state = READ_UDP;
				timeout.start();
			}
			break;
		case READ_UDP:
			if(timeout.elapsed() < VALVESQ_TIMEOUT)
			{
				if(readUdp(datagram, sender, senderPort))
				{
					if(splitted) //am i waiting splitted datagrams?
					{
						if(datagram[0] == 0xFE && Utility::getInt32(datagram.mid(4, 4)) == m_lastSplitID)
						{
							m_datagrams.append(datagram);

							if(m_datagrams.count() == m_lastSplitTot)
							{
								for(int i = 0; i < m_datagrams.count(); i++)
								{
									qint8 seq;
									if(command.engine == GOLDSRC)
										seq = (m_datagrams.at(i).at(8) & 0xF0) >> 4;
									else
										seq = m_datagrams.at(i).at(9);

									while(i != seq)
									{
										m_datagrams.swap(i, seq);
										seq = m_datagrams.at(i).at(9);
									}
								}

								datagram = m_datagrams.at(0);
								datagram.remove(0, (command.engine == GOLDSRC ? 9 : 12));

								for(int i = 1; i < m_datagrams.count(); i++)
								{
									QByteArray tmp;
									tmp = m_datagrams.at(i);
									tmp.remove(0, (command.engine == GOLDSRC ? 9 : 12));
									datagram+=tmp;
								}
								//the splitted datagram is now complete

								splitted = false;
								m_state = DATA_HANDLING;
							}
							else
							{
								m_state = READ_UDP;
								timeout.start();
							}
						}
						else
						{
							qDebug()<< "Errore splittato";
							m_state = READ_MESSAGE_QUEUE;
							splitted = false;
						}
					}
					else
					{
						if(datagram[0] == 0xFE) //splitted datagram
						{
							qDebug()<< "SPLITTATO";

							m_lastSplitID = Utility::getInt32(datagram.mid(4, 4));
							m_lastSplitTot = (qint8)datagram[8];

							if(command.engine == GOLDSRC)					//GOLDSRC: The lower four bits represent the number of packets (2 to 15)
								m_lastSplitTot = m_lastSplitTot & 0xF;			//and the upper four bits represent the current packet starting with 0.

							splitted = true;
							m_datagrams.clear();
							m_datagrams.append(datagram);
						}
						else
							m_state = DATA_HANDLING;
					}
				}
			}
			else
			{
				m_state = READ_MESSAGE_QUEUE;
				splitted = false;
				//qDebug()<< "TIMEOUT";
			}
			break;
		case DATA_HANDLING:
			//check if this is the expected datagram
			if(!((command.type == A2S_PLAYER || command.type == A2S_RULES) && datagram[4] == A2S_SERVERQUERY_GETCHALLENGE) &&
			   datagram[4] != command.type)
			{
				m_state = READ_UDP;
				timeout.start();
				break;
			}

			switch(datagram[4])
			{
			case A2S_INFO:
				info = new ServerInfo();
				info->ping = ping.elapsed();

				info->ipv4addr = sender.toIPv4Address();
				info->port = senderPort;

				next = 5;
				info->version = datagram[next++];
				info->name = Utility::getString(datagram, next, next);
				info->map = Utility::getString(datagram, next, next);
				info->gameDirectory = Utility::getString(datagram, next, next);
				info->gameDescription = Utility::getString(datagram, next, next);
				info->appID = Utility::getUInt16(datagram, next, next);
				info->players = datagram[next++];
				info->maxPlayers = datagram[next++];
				info->bots = datagram[next++];
				info->dedicated = datagram[next++];
				info->os = datagram[next++];
				info->password = datagram[next++];
				info->secure = datagram[next++];
				info->gameVersion = Utility::getString(datagram, next, next);

				info->extraDataFlag = datagram[next++];

				if(info->extraDataFlag & 0x80) //The server's game port # is included
				{
					info->serverGamePort = Utility::getUInt16(datagram, next, next);
				}
				if(info->extraDataFlag & 0x10) //The server's SteamID is included
				{
					info->steamID = Utility::getUInt64(datagram, next, next);
				}
				if(info->extraDataFlag & 0x40) //The spectator port # and then the spectator server name are included
				{
					info->tvPort = Utility::getUInt16(datagram, next, next);
					info->tvName = Utility::getString(datagram, next, next);
				}
				if(info->extraDataFlag & 0x20) //The game tag data string for the server is included
				{
					info->tags = Utility::getString(datagram, next, next);
				}
				if(info->extraDataFlag & 0x01) //The Steam Application ID again + several 0x00 bytes
				{
					info->extraAppID = Utility::getUInt16(datagram, next, next);
				}
				m_state = READ_MESSAGE_QUEUE;
				emit a2s_info(m_requestId, info);
				break;
//			case A2S_INFO_GOLDSRC:
//				info.ping = ping.elapsed();

//				info.ipv4addr = sender.toIPv4Address();
//				info.port = senderPort;

//				next = 5;
//				Utility::getString(datagram, next, next); //useless ip and port
//				info.name = Utility::getString(datagram, next, next);
//				info.map = Utility::getString(datagram, next, next);
//				info.gameDirectory = Utility::getString(datagram, next, next);
//				info.gameDescription = Utility::getString(datagram, next, next);
//				info.players = datagram[next++];
//				info.maxPlayers = datagram[next++];
//				info.version = datagram[next++];
//				info.dedicated = datagram[next++];
//				info.os = datagram[next++];
//				info.password = datagram[next++];
//				info.isMod = datagram[next++]; //ismod

//				if(info.isMod == 0x01)
//				{
//					info.URLInfo = Utility::getString(datagram, next, next);
//					info.URLDL = Utility::getString(datagram, next, next);
//					next++; //unused null byte
//					info.ModVersion = Utility::getInteger32(datagram, next, next);
//					info.ModSize = Utility::getInteger32(datagram, next, next);
//					info.SvOnly = datagram[next++];
//					info.ClDLL = datagram[next++];
//				}

//				info.secure = datagram[next++];
//				info.bots = datagram[next];
//				state = READ_UDP; //wait for player list
//				timeout.start();
//				emit a2s_info(requestId, info);
//				break;
			case A2A_PING:
				emit a2a_ping(m_requestId, ping.elapsed());
				m_state = READ_MESSAGE_QUEUE;
				break;
			case A2S_SERVERQUERY_GETCHALLENGE:
				datagram[4] = lastChallengeRequest;
				if(m_socket->writeDatagram(datagram, sender, senderPort) < 0)
				{
					qDebug() << m_socket->error();
					qDebug() << m_socket->errorString();
					qDebug() << m_socket->state();
				}
				m_state = READ_UDP;
				timeout.start();
				break;
			case A2S_PLAYER:
				playerList.clear();
				for(int next = 7; next < datagram.length(); next++)
				{
					player.name = Utility::getString(datagram, next, next);
					player.kills = Utility::getInt32(datagram, next, next);
					player.time_s = Utility::getReal32(datagram, next, next);

					playerList.append(player);
				}
				m_state = READ_MESSAGE_QUEUE;
				emit a2s_player(m_requestId, playerList);
				break;
			case A2S_RULES:
				ruleList.clear();
				for(int next = 8; next < datagram.length();)
				{
					rule.name = Utility::getString(datagram, next, next);
					rule.value = Utility::getString(datagram, next, next);

					ruleList.append(rule);
				}
				m_state = READ_MESSAGE_QUEUE;
				emit a2s_rules(m_requestId, ruleList);
				break;
			default:
				m_state = READ_MESSAGE_QUEUE;
			}
			break;
		}

		msleep(1);
	}
}

void ValveServerQuery::bindPort()
{
	int tmpPort = 1024;
	while( !m_socket->bind(++tmpPort) );
	qDebug() << "ValveSQ port " + QString::number(m_socket->localPort());
}

bool ValveServerQuery::readQueue(Messaggio &msg) //reading from the message queue
{
	bool ok = false;

	m_mutex->lock();
	if(!m_queue->isEmpty())
	{
		msg = m_queue->dequeue();
		ok = true;
	}
	m_mutex->unlock();

	return ok;
}

bool ValveServerQuery::readUdp(QByteArray &datagram, QHostAddress &sender, quint16 &senderPort)
{
	datagram.clear();

	if(m_socket->hasPendingDatagrams())
	{
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size(),
					 &sender, &senderPort);
		return true;
	}

	return false;
}
