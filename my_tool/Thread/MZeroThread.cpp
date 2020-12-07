#include "MZeroThread.h"




void MZeroThread::run()
{
	if (allrun)
	{
		while (true)
		{
			if (mTask)
			{
				mSemaphore->acquire();
				mTask->runWork();
			}
		}
	}
	else
	{
		if (mTask)
		{
			mSemaphore->acquire();
			mTask->runWork();
		}
	}

}
