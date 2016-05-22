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

#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>

namespace Utility
{
	qint16 getInt16(QByteArray source, int from, int &position);
	qint16 getInt16(QByteArray source, int from);
	qint16 getInt16(QByteArray source);

	quint16 getUInt16(QByteArray source, int from, int &position);
	quint16 getUInt16(QByteArray source, int from);
	quint16 getUInt16(QByteArray source);

	qint32 getInt32(QByteArray source, int from, int &position);
	qint32 getInt32(QByteArray source, int from);
	qint32 getInt32(QByteArray source);

	quint32 getUInt32(QByteArray source, int from, int &position);
	quint32 getUInt32(QByteArray source, int from);
	quint32 getUInt32(QByteArray source);

	qint64 getInt64(QByteArray source, int from, int &position);
	qint64 getInt64(QByteArray source, int from);
	qint64 getInt64(QByteArray source);

	quint64 getUInt64(QByteArray source, int from, int &position);
	quint64 getUInt64(QByteArray source, int from);
	quint64 getUInt64(QByteArray source);

	qreal getReal32(QByteArray source, int from, int &position);
	qreal getReal32(QByteArray source, int from);
	qreal getReal32(QByteArray source);

	QByteArray getBytesFromInteger32(qint32 integer);
	QString getString(QByteArray source, int from, int &position);
	QString getString(QByteArray source, int from);
}

#endif // UTILITY_H
