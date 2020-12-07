#ifndef PACKINGMANAGER_H
#define PACKINGMANAGER_H
#include <QByteArray>
#include <QString>
#include <QMutex>
#include <QByteArray>
#include <QHash>
#include <QThread>
#include "PackIngManager.h"
typedef void(*dopacking)(void *pr, void *parameter);

#define  ARP_Broadcast "ARP_Broadcast_"
#define ARP_Client "ARP_Client_"
#define STOP_NET "Stop_Net_" //断网
#define Deception "Deception"
#define DEV "Dev_"
#define GATEWAY "Gateway"

#define ICMP "ICMP"



struct arppack
{
	QString disMac;
	QString surMac;
	int type;
	QString sendMac;
	QString sendIp;
	QString targetMac;
	QString targetIp;
};
struct icmppack
{
	QString disMac;
	QString surMac;
	QString sendIp;
	QString targetIp;
};

//ARP组包命名规则：
//        欺骗++Client/Gateway
//         /
//       /
//IP+
//      \
//        \
//        断网
struct Arp_Client_Par
{
	QHash<QString,arppack*>packlist;
};

struct packingnfo
{
	dopacking call;
	bool isRuned;//是否运行过
	bool isRunOneTime;//是否只运行一次
	void *pr;
	void *parameter;
	QString name;
	packingnfo()
	{
		isRuned = false;
		isRunOneTime = false;
		pr = nullptr;
		parameter = nullptr;
	}
	~packingnfo()
	{
		if (parameter)
		{
			delete parameter;
		}
	}
};


class PackingManager :public QThread
{
public:
	PackingManager();

	~PackingManager();

	static  PackingManager*instance();

	void init();

	void run();

	QList<packingnfo*>mPackingHash;

	QMutex mPackingMutex;

	void addPackTask(packingnfo *info)
	{
		if (info)
		{
			mPackingMutex.lock();
			QListIterator<packingnfo*>index(mPackingHash);
			bool isExit = false;
			while (index.hasNext())
			{
				packingnfo *oneinfo = index.next();
				if (oneinfo && (oneinfo->name == info->name))
				{
					isExit = false;
					break;
				}
			}
			if (!isExit)
			{
				mPackingHash.push_back(info);
			}
			mPackingMutex.unlock();
		}
	};

	void deletePackTask(QString callname)
	{
		mPackingMutex.lock();
		QMutableListIterator<packingnfo*>index(mPackingHash);
		while (index.hasNext())
		{
			packingnfo *oneinfo = index.next();
			if (oneinfo && (oneinfo->name == callname))
			{
				delete oneinfo;
				index.remove();
				break;
			}
		}
		mPackingMutex.unlock();
	};

	 QString getThreeIp(QString ip);//获取IP前三位

	 QByteArray createarppack(QString ethernetDstMac, QString ethernetSendMac, int type, QString dstMac, QString disIp, QString senderMac, QString senderIP);

	 QByteArray creatIcmppack(QString ethernetDstMac, QString ethernetSendMac, QString disIp, QString senderIP);

	 void createcharip(QString ipstr, char *str);

	 void createcharmac(QString macip, char *str);

	 void createherader(int type, char *str);

	static void packArpBroadcast(void *pr, void *parameter);

	static void packArpClient(void *pr, void *parameter);

	static void packIcmp(void *pr,void *parmeter);

	quint16 crc16ForModbus(const QByteArray &data);
	//void AnalysisArp(QByteArray &receivedata);
	//static void getDataFromCapManager(QByteArray &receivedata, void *pr);
private:

};











#endif