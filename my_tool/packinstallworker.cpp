
#include "packinstallworker.h"


PackInstallWorker::PackInstallWorker()
{
}

PackInstallWorker::~PackInstallWorker()
{
}

QString  PackInstallWorker::getHostIpAddress()
{
	QString strIpAddress ;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// 获取第一个本主机的IPv4地址  
	int nListSize = ipAddressesList.size();
	for (int i = 0; i < nListSize; ++i)
	{
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
			ipAddressesList.at(i).toIPv4Address()) 
		{
			if (!isLinkLocalAddress(ipAddressesList.at(i)))
			{
			//	strIpAddress = ipAddressesList.at(i).toIPv4Address();
				 strIpAddress = ipAddressesList.at(i).toString();
				break;
			}
	
		
		}
	}
	// 如果没有找到，则以本地IP地址为IP  
	if (strIpAddress.isEmpty())
	{
		strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
	}
		
	return strIpAddress;
}

QString PackInstallWorker::getHostMacAddress()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表  
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址  
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

QByteArray PackInstallWorker::createarppack(QString ethernetDstMac, QString ethernetSendMac,int type, QString dstMac, QString disIp, QString senderMac, QString senderIP)
{
	QByteArray oneByteArray;
	oneByteArray.clear();



	char ethernetSendMacchar[10];
	char ethernetDstMacchar[10];
	memset(ethernetSendMacchar, 0, 10);
	memset(ethernetDstMacchar, 0, 10);
	createcharmac(ethernetSendMac, ethernetSendMacchar);
	createcharmac(ethernetDstMac, ethernetDstMacchar);

	char protoalType[2] = { 0x08, 0x06 };
	char header[6] = {0x00,0x01,0x08,0x00,0x06,0x04};

	char typestr[5]; 

	createherader(type,typestr);

	char senderIpchar[4];
	memset(senderIpchar, 0, 4);
    createcharip(senderIP,senderIpchar);

	char dstIPchar[4];
	memset(dstIPchar, 0, 4);
	createcharip(disIp,dstIPchar);

	char sendermacchar[6]; 
	memset(sendermacchar, 0, 6);
	createcharmac(senderMac, sendermacchar);

	char dstmacchar[6];
	memset(dstmacchar, 0, 6);
	createcharmac(dstMac,dstmacchar);
	
	oneByteArray.append(ethernetDstMacchar, 6);
	oneByteArray.append(ethernetSendMacchar, 6);
	oneByteArray.append(protoalType, 2);
	oneByteArray.append(header,6);
	oneByteArray.append(typestr,2);
	oneByteArray.append(sendermacchar, 6);
	oneByteArray.append(senderIpchar, 4);
	oneByteArray.append(dstmacchar,6);
	oneByteArray.append(dstIPchar,4);
	

	return oneByteArray;
}

void   PackInstallWorker::createherader(int type, char *str)
{

	str[0] = 0x00;
	str[1] = type;

}

bool PackInstallWorker::isLinkLocalAddress(QHostAddress addr)
{
	quint32 nIPv4 = addr.toIPv4Address();
	quint32 nMinRange = QHostAddress("169.254.1.0").toIPv4Address();
	quint32 nMaxRange = QHostAddress("169.254.254.255").toIPv4Address();
	if ((nIPv4 >= nMinRange) && (nIPv4 <= nMaxRange)) {
		return true;
	}
	else {
		return false;
	}
}

void  PackInstallWorker::createcharip(QString ipstr, char *str)
{

	if (ipstr != "")
	{
		sscanf(ipstr.toLocal8Bit().data(), "%d.%d.%d.%d", &str[0], &str[1], &str[2], &str[3]);
	}

}

void PackInstallWorker::createcharmac(QString macip, char *str)
{
	if (macip != "")
	{
		sscanf(macip.toLocal8Bit().data(), "%x:%x:%x:%x:%x:%x", &str[0], &str[1], &str[2], &str[3], &str[4], &str[5]);
	}

}

QString PackInstallWorker::GetThreeIp(QString ip)
{
	
	int one = 0;
	int two=0;
	int three=0;
	int four = 0;
	 QByteArray iiii = ip.toLocal8Bit();
	 char *bbb =iiii.data();
	 sscanf(bbb, "%d.%d.%d.%d", &one,  &two, &three,&four);

	return QString::number(one) +"." + QString::number(two) + "." + QString::number(three) + ".";///这里是什么鬼  one直接就是192.168.2.了
}



