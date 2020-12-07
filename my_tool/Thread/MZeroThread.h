#ifndef MZEROTHREAD_H
#define MZEROTHREAD_H


#include <QThread>
#include "Task/Task.h"

class MZeroThread :public QThread
{
public:
	MZeroThread(){ allrun = true; };
	~MZeroThread(){};

	MZeroThread(Task *task){ mTask = task; };

	Task *mTask;
	void run();
	bool allrun;
	inline void SetSemaphore(QSemaphore *Semaphore){ mSemaphore = Semaphore; };
	//QMutex *mMutex;
	QSemaphore *mSemaphore;

private:


};
#endif
