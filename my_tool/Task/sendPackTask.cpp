#include "sendPackTask.h"
#include "Module/CapManager.h"

sendPackTask::sendPackTask()
{
}

sendPackTask::~sendPackTask()
{
}

void sendPackTask::runWork()
{
	CapManager::instance()->send();
}
