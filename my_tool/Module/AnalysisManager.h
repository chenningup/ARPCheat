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
#define  ARP_TRANSMIT "ARP_TRANSMIT"//����ת��
#define  CUT_OUT_PIC "CUT_OUT_PIC"
struct deviceInfo
{
	QByteArray IP;
	QByteArray MAC;
	QString strIp;
	QString strMac;
};
struct transmitInfo//ת������Ҫ����Ϣ
{
	QString devMacStr;//�����豸mac

	QString gatewayMacStr;//�����豸mac

	QString mMacStr;//����mac

	QByteArray devMac;

	QByteArray gatewayMac;//�����豸mac

	QByteArray mMac;//����mac

	QByteArray devIP;//�����豸ip

	QByteArray mIP;//����IP
};



struct AnalysisInfo
{
	doAnalying doAnaly;//
	bool isRuned;
	bool isRunOneTime;//�Ƿ�ֻ����һ��
	void *anaPr;
	void *anaParameter;
	mcallback call;//���ڻص�
	void *callpr;//�ص���ָ��
	void *callPar;//�ص���Ĳ���
	QString name;//������������

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
//	ARP_GET_DEV=0,//arp�㲥��ȡ�豸ip��mac��Ϣ
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

	static void monitorAndTransmit(QByteArray *receivedata, AnalysisInfo*info);//���� Ȼ��ת��

	static void cutOutPic(QByteArray *receivedata, AnalysisInfo*info);//��ȡͼƬ

	QList<QByteArray*>								mReceiveData;

	QMutex														mReceiveMutex;

	QSemaphore												mReceiveSem;

	

private:

};

#endif