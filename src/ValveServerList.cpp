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

#include "ValveServerList.h"

ValveServerList::ValveServerList(QObject *parent) :
	QThread(parent)
{
	m_new_command = NULL;
	m_stop = false;
	loadMSData(); //TODO: caricare i dati dalla futura classe setting generale
}

ValveServerList::~ValveServerList()
{
	delete m_socket;
	delete m_new_command;
}

void ValveServerList::bindPort()
{
	int tmpPort = 1024;
	while( !m_socket->bind(++tmpPort) );
	qDebug() << "ValveQMS port " + QString::number(m_socket->localPort());
}

void ValveServerList::run()
{
	QByteArray datagram;
	QHostAddress sender;
	quint16 senderPort;
	QString m_lastAddress = "0.0.0.0";
	quint16 m_lastPort = 0;
	m_currentMS = 0;
	quint32 retry = 0;
	m_state = READ_COMMAND;
	m_socket = new QUdpSocket();
	bindPort();

	while(true)
	{
		switch(m_state)
		{
		case READ_COMMAND:
			if(newCommand())
			{
				switch(m_command.type)
				{
				case GLOBAL_LIST:
					/*
					 ServerType type, bool secure, QString gamedir, QString map, bool linux, bool noEmpty, bool noFull, bool proxy, quint32 napp, bool noPlayer, bool white

					 \type\d
					 Servers running dedicated
					 \secure\1
					 Servers using anti-cheat technology (VAC, but potentially others as well)
					 \gamedir\[mod]
					 Servers running the specified modification (ex. cstrike)
					 \map\[map]
					 Servers running the specified map (ex. cs_italy)
					 \linux\1
					 Servers running on a Linux platform
					 \empty\1
					 Servers that are not empty
					 \full\1
					 Servers that are not full
					 \proxy\1
					 Servers that are spectator proxies
					 \napp\[appid]
					 (mettere 500 default)Servers that are NOT running game [appid] (This was introduced to block Left 4 Dead games from the Steam Server Browser)
					 \noplayers\1
					 Servers that are empty
					 \white\1
					 Servers that are whitelisted
					*/

					if(m_socket->writeDatagram(buildVMSQ(), QHostAddress(m_serverList[m_currentMS].ipv4addr), m_serverList[m_currentMS].port) == -1)
					{
						//TODO: gestire gli errori
						qDebug() << m_socket->error();
						qDebug() << m_socket->errorString();
						qDebug() << m_socket->state();
					}

					m_state = READ_UDP;
					m_timeout = QTime::currentTime();
					break;

				case LOCAL_LIST:
					/*
					Steam LAN port ranges:
					from  |  to
					27015 | 27020
					26900 | 26905
					4242  | 4242
					27215 | 27215
					*/

					//TODO: pensare se è meglio un array statico per le porte
					for(quint16 port = 27015; port < 27021; port++)
						m_socket->writeDatagram(LAN_REQUEST, LAN_REQUEST_SIZE, QHostAddress::Broadcast, port);

					for(quint16 port = 26900; port < 26906; port++)
						m_socket->writeDatagram(LAN_REQUEST, LAN_REQUEST_SIZE, QHostAddress::Broadcast, port);

					m_socket->writeDatagram(LAN_REQUEST, LAN_REQUEST_SIZE, QHostAddress::Broadcast, 4242);
					m_socket->writeDatagram(LAN_REQUEST, LAN_REQUEST_SIZE, QHostAddress::Broadcast, 27215);

					m_state = READ_UDP;
					m_timeout = QTime::currentTime();
					break;
				}
			}
			break;

		case READ_UDP:
			if(!isStopped())
			{
				if(m_timeout.msecsTo(QTime::currentTime()) < VALVEMSQ_TIMEOUT)
				{
					if(readUdp(datagram, sender, senderPort))
					{
						retry = 0;

						switch(m_command.type)
						{
						case LOCAL_LIST:
							if(datagram[4] == 0x6A) //LAN server ping responce
								emit server(m_command.engine, m_command.requestId, sender.toIPv4Address(), senderPort);
							else
								m_state = END;
							break;

						case GLOBAL_LIST:
							if((int)datagram[4] == 0x66 && (int)datagram[5] == 0x0A)
							{
								if(m_lastPort != 0)
								{
									//datagram sequence check
									QString tmpAddr;
									quint16 tmpPort;

									getIpPort(datagram.mid((m_command.engine == SOURCE ? 6 : 12), 6), tmpAddr, tmpPort);

									if(tmpAddr != m_lastAddress && tmpPort != m_lastPort)
									{
										qDebug()<<"SEQUENZA SCAZZATA!!!";
										m_state = END;
										break;
									}

									datagram.remove((m_command.engine == SOURCE ? 6 : 12), 6);
								}

								for(int i = 6; i < datagram.length(); i += 6)
								{
									getIpPort(datagram.mid(i, 6), m_lastAddress, m_lastPort);

									if(m_lastAddress != "0.0.0.0" && !isStopped()) //The end of the IP list is indicated by a received IP address of 0.0.0.0 Port:0
									{
										emit server(m_command.engine, m_command.requestId, QHostAddress(m_lastAddress).toIPv4Address(), m_lastPort);
									}
									else
									{
										m_state = END;
										break;
									}
								}

								m_socket->writeDatagram(buildVMSQ(m_lastAddress, m_lastPort), QHostAddress(m_serverList.at(m_currentMS).ipv4addr), m_serverList.at(m_currentMS).port);
								m_timeout = QTime::currentTime();
							}
							break;
						}
					}
				}
				else
				{
					retry++;
					if(retry == 3)
					{
						if(m_command.engine == GOLDSRC)
						{
							nextMasterServer();
							retry = 0;
						}
						//else
							//basta tentativi
					}

					m_socket->writeDatagram(buildVMSQ(m_lastAddress, m_lastPort), QHostAddress(m_serverList.at(m_currentMS).ipv4addr), m_serverList.at(m_currentMS).port);
					m_timeout = QTime::currentTime();
					qDebug()<<"---UPDATE---";
				}
			}
			else
			{
				m_state = END;
				break;
			}
			break;

		case END:
			m_lastAddress = "0.0.0.0";
			m_lastPort = 0;
			retry = 0;
			m_state = READ_COMMAND;

			m_stopMutex.lock();
			m_stop = false;
			m_stopMutex.unlock();

			//Cambio MS perche se no i pacchetti della ricerca precedente rimasti nel buffer di lettura
			//fanno scazzare la sequenza della nuova ricerca
			nextMasterServer();
			//TODO: sarebbe meglio che per l'engine GOLDSRC l'indice dei master server ripartisse sempre da 0

			emit endOfList();
			break;
		}

		msleep(1);
	}
}

void ValveServerList::nextMasterServer()
{
	if(m_currentMS + 1 < m_serverList.length())
		m_currentMS++;
	else
		m_currentMS = 0;
}

bool ValveServerList::newCommand()
{
	m_commandMutex.lock();
	bool ok = m_new_command != NULL;
	if(ok)
	{
		m_command = *m_new_command;
		delete m_new_command;
		m_new_command = NULL;
	}
	m_commandMutex.unlock();

	return ok;
}

bool ValveServerList::readUdp(QByteArray &datagram, QHostAddress &sender, quint16 &senderPort)
{
	datagram.clear();

	if(m_socket->hasPendingDatagrams())
	{
		datagram.resize(m_socket->pendingDatagramSize());
		m_socket->readDatagram(datagram.data(), datagram.size(),
					 &sender, &senderPort);

		if(QHostAddress(m_serverList.at(m_currentMS).ipv4addr) != sender)
		{
			datagram.clear();
			return false;
		}
		else
			return true;
	}

	return false;
}

void ValveServerList::stopList()
{
	m_stopMutex.lock();
	if(m_state == READ_UDP)
		m_stop = true;
	m_stopMutex.unlock();
}

bool ValveServerList::isStopped()
{
	bool tmp;
	m_stopMutex.lock();
	tmp = m_stop;
	m_stopMutex.unlock();
	return tmp;
}

void ValveServerList::getLANServerList(quint8 requestId)
{
	stopList();

	m_commandMutex.lock();

	if(m_new_command != NULL)
		delete m_new_command;

	m_new_command = new Messaggio2();
	m_new_command->type = LOCAL_LIST;
	m_new_command->requestId = requestId;

	m_commandMutex.unlock();
}

void ValveServerList::getServerList(ValveEngine engine, quint8 requestId, ServerType type, quint32 napp, QString gamedir, QString map, bool secure, bool isLinux, bool noEmpty, bool noFull, bool proxy, bool noPlayer, bool white)
{
	stopList();

	m_commandMutex.lock();

	if(m_new_command != NULL)
		delete m_new_command;

	m_new_command = new Messaggio2();
	m_new_command->engine = engine;
	m_new_command->type = GLOBAL_LIST;
	m_new_command->requestId = requestId;
	m_new_command->serverType = type;
	m_new_command->napp = napp;
	m_new_command->gamedir = gamedir;
	m_new_command->map = map;
	m_new_command->secure = secure;
	m_new_command->isLinux = isLinux;
	m_new_command->noEmpty = noEmpty;
	m_new_command->noFull = noFull;
	m_new_command->proxy = proxy;
	m_new_command->noPlayer = noPlayer;
	m_new_command->white = white;

	m_commandMutex.unlock();
}

QByteArray ValveServerList::buildVMSQ()
{
	return buildVMSQ("0.0.0.0", 0);
}

QByteArray ValveServerList::buildVMSQ(QString ip, quint16 port)
{
	QByteArray msg;
	QByteArray filters;

	if(m_command.serverType != ANY)
		filters.append(QString("\\type\\") + (unsigned char)m_command.serverType);
	if(m_command.gamedir != "")
		filters.append("\\gamedir\\" + m_command.gamedir);
	if(m_command.map != "")
		filters.append("\\map\\" + m_command.map);
	if(m_command.secure)
		filters.append("\\secure\\1");
	if(m_command.isLinux)
		filters.append("\\linux\\1");
	if(m_command.noEmpty)
		filters.append("\\empty\\1");
	if(m_command.noFull)
		filters.append("\\full\\1");
	if(m_command.proxy)
		filters.append("\\proxy\\1");
	if(m_command.noPlayer)
		filters.append("\\noplayer\\1");
	if(m_command.white)
		filters.append("\\white\\1");
	if(m_command.napp != 0)
		filters.append("\\napp\\" + QString::number(m_command.napp));

	msg = "\x31\xff";
	msg += ip + ":" + QString::number(port);
	msg += QByteArray::fromRawData("\0", 1);

	if(filters.length() > 0)
		msg += filters + QByteArray::fromRawData("\0", 1);

	return msg;
}

void ValveServerList::getIpPort(QByteArray rawData, QString &addr, quint16 &port)
{
	//TODO: fare il check sulla lunghezza dei dati (inutile da fare solo quando ho voglia di essere pignolo)
	addr = QString("%1.%2.%3.%4")
		.arg( (quint8)rawData[0] )
		.arg( (quint8)rawData[1] )
		.arg( (quint8)rawData[2] )
		.arg( (quint8)rawData[3] );

	port = (quint8)rawData[4] << 8 | (quint8)rawData[5];
}

void ValveServerList::loadMSData() //funzione obsoleta gia da quando e' stata pensata
{
	QFile file("MasterServer.ini");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);

	while (!in.atEnd())
	{
		ServerAddress tmpServer;
		QString line = in.readLine();
		QStringList list = line.split(':');

		tmpServer.ipv4addr = QHostAddress(list[0]).toIPv4Address();
		tmpServer.port = list[1].toUShort();

		m_serverList.append(tmpServer);
	}
}
