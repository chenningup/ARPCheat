#include "PackIngManager.h"
#include "CapManager.h"
#include <QDebug>
static PackingManager *mManager = nullptr;
PackingManager::PackingManager()
{

}

PackingManager::~PackingManager()
{

}

PackingManager* PackingManager::instance()
{
	if (!mManager)
	{
		mManager = new PackingManager();
	}
	return mManager;
}

void PackingManager::init()
{
	this->start();
}

void PackingManager::run()
{
	while (true)
	{
		mPackingMutex.lock();
		QMutableListIterator<packingnfo*>index(mPackingHash);
		while (index.hasNext())
		{
			index.next();
			if (!index.value())
			{
				continue;
			}
			if (index.value()->isRuned&&index.value()->isRunOneTime)
			{
				delete index.value();
				index.remove();
			}
			else
			{
				if (index.value()->call)
				{
					index.value()->call(index.value()->pr, index.value()->parameter);
					(index.value())->isRuned = true;		
				}
			}
		}
		mPackingMutex.unlock();
		this->sleep(8);
	}
}

QString PackingManager::getThreeIp(QString ip)
{
	QStringList iplist = ip.split('.');
	if (iplist.count() == 4)
	{
		return iplist.at(0) + "." + iplist.at(1) + "." + iplist.at(2) + ".";
	}
}


void  PackingManager::createcharip(QString ipstr, char *str)
{

	if (ipstr != "")
	{
		sscanf(ipstr.toLocal8Bit().data(), "%d.%d.%d.%d", &str[0], &str[1], &str[2], &str[3]);
	}

}

void PackingManager::createcharmac(QString macip, char *str)
{
	if (macip != "")
	{
		sscanf(macip.toLocal8Bit().data(), "%x:%x:%x:%x:%x:%x", &str[0], &str[1], &str[2], &str[3], &str[4], &str[5]);
	}

}

void   PackingManager::createherader(int type, char *str)
{
	str[0] = 0x00;
	str[1] = type;
}

QByteArray PackingManager::createarppack(QString ethernetDstMac, QString ethernetSendMac, int type, QString dstMac, QString disIp, QString senderMac, QString senderIP)
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

	char tmp[6] = { 0x00, 0x01, 0x08, 0x00, 0x06, 0x04 };
	oneByteArray.append(ethernetDstMacchar, 6);
	oneByteArray.append(ethernetSendMacchar, 6);
	oneByteArray.append(protoalType,2);
	oneByteArray.append(tmp,6);
	char opcode[2] = {0};
	createherader(type, opcode);

	char senderIpchar[4];
	memset(senderIpchar, 0, 4);
	createcharip(senderIP, senderIpchar);


	char dstIPchar[4];
	memset(dstIPchar, 0, 4);
	createcharip(disIp, dstIPchar);

	char sendMacChar[10] = {0};
	char targetMacChar[10] = {0};
	//memset(sendMacChar, 0, 10);
	//memset(targetMacChar, 0, 10);
	createcharmac(senderMac, sendMacChar);
	createcharmac(dstMac, targetMacChar);



	oneByteArray.append(opcode,2);
	oneByteArray.append(sendMacChar, 6);
	oneByteArray.append(senderIpchar,4);
	oneByteArray.append(targetMacChar, 6);
	oneByteArray.append(dstIPchar,4);
	return oneByteArray;
}


QByteArray PackingManager::creatIcmppack(QString ethernetDstMac, QString ethernetSendMac, QString disIp, QString senderIP)
{
	QByteArray oneByteArray;
	oneByteArray.clear();

	char ethernetSendMacchar[10];
	char ethernetDstMacchar[10];
	memset(ethernetSendMacchar, 0, 10);
	memset(ethernetDstMacchar, 0, 10);
	createcharmac(ethernetSendMac, ethernetSendMacchar);
	createcharmac(ethernetDstMac, ethernetDstMacchar);

	char protoalType[2] = { 0x08, 0x00 };

	QByteArray ipArray;
	ipArray.append(0X45);
	char  Field[1] = { 0X00 };
	ipArray.append(Field,1);

	char  totalnum[2] = { 0X00, 0X3C };
	ipArray.append(totalnum,2);

	char  Identif[2] = { 0X0C, 0XEE };
	ipArray.append(Identif,2);

	char  flags[2] = { 0X00,0X00 };
	ipArray.append(flags,2);

	char live[1] = { 0X40 };
	ipArray.append(live,1);

	char protocol[1] = { 0X01 };
	ipArray.append(protocol,1);

	char check[2] = { 0X00, 0X00 };
	ipArray.append(check,2);

	char senderIpchar[4];
	memset(senderIpchar, 0, 4);
	createcharip(senderIP, senderIpchar);
	ipArray.append(senderIpchar,4);

	char dstIPchar[4];
	memset(dstIPchar, 0, 4);
	createcharip(disIp, dstIPchar);
	ipArray.append(dstIPchar,4);

	quint16 checkresult = crc16ForModbus(ipArray);
	quint8 gao8wei = (checkresult & 0XFF00) >> 8;
	quint8 di8wei = (checkresult & 0X00FF);
	QByteArray checkaresult;
	checkaresult.append(gao8wei);
	checkaresult.append(di8wei);
	ipArray.replace(10, 2, checkaresult);

	QByteArray icmpArray;
	char tou[8] = { 0X08, 0X00, 0X00, 0X00, 0X00, 0X01, 0X00, 0X01 };
	icmpArray.append(tou,8);
	for (size_t i = 0; i < 32; i++)
	{
		icmpArray.append('a' + i);
	}
	qDebug() << icmpArray.toHex();
	quint16 icmpArraycheckresult = crc16ForModbus(icmpArray);
	quint8 gao8wei1 = (icmpArraycheckresult & 0XFF00) >> 8;
	quint8 di8wei1 = (icmpArraycheckresult & 0X00FF);
	QByteArray icmpArraycheckaresult;
	icmpArraycheckaresult.append(gao8wei1);
	icmpArraycheckaresult.append(di8wei1);
	icmpArray.replace(2, 2, icmpArraycheckaresult);

	oneByteArray.append(ethernetDstMacchar, 6);
	oneByteArray.append(ethernetSendMacchar, 6);
	oneByteArray.append(protoalType, 2);
	//oneByteArray.append(sendermacchar, 6);
	oneByteArray.append(ipArray);
	//	oneByteArray.append(dstmacchar, 6);
	oneByteArray.append(icmpArray);
	qDebug() << oneByteArray.toHex();
	return oneByteArray;




}



void PackingManager::packArpBroadcast(void *pr, void *parameter)
{
	arppack *pack = (arppack*)parameter;
	PackingManager *packingManager = (PackingManager*)pr;
	if (parameter&&packingManager)
	{
		for (int i = 1; i <= 255; i++)
		{
			QString targetIp = packingManager->getThreeIp(pack->sendIp) + QString::number(i);
			QByteArray senddata = packingManager->createarppack(pack->disMac, pack->surMac,
				pack->type, pack->targetMac, targetIp, pack->sendMac, pack->sendIp);
			CapManager::instance()->sendMessage(senddata);
			Sleep(20);
		}
	}
}

void PackingManager::packArpClient(void *pr, void *parameter)
{
	arppack *pack = (arppack*)parameter;
	PackingManager *packingManager = (PackingManager*)pr;
	if (parameter&&packingManager)
	{
		QByteArray senddata = packingManager->createarppack(pack->disMac, pack->surMac,
			pack->type, pack->targetMac, pack->targetIp, pack->sendMac, pack->sendIp);
		CapManager::instance()->sendMessage(senddata);	
	}
}

void PackingManager::packIcmp(void *pr, void *parmeter)
{
	icmppack *pack = (icmppack*)parmeter;
	PackingManager *packingManager = (PackingManager*)pr;
	if (parmeter&&packingManager)
	{
		QByteArray senddata = packingManager->creatIcmppack(pack->disMac, pack->surMac,
			 pack->targetIp, pack->sendIp);
		CapManager::instance()->sendMessage(senddata);
	}
}

quint16 PackingManager::crc16ForModbus(const QByteArray &data)
{
	//static const quint16 crc16Table[] =
	//{
	//	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	//	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	//	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	//	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	//	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	//	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	//	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	//	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	//	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	//	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	//	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	//	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	//	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	//	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	//	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	//	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	//	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	//	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	//	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	//	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	//	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	//	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	//	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	//	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	//	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	//	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	//	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	//	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	//	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	//	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	//	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	//	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
	//};
	//quint8 buf;
	//quint16 crc16 = 0xFFFF;

	//for (auto i = 0; i < data.size(); ++i)
	//{
	//	buf = data.at(i) ^ crc16;
	//	crc16 >>= 8;
	//	crc16 ^= crc16Table[buf];
	//}
	//return crc16;
	QList<quint16>quintdatalist;
	quint8 gaowei;
	quint8 diwei;
	for (size_t i = 0; i < data.count(); i++)
	{
		if (i%2)
		{
			diwei = data[i];
			quint16 onequint16 = (gaowei << 8) + diwei;
			quintdatalist.push_back(onequint16);
		}
		else
		{
			gaowei = data[i];
		}
	}
	quint32 cksum=0;
	QListIterator<quint16>index(quintdatalist);
	while (index.hasNext())
	{
		cksum +=index.next();
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);

	return(quint16)(~cksum);





}
