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

#include "ValveRcon.h"

//TODO: controllare perche sembra che in LAN bisogna utilizzare il protocollo UDP

ValveRcon::ValveRcon(QObject *parent) :
	QObject(parent)
{
	m_socket = new QTcpSocket(this);

	connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
	connect(m_socket, SIGNAL(connected()), this, SIGNAL(connectedToServer()));
	connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(disconnectedFromServer()));
	connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

	m_splitted = false;
	authenticated = false;
	m_size = 0;
}

ValveRcon::~ValveRcon()
{
}

void ValveRcon::connectToServer(QHostAddress ip, quint16 port)
{
	m_socket->connectToHost(ip, port);
}

void ValveRcon::disconnectFromServer()
{
	m_socket->disconnectFromHost();
}

void ValveRcon::sendCommand(QString command)
{
	QByteArray msg = QByteArray::fromRawData("\x01\x00\x00\x00", 4);

	if(command.left(14) == "rcon_password ")
	{
		command.remove(0,14); //keep only the password
		msg += Utility::getBytesFromInteger32(SERVERDATA_AUTH);
	}
	else if(authenticated)
		msg += Utility::getBytesFromInteger32(SERVERDATA_EXECCOMMAND);
	else
	{
		emit response("You must login before send commands! Use rcon_password <password>");
		return;
	}

	msg += command;
	msg += QByteArray::fromRawData("\0\0", 2);
	msg.prepend(Utility::getBytesFromInteger32(msg.length()));

	m_socket->write(msg);
}

void ValveRcon::readData()
{
	if(!m_splitted)
	{
		m_data = m_socket->read(4);

		if(m_data.length() < 4)
			return;

		m_size = Utility::getInt32(m_data);
		m_data = m_socket->read(m_size);
	}
	else
		m_data = m_socket->read(m_size - m_data.length());

	if(m_data.length() == m_size)
	{
		QString tmp;
		qint32 requestId = Utility::getInt32(m_data);

		if(requestId == -1)
			emit response("ERROR: Authentication failed! Wrong password.");
		else
		{
			switch(Utility::getInt32(m_data, 4)) //command response
			{
				case SERVERDATA_RESPONSE_VALUE:
					tmp = Utility::getString(m_data, 8);
					tmp.chop(2);
					if(tmp != "") emit response(tmp);
					break;
				case SERVERDATA_AUTH_RESPONSE:
					authenticated = true;
					emit response("Authentication successful!");
					break;
			}
		}

		m_splitted = false;
		m_data.clear();
	}
	else
	m_splitted = true;
}

void ValveRcon::error(QAbstractSocket::SocketError error)
{
	//TODO: fare una gestione intelligente dell'errore con un bel switchettone
	qDebug()<<error;
}
