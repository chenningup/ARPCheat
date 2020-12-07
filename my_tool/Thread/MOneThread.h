#ifndef MONETHREAD_H
#define MONETHREAD_H


#include <QThread>
#include "Task/Task.h"

class MOneThread :public QThread
{
public:
	MOneThread();
	~MOneThread();

	MOneThread(Task *task){ mTask = task; };
	bool allrun;
	Task *mTask;
	void run();
	inline void SetSemaphore(QSemaphore *Semaphore){ mSemaphore = Semaphore; };
	//QMutex *mMutex;
	QSemaphore *mSemaphore;

private:


};
#endif
