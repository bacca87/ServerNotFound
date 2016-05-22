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

#ifndef VALVERCON_H
#define VALVERCON_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "Utility.h"

#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_AUTH 3
#define SERVERDATA_RESPONSE_VALUE 0
#define SERVERDATA_AUTH_RESPONSE 2

class ValveRcon : public QObject
{
    Q_OBJECT

    QTcpSocket *m_socket;
    QByteArray m_data;
    qint32 m_size;
    bool m_splitted;
    bool authenticated;

public:
    explicit ValveRcon(QObject *parent = 0);
    ~ValveRcon();

    void sendCommand(QString command);
    void connectToServer(QHostAddress ip, quint16 port);
    void disconnectFromServer();

signals:
    void connectedToServer();
    void disconnectedFromServer();
    void response(QString data);

private slots:
    void readData();
    void error(QAbstractSocket::SocketError error);
};


#endif // VALVERCON_H
