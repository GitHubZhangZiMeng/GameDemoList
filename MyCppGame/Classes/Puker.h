#pragma once

#include "cocos2d.h"
#include "base/CCRef.h"
#include "PukerSp.h"
USING_NS_CC;

class Puker : public Ref
{

public:
	int pikerNum;
	PukerSp *pukerSprite;
	virtual bool init();

	static Puker * create();

	//static void create(int num, Sprite* p);
};

