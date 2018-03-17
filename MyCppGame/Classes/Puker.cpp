#include "Puker.h"



Puker * Puker::create()
{
	Puker * ret = new (std::nothrow) Puker();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool Puker::init()
{
	return true;
}