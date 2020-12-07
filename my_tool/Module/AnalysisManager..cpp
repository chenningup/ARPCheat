#include "AnalysisManager.h"
#include "CapManager.h"
#include <QDebug>
#include <QFile>
static AnalysisManager *analysisManager = nullptr;
int AnalysisManager::numadsfsdf = 0;
AnalysisManager::AnalysisManager()
{

	CapManager::instance()->addcallback(getDataFromCapManager, this);
}

AnalysisManager::~AnalysisManager()
{

}

AnalysisManager* AnalysisManager::instance()
{
	if (analysisManager==nullptr)
	{
		analysisManager = new AnalysisManager();
	}
	return analysisManager;
}

void AnalysisManager::init()
{
	this->start();
}

void AnalysisManager::run()
{
	while (true)
	{
		mReceiveSem.acquire();
		mReceiveMutex.lock();
		QByteArray *data = mReceiveData[0];
		QMutableListIterator<AnalysisInfo*>index(mAnaHash);
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
				if (index.value()->doAnaly)
				{
					index.value()->doAnaly(data, index.value());
					(index.value())->isRuned = true;
				}
			}
		}
		delete data;
		mReceiveData.removeAt(0);
		mReceiveMutex.unlock();
	}
}


void AnalysisManager::getDataFromCapManager(QByteArray &receivedata, void *pr)
{
	AnalysisManager *Analysis = (AnalysisManager*)pr;
	if (Analysis)
	{
		Analysis->mReceiveMutex.lock();
		QByteArray *array = new QByteArray(receivedata);
		Analysis->mReceiveData.push_back(array);
		Analysis->mReceiveMutex.unlock();
		Analysis->mReceiveSem.release();
	}
}

void AnalysisManager::AnalysisArp(QByteArray *receivedata, AnalysisInfo*info)
{
	if (!info)
	{
		return;
	}
	if (receivedata->count()==42)
	{
		if ((receivedata->at(12) == (char)0X08) &&
			(receivedata->at(13) == (char)0X06) &&
			(receivedata->at(21) == (char)0X02)
			)
		{
			QByteArray macArray = receivedata->mid(22, 6);
			QByteArray ipArray = receivedata->mid(28, 4);
			deviceInfo *devinfo=new deviceInfo();
			devinfo->IP = ipArray;
			devinfo->MAC = macArray;
			info->call(info->callpr, devinfo);
		}
	}
}

void AnalysisManager::monitorAndTransmit(QByteArray *receivedata, AnalysisInfo*info)
{
	if (info&&receivedata)
	{
		transmitInfo *traninfo = (transmitInfo*)info->anaParameter;
		if (traninfo)
		{
			QByteArray desmac = receivedata->mid(0,6);
			QByteArray sourmac = receivedata->mid(6,6);
			int type = receivedata->at(12);
			bool ok;
			QByteArray dstportarray = receivedata->mid(36, 2);
			int dstport = dstportarray.toHex().toInt(&ok,16);
			bool issend = false;
		//	qDebug() << "des mac:     " + desmac.toHex()+"    sour mac:" +sourmac.toHex();
		//	&&type == 8 && dstport == 53
			QByteArray sendbyte;
			if (desmac == traninfo->mMac&&sourmac == traninfo->devMac&&type == 8)//是监听的发给本机的
			{
				//qDebug() << "seceive dev:" << receivedata->toHex();
			    sendbyte = *receivedata;
				sendbyte.replace(0, 6, traninfo->gatewayMac);//替换成网关
				sendbyte.replace(6, 6, traninfo->mMac);//替换成本机
			//	receivedata->replace(6, 6, traninfo->mMac);//替换成网关
			//	receivedata->replace(26, 4, traninfo->mIP);//替换成网关
				issend = true;
			}
			//if (desmac == traninfo->mMac&&sourmac == traninfo->gatewayMac&&receivedata->count()>34 && (receivedata->mid(30, 4) == traninfo->devIP) && type == 8 && dstport == 53)//是网关发给我的
			if (desmac == traninfo->mMac&&sourmac == traninfo->gatewayMac && type == 8 && receivedata->count()>34 && (receivedata->mid(30, 4) == traninfo->devIP))//是网关发给我的
			{
				//qDebug() << "seceive gateway:" << receivedata->toHex();
				sendbyte = *receivedata;
				sendbyte.replace(0, 6, traninfo->devMac);//替换成网关
				sendbyte.replace(6, 6, traninfo->mMac);//替换成本机
				issend = true;
			}
			if (issend)
			{
				CapManager::instance()->sendMessage(sendbyte);
				//qDebug() <<"send"<< sendbyte.toHex();
			}
		}
	}
}

void AnalysisManager::cutOutPic(QByteArray *receivedata, AnalysisInfo*info)
{
	if (info&&receivedata)
	{
		QByteArray desmac = receivedata->mid(0, 6);
		QByteArray sourmac = receivedata->mid(6, 6);
		int type = receivedata->at(12);
		bool ok;
		QByteArray dstportarray = receivedata->mid(34, 2);
		int dstport = dstportarray.toHex().toInt(&ok, 16);
		if( dstport == 80) 
		{
			int contenttype = receivedata->indexOf("Content-Type");
			if (contenttype>0)
			{
				int jieshu = receivedata->indexOf("\r\n", contenttype);
				if (jieshu)
				{
					QByteArray type = receivedata->mid(contenttype, jieshu - contenttype);
					int typeindex = type.indexOf("image");
					if (typeindex>0)
					{
						qDebug() << QString(type);

						QString houzhui = type.mid(typeindex+6);
						int httpend = receivedata->indexOf("\r\n\r\n")+4;
						QByteArray picbyte = receivedata->mid(httpend);
						QString num = QString::number(AnalysisManager::getnum());
						numadsfsdf++;
						QFile file(num + "." + houzhui);
						file.open(QIODevice::WriteOnly);
						file.write(picbyte);        // write to stderr
						file.close();
						QFile file1(num  + houzhui);
						file1.open(QIODevice::WriteOnly);
						file1.write(*receivedata);        // write to stderr
						file1.close();



					}
				}
			}
			//qDebug() << receivedata->toHex();	
		}
		//transmitInfo *traninfo = (transmitInfo*)info->anaParameter;
	//	if (traninfo)
		//{
			//if (desmac == traninfo->mMac&&sourmac == traninfo->devMac&&type == 8 && dstport == 80)//是监听的发给本机的
		//	{
				

		//	}
	//	}
	}
}

