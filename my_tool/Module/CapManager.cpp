#include "CapManager.h"
#include <Packet32.h>
#include <ntddndis.h>
#include <conio.h>
#include <QDebug>
#include <Iphlpapi.h>
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
static CapManager *mCapManager=nullptr;
static QList<QByteArray>SendArray;
static QList<QByteArray*>ReceiveArray;
//问题待查   			用了memcpy(AdapterList[i],temp1,temp-temp1); 后 报错
#define Max_Num_Adapter 10
char		AdapterList[Max_Num_Adapter][1024];

#define IPTOSBUFFERS    12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}
CapManager::CapManager()
{
	mSendSem = new QSemaphore();
	mReceiveSem = new QSemaphore();
}

CapManager::~CapManager()
{
}

CapManager * CapManager::instance()
{
	if (!mCapManager)
	{
		mCapManager = new CapManager();
	}
	else
	{
		return mCapManager;
	}
}

void CapManager::init()
{
	msendCap = nullptr;
	mCap = nullptr;
//	getWangkaMacinfo();
	getWangkaInfo();
	mSendPackTask = new sendPackTask();
	SendThread = new MZeroThread(mSendPackTask);
	SendThread->SetSemaphore(mSendSem);
	//mInstallPackTask = new installPackTask();
	//mInstallPackThread = new MOneThread(mInstallPackTask);
	//mInstallPackThread->SetSemaphore(mSendSem);
	//mInstallPackThread->allrun = false;

	ReceiveTask = new receiveTask();
	mReceiveThread = new MOneThread(ReceiveTask);
	mHandleTask = new handleRecTask();
	mHandleRecThread = new MZeroThread(mHandleTask);
	mHandleRecThread->SetSemaphore(mReceiveSem);
}

bool CapManager::open(QString wangka)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	mCurrentWangka = mWangkaInfo.value(wangka);
	if (mWangkaInfo.contains(wangka) && mPcapHash.value(ToWinCapWangKaName(wangka)))
	{
		if ((mCap = pcap_open(mPcapHash.value(ToWinCapWangKaName(wangka))->name,           // 设备名
			65536,                // 要捕获的部分 (只捕获前100个字节)
			PCAP_OPENFLAG_PROMISCUOUS,  // 混杂模式
			1000,               // 读超时时间
			NULL,               // 远程机器验证
			errbuf              // 错误缓冲
			)) == NULL)
		{
			return false;
		}

		if (mReceiveThread)
		{
			mReceiveThread->start();
		}
		if (mHandleRecThread)
		{
			mHandleRecThread->start();
		}
	}
	if (mWangkaInfo.contains(wangka) && mPcapHash.value(ToWinCapWangKaName(wangka)))
	{
		if ((msendCap = pcap_open(mPcapHash.value(ToWinCapWangKaName(wangka))->name,           // 设备名
			65536,                // 要捕获的部分 (只捕获前100个字节)
			PCAP_OPENFLAG_PROMISCUOUS,  // 混杂模式
			1000,               // 读超时时间
			NULL,               // 远程机器验证
			errbuf              // 错误缓冲
			)) == NULL)
		{
			return false;
		}

		if (SendThread)
		{
			SendThread->start();
		}
	}

	return true;
}

void CapManager::close()
{

}

void CapManager::send()
{
	if (!msendCap)
	{
		return;
	}
	sendMutex.lock();
	if (SendArray.count()>0)
	{
		mSendCapM.lock();
		if (pcap_sendpacket(msendCap, (u_char*)SendArray.at(0).data(), SendArray.at(0).count()) == 0)
		{
			SendArray.removeAt(0);	
		}
		mSendCapM.unlock();
	}
	sendMutex.unlock();

}
void CapManager::sendMessage(QByteArray &byte)
{
	sendMutex.lock();
	SendArray.push_back(byte);
	sendMutex.unlock();
	mSendSem->release();
}

void CapManager::receive()
{
	if (!mCap)
	{
		return;
	}
	const u_char *pkt_data=nullptr;
	struct pcap_pkthdr *header=nullptr;
	int res = 0;
	//while (true)
{
	mCapM.lock();
	res = pcap_next_ex(mCap, &header, &pkt_data);
	mCapM.unlock();
	if (res == 0)
		return;
			/* 超时时间到 */
		/* 将时间戳转换成可识别的格式 */
	//	local_tv_sec = header->ts.tv_sec;
		//ltime = localtime(&local_tv_sec);
	//	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);
		//	u_char  ch=*pkt_data;
	//	QByteArray onebyte((char*)pkt_data);

	//QString sstr;
	QByteArray *dataarray=new QByteArray();
		for (int i = 1; (i < header->caplen + 1); i++)
		{
			//int len = strlen((char *)pkt_data[i - 1]);
		    char data[1024 * 10];
			memset(data, 0, 1024 * 10);

		//	memcpy(data, pkt_data[i - 1],3);
			int a= pkt_data[i - 1];
			dataarray->push_back(a);
			//sprintf(data, "%.2x ", pkt_data[i - 1]);
			//sstr += QString::fromLocal8Bit(data);
			//printf("%.2x ", pkt_data[i - 1]);
			//qDebug() << sstr;
		//	if ((i % LINE_LEN) == 0) printf("\n");
		}
		receiveMutex.lock();
		//ReceiveArray.push_back(sstr.toUtf8());
		ReceiveArray.push_back(dataarray);
		receiveMutex.unlock();
		mReceiveSem->release();
		/*	sscanf((char*)pkt_data, "%x", aaa);
		QString sstr = QString::fromLocal8Bit(aaa);*/
	//	emit receivedata(sstr);
//		qDebug() << timestr << QString::number(header->ts.tv_usec) << QString::number(header->len);
		//printf("%s,%.6d len:%d\n", timestr, header->ts.tv_usec, header->len);
	//	Sleep(10);
	}

}
void CapManager::handlereceive()
{
	mCallBackMutex.lock();

	receiveMutex.lock();
	if (ReceiveArray.count()>0)
	{
		QListIterator<callbackInfo>index(handlelist);
		while (index.hasNext())
		{
			callbackInfo info = index.next();
			QByteArray *array = ReceiveArray[0];
			info.Handle(*array, info.pr);
		}
		delete ReceiveArray[0];
		ReceiveArray.removeAt(0);
	}
	receiveMutex.unlock();
	mCallBackMutex.unlock();
}

void CapManager::getWangkaInfo()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_if_t *alldevs;
	pcap_if_t *d;
	pcap_addr_t *a;
	quint32 net_ip;
	quint32 net_mask;
	//struct in_addr net_ip_address;
	//QString net_ip_string;
	pcap_findalldevs(&alldevs, errbuf);
	for (d = alldevs; d != NULL; d = d->next)
	{
		for (a = d->addresses; a; a = a->next)
		{
			if (a->addr->sa_family == AF_INET)
			{
				QString wangkaname = QString::fromLocal8Bit(d->name);
				wangkaname=wangkaname.mid(wangkaname.indexOf('{'));
				mPcapHash.insert(wangkaname, d);
			//	mWangkaNameHash.insert(QString::fromLocal8Bit(d->description), QString::fromLocal8Bit(d->name));
				//mWangkaNameHash.insert("1","123");
			}
		}
	}
	/*****************获取网卡名、mac地址、ip地址、子网掩码、默认网关**********************/
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO adapter_info = new IP_ADAPTER_INFO();
	PIP_ADAPTER_INFO adapter = nullptr;
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	memset(adapter_info, 0, stSize);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	auto ret = GetAdaptersInfo(adapter_info, &stSize);
	if (ERROR_BUFFER_OVERFLOW == ret) {
		delete adapter_info;
		adapter_info = (PIP_ADAPTER_INFO)new BYTE[stSize];
		ret = GetAdaptersInfo(adapter_info, &stSize);
	}
	if (ERROR_SUCCESS == ret) 
	{ // 可能有多网卡,因此通过循环去判断
		adapter = adapter_info; // 保存链表头，等下需要释放空间
		while (adapter) 
		{
			if (adapter_info->Type == MIB_IF_TYPE_LOOPBACK) 
				 continue;// 忽略环回地址
			wangKaInfo *info = new wangKaInfo();
			info->adapterName = QString::fromLocal8Bit(adapter_info->AdapterName);
			char mac[128] = { 0 };
			for (DWORD i = 0; i < adapter_info->AddressLength; i++)
				sprintf(mac + strlen(mac)
				, (i < adapter_info->AddressLength - 1 ? "%02X:" : "%02X")
				, adapter_info->Address[i]);
			info->mac = QString::fromLocal8Bit(mac);
			info->ip = QString::fromLocal8Bit(adapter_info->IpAddressList.IpAddress.String);
			info->ipMask = QString::fromLocal8Bit(adapter_info->IpAddressList.IpMask.String);
			info->gateway = QString::fromLocal8Bit(adapter_info->GatewayList.IpAddress.String);
			info->description = QString::fromLocal8Bit(adapter_info->Description);
			mWangkaInfo.insert(info->description, info);
			adapter = adapter->Next;
		}
	}
	if (adapter_info) {
		delete adapter_info;
	}
}

void CapManager::getWangkaMacinfo()
{
	LPADAPTER	lpAdapter = 0;
	char		AdapterName[8192];
	int i = 0;
	int			AdapterNum = 0, Open;
	ULONG		AdapterLength;
	DWORD		dwErrorCode;
	_PACKET_OID_DATA  OidData;
	BOOLEAN		Status;
	AdapterLength = sizeof(AdapterName);
	if (PacketGetAdapterNames((PTSTR)AdapterName, &AdapterLength) == FALSE){
		printf("Unable to retrieve the list of the adapters!\n");
		return;
	}
	char		*temp, *temp1;
	temp = AdapterName;
	temp1 = AdapterName;
	QString tempstr; //= QString::fromLocal8Bit(AdapterName);
	QStringList templist;
	int index = 0;

	while ((*temp != '\0') || (*(temp - 1) != '\0'))
	{
		//if (*temp == '\0')
		//{
		//	strncpy(AdapterList[i], temp1, temp - temp1);       ////用了这段代码 return a.exec();的时候报错，无法理解
		//	temp1 = temp + 1;
		//	i++;
		//}

		//char temchar[1] = *temp;
		//strcpy(temchar, *temp);
		temp++;
		index++;
	}
	tempstr = QString::fromLocal8Bit(AdapterName, index);
	index = 0;
	QString str;
	for (size_t j = 0; j < tempstr.count(); j++)
	{
		if (tempstr.at(j) != '\0')
		{
			str = str + tempstr.at(j);
			if (j ==tempstr.count()-1)
			{
				templist.append(str);
			}
		}
		else
		{
			templist.append(str);
			str = "";
		}
	}
	temp = nullptr;
	temp1 = nullptr;
	AdapterNum = i;
	for (i = 0; i < templist.count(); i++)
	{
		qDebug() << templist.at(i);
		lpAdapter = PacketOpenAdapter(templist.at(i).toLocal8Bit().data());
		if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
		{
			dwErrorCode = GetLastError();
			printf("Unable to open the adapter, Error Code : %lx\n", dwErrorCode);
		}
		else
		{
			//OidData = (PACKET_OID_DATA)malloc(6 + sizeof(PACKET_OID_DATA));
			//OidData = new PACKET_OID_DATA();
			OidData.Oid = OID_802_3_CURRENT_ADDRESS;
			OidData.Length = 6;
			ZeroMemory(OidData.Data, 6);
			Status = PacketRequest(lpAdapter, FALSE, &OidData);
			if (Status)
			{
				printf("The MAC address of the adapter is %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
					(OidData.Data)[0],
					(OidData.Data)[1],
					(OidData.Data)[2],
					(OidData.Data)[3],
					(OidData.Data)[4],
					(OidData.Data)[5]);
				int a = (OidData.Data)[0];
				int b = (OidData.Data)[1];
				int c = (OidData.Data)[2];
				int d = (OidData.Data)[3];
				int e = (OidData.Data)[4];
				int f = (OidData.Data)[5];
				//mWangkaMacHash.insert(templist.at(i), 
				//	QString::number(a,16)+":"+
				//	QString::number(b,16) + ":" +
				//	QString::number(c,16) + ":" +
				//	QString::number(d,16) + ":" +
				//	QString::number(e,16) + ":" +
				//	QString::number(f,16) 		
				//	);
			}
			//Sleep(200);
		}
		PacketCloseAdapter(lpAdapter);
	}
	int a = 1;
}



QStringList* CapManager::GetWangKaNameList()
{
	QHashIterator<QString, wangKaInfo*>index(mWangkaInfo);
	QStringList *list=new QStringList();
	while (index.hasNext())
	{
		index.next();
		list->append(index.key());
	}
	return list;
}


QString CapManager::getCurrentWangkaMac()
{
	if (mCurrentWangka)
	{
		return mCurrentWangka->mac;
	}
}

QString CapManager::getCurrentWangkaIp()
{
	if (mCurrentWangka)
	{
		return mCurrentWangka->ip;
	}
}

QString CapManager::ToWinCapWangKaName(QString &name)
{
	if (mWangkaInfo.contains(name))
	{
		if (mWangkaInfo.value(name))
		{
			return mWangkaInfo.value(name)->adapterName;
		}
	}
	return "";
}

QString CapManager::getCurrentWangkaGateway()
{
	if (mCurrentWangka)
	{
		return mCurrentWangka->gateway;
	}
}

bool CapManager::macStrToByte(QByteArray &marray, QString &str)
{
	marray.clear();
	QStringList maclist = str.split(':');
	for each (QString var in maclist)
	{
		bool ok;
		int hexnum = var.toInt(&ok,16);
		marray.push_back(hexnum);
	}
	return true;
}

bool CapManager::ipStrToByte(QByteArray &marray, QString &str)
{
	marray.clear();
	QStringList maclist = str.split('.');
	for each (QString var in maclist)
	{
		bool ok;
		int hexnum = var.toInt(&ok, 10);
		marray.push_back(hexnum);
	}
	return true;
}



