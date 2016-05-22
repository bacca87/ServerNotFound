#ifndef IPTOCOUNTRY_H
#define IPTOCOUNTRY_H

#include <QtCore>
#include <QtNetwork>

#include "Global.h"

class IpToCountry
{
	struct IPv4Country
	{
		quint32 ipFrom;
		quint32 ipTo;
		char *ctry,
			 *cntry,
			 *country;
		~IPv4Country()
		{
			delete []ctry;
			delete []cntry;
			delete []country;
		}
	};

	QList<IPv4Country*> ipv4Info;
	void LoadData(QIODevice &device);
	bool _isLoaded;

public:
	IpToCountry(QString dbPath);
	~IpToCountry(){ qDeleteAll(ipv4Info); }

	CountryInfo getIPv4Info(quint32 address);
};

#endif // IPTOCOUNTRY_H
