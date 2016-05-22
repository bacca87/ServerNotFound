#include <math.h>

#include "IpToCountry.h"

#define DATA_FIELDS_NUMBER		7
#define IP_FROM					0
#define IP_TO					1
//#define IP_REGISTRY				2
//#define IP_ASSIGNED				3
#define IP_CTRY					4
#define IP_CNTRY				5
#define IP_COUNTRY				6


IpToCountry::IpToCountry(QString dbPath)
{
	QFile db(dbPath);

	if (!db.open(QIODevice::ReadOnly | QIODevice::Text) || db.size() <= 0)
	{
		_isLoaded = false;
	}
	else
	{
		LoadData(db);
		_isLoaded = true;
	}

	db.close();
}

void IpToCountry::LoadData(QIODevice &device)
{
	while (!device.atEnd())
	{
		QString line = (device.readLine()).trimmed();
		if(line[0] != '#' && line[0] != '\0')
		{
			line.remove('"');
			QStringList data = line.split(',');

			if(data.count() == DATA_FIELDS_NUMBER)
			{
				IPv4Country *info = new IPv4Country;

				info->ipFrom = data[IP_FROM].toUInt();
				info->ipTo = data[IP_TO].toUInt();
				info->ctry = qstrdup(data[IP_CTRY].toAscii());
				info->cntry = qstrdup(data[IP_CNTRY].toAscii());
				info->country = qstrdup(data[IP_COUNTRY].toAscii());

				ipv4Info.append(info);
			}
		}
	}
}

//TODO: fare un metodo che controlla che non ci siano buchi all'interno del db se no questa funzione va in deadlock e blocca il programma

CountryInfo IpToCountry::getIPv4Info(quint32 address)
{
	CountryInfo info;

	if(!_isLoaded)
		return info;

	quint32 begin = 0,
			end = ipv4Info.count() - 1,
			p;

	while(begin < end)
	{
		p = begin + (end - begin) / 2;

		if(p == begin || p == end)
			break;

		if(address >= ipv4Info.at(p)->ipFrom && address <= ipv4Info.at(p)->ipTo)
		{
			info.ctry = ipv4Info.at(p)->ctry;
			info.cntry = ipv4Info.at(p)->cntry;
			info.country = ipv4Info.at(p)->country;

			return info;
		}
		else if(address > ipv4Info.at(p)->ipFrom)
		{
			begin = p;
		}
		else
		{
			end = p;
		}
	}

	return info;
}
