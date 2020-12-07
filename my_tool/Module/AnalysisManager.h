#ifndef ANALYSISMANAGER_H
#define ANALYSISMANAGER_H
#include <QByteArray>
#include <QString>
#include <QMutex>
#include <QByteArray>
#include <QHash>
#include <QThread>
#include <QSemaphore>

class AnalysisInfo;

typedef void(*mcallback)(void *pr, void *data);

typedef void(*doAnalying)(QByteArray *receivedata, AnalysisInfo*info);

#define  ARP_GET_DEV "ARP_GET_DEV"
#define  ARP_TRANSMIT "ARP_TRANSMIT"//流量转发
#define  CUT_OUT_PIC "CUT_OUT_PIC"
struct deviceInfo
{
	QByteArray IP;
	QByteArray MAC;
	QString strIp;
	QString strMac;
};
struct transmitInfo//转发所需要的信息
{
	QString devMacStr;//监听设备mac

	QString gatewayMacStr;//网关设备mac

	QString mMacStr;//本机mac

	QByteArray devMac;

	QByteArray gatewayMac;//网关设备mac

	QByteArray mMac;//本机mac

	QByteArray devIP;//监听设备ip

	QByteArray mIP;//本机IP
};



struct AnalysisInfo
{
	doAnalying doAnaly;//
	bool isRuned;
	bool isRunOneTime;//是否只运行一次
	void *anaPr;
	void *anaParameter;
	mcallback call;//用于回调
	void *callpr;//回调类指针
	void *callPar;//回调类的参数
	QString name;//分析任务名字

	AnalysisInfo()
	{
		isRuned = false;
		isRunOneTime = false;
		doAnaly = nullptr;
		anaPr = nullptr;
		anaParameter = nullptr;
		call = nullptr;
		callPar = nullptr;
	}
	~AnalysisInfo()
	{
		if (anaParameter)
		{
			delete anaParameter;
		}
		if (callPar)
		{
			delete callPar;
		}
	}
};

//enum calltype
//{
//	ARP_GET_DEV=0,//arp广播获取设备ip，mac信息
//};


class AnalysisManager :public QThread
{
public:
	AnalysisManager();
	~AnalysisManager();

	static  AnalysisManager*instance();

	void init();

	void run();

	static int numadsfsdf;

	static int getnum(){
		return numadsfsdf;
	}
	QList<AnalysisInfo*>mAnaHash;

	QMutex mAnaMutex;

	void addAnaTask(AnalysisInfo *info)
	{
		if (info)
		{
			mAnaMutex.lock();
			QListIterator<AnalysisInfo*>index(mAnaHash);
			bool isExit = false;
			while (index.hasNext())
			{
				AnalysisInfo *oneinfo = index.next();
				if (oneinfo && (oneinfo->name == info->name))
				{
					isExit = true;
					break;
				}
			}
			if (!isExit)
			{
				mAnaHash.push_back(info);
			}
			mAnaMutex.unlock();
		}
	};

	void deleteAnaTask(QString callname)
	{
		mAnaMutex.lock();
		QMutableListIterator<AnalysisInfo*>index(mAnaHash);
		while (index.hasNext())
		{
			AnalysisInfo *oneinfo = index.next();
			if (oneinfo && (oneinfo->name == callname))
			{
				delete oneinfo;
				index.remove();
				break;
			}
		}
		mAnaMutex.unlock();
	};

	static void getDataFromCapManager(QByteArray &receivedata, void *pr);

	static void AnalysisArp(QByteArray *receivedata, AnalysisInfo*info);

	static void monitorAndTransmit(QByteArray *receivedata, AnalysisInfo*info);//监听 然后转发

	static void cutOutPic(QByteArray *receivedata, AnalysisInfo*info);//获取图片

	QList<QByteArray*>								mReceiveData;

	QMutex														mReceiveMutex;

	QSemaphore												mReceiveSem;

	

private:

};

#endif