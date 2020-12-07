#include "handleRecTask.h"
#include "Module/CapManager.h"



void handleRecTask::runWork()
{
	CapManager::instance()->handlereceive();
}

