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

#include "Utility.h"

namespace Utility
{
	qint16 getInt16(QByteArray source, int from, int &position)
	{
		qint16 tmp16 = *(qint16*)source.mid(from, 2).data();
		position = from + 2;

		return tmp16;
	}

	qint16 getInt16(QByteArray source, int from)
	{
		int unused;
		return getInt16(source, from, unused);
	}

	qint16 getInt16(QByteArray source)
	{
		return getInt16(source, 0);
	}

	quint16 getUInt16(QByteArray source, int from, int &position)
	{
		return (quint16)getInt16(source, from, position);
	}

	quint16 getUInt16(QByteArray source, int from)
	{
		return (quint16)getInt16(source, from);
	}

	quint16 getUInt16(QByteArray source)
	{
		return (quint16)getInt16(source);
	}

	qint32 getInt32(QByteArray source, int from, int &position)
	{
		qint32 tmp32 = *(qint32*)source.mid(from, 4).data();
		position = from + 4;

		return tmp32;
	}

	qint32 getInt32(QByteArray source, int from)
	{
		int unused;
		return getInt32(source, from, unused);
	}

	qint32 getInt32(QByteArray source)
	{
		return getInt32(source, 0);
	}

	quint32 getUInt32(QByteArray source, int from, int &position)
	{
		return (quint32)getInt32(source, from, position);
	}

	quint32 getUInt32(QByteArray source, int from)
	{
		return (quint32)getInt32(source, from);
	}

	quint32 getUInt32(QByteArray source)
	{
		return (quint32)getInt32(source);
	}

	qint64 getInt64(QByteArray source, int from, int &position)
	{
		qint64 tmp64 = *(qint64*)source.mid(from, 8).data();
		position = from + 8;

		return tmp64;
	}

	qint64 getInt64(QByteArray source, int from)
	{
		int unused;
		return getInt32(source, from, unused);
	}

	qint64 getInt64(QByteArray source)
	{
		return getInt32(source, 0);
	}

	quint64 getUInt64(QByteArray source, int from, int &position)
	{
		return (quint64)getInt64(source, from, position);
	}

	quint64 getUInt64(QByteArray source, int from)
	{
		return (quint64)getInt64(source, from);
	}

	quint64 getUInt64(QByteArray source)
	{
		return (quint64)getInt64(source);
	}

	qreal getReal32(QByteArray source, int from, int &position)
	{
		qreal tmp = *(float*)source.mid(from, 4).data();
		position = from + 4;

		return tmp;
	}

	qreal getReal32(QByteArray source, int from)
	{
		int unused;
		return getReal32(source, from, unused);
	}

	qreal getReal32(QByteArray source)
	{
		return getReal32(source, 0);
	}

	QByteArray getBytesFromInteger32(qint32 integer)
	{
		QByteArray data;
		data.resize(4);

		data[0] = integer;
		data[1] = integer << 8;
		data[2] = integer << 16;
		data[3] = integer << 24;

		return data;
	}

	QString getString(QByteArray source, int from, int &position)
	{
		QByteArray str;

		for(; source[from] != '\0'; from++)
		str += source[from];

		position = ++from;
		return QString::fromUtf8(str.data(), str.length());
	}

	QString getString(QByteArray source, int from)
	{
		int unused;
		return getString(source, from, unused);
	}
}
