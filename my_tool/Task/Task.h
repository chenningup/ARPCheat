#ifndef TASK_H
#define TASK_H


#include <QMutex>
#include <QSemaphore>


class Task
{
public:
	Task(){};
	virtual  ~Task(){};
	virtual void runWork()=0;
	inline void SetMutex(QMutex *Mutex){ mMutex = Mutex; };
	//inline void SetSemaphore(QSemaphore *Semaphore){ mSemaphore = Semaphore; };
	QMutex *mMutex;
//	QSemaphore *mSemaphore;
private:

};

#endif