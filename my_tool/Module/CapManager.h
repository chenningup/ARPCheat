#ifndef CAPMANAGER_H
#define CAPMANAGER_H

#include  "pcap.h"
#include <QHash>
#include <QMutex>
#include <QSemaphore>
#include "Thread/MOneThread.h"
#include "Thread/MZeroThread.h"

#include "Task/sendPackTask.h"
#include "Task/installPackTask.h"
#include "Task/ReceiveTask.h"
#include "Task/handleRecTask.h"
enum packTask
{
	normal=0,//空闲状态   //没有组包发送
	arpALL,//全网段发送
	arpOne//单个IP
};


typedef void(*handeReceiveData)(QByteArray &receivedata,void *pr);

struct wangKaInfo
{
	QString ip;
	QString mac;
	QString ipMask;
	QString gateway;
	QString adapterName;
	QString description;
};


struct callbackInfo
{
	handeReceiveData Handle;
	void *pr;
};
class CapManager
{
public:
	CapManager();
	~CapManager();

	static CapManager *instance();

	void init();

	void getWangkaInfo();

    void getWangkaMacinfo();

	bool open(QString wangka);

	void close();

	void send();

	void sendMessage(QByteArray &byte);

	void receive();

	void handlereceive();

	void addcallback(handeReceiveData handle, void *pr)
	{
		mCallBackMutex.lock();
		callbackInfo info;
		info.Handle = handle;
		info.pr = pr;
		handlelist.push_back(info);
		mCallBackMutex.unlock();
	};

	static bool														macStrToByte(QByteArray &marray,QString &str);

	static bool														ipStrToByte(QByteArray &marray, QString &str);

	QString															ToWinCapWangKaName(QString &name);

	QString															getCurrentWangkaGateway();

	QString															getCurrentWangkaMac();

	QString															getCurrentWangkaIp();

	QStringList *													GetWangKaNameList();

	QList<callbackInfo>										handlelist;

	QMutex															mCallBackMutex;

	QHash<QString, pcap_if_t*>						mPcapHash;

	QHash<QString, wangKaInfo*>					mWangkaInfo;

	QMutex															mCapM;

	QMutex															mSendCapM;

	pcap_t *															mCap;

	pcap_t *															msendCap;

	sendPackTask *												mSendPackTask;

	MZeroThread *												SendThread;

	QSemaphore *												mSendSem;

	QMutex															sendMutex;

	QSemaphore *												mReceiveSem;

	QMutex															receiveMutex;

	receiveTask *													ReceiveTask;

	MOneThread *												mReceiveThread;

	handleRecTask *												mHandleTask;

	MZeroThread *												mHandleRecThread;

	wangKaInfo*													mCurrentWangka;

private:

};

#endif