#include "ReceiveTask.h"
#include "Module/CapManager.h"




void receiveTask::runWork()
{
	CapManager::instance()->receive();
}
