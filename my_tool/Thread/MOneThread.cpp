#include "MOneThread.h"




MOneThread::MOneThread()
{

	allrun = true;

}

MOneThread::~MOneThread()
{
	mTask = nullptr;


}

void MOneThread::run()
{

	if (allrun)
	{
		while (true)
		{
			if (mTask)
			{
				mTask->runWork();
			//	QThread::sleep(0.01);
			//	mSemaphore->release();
			}
		}
	}
	else
	{
		if (mTask)
		{
			mTask->runWork();
		//	mSemaphore->release();
		}
	}

}
