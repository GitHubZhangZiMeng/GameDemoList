#pragma once

#include "cocos2d.h"

USING_NS_CC;

class PukerSp :public cocos2d::Node
{

public:
	Sprite * puker;
	Rect getRect();
	static PukerSp* create(const std::string &filename);
	virtual bool init(const std::string &filename);
	void funEventTouch(Sprite *sp);
	bool onTouchBeganss(Touch* touch, Event* ev);
	void onTouchMovedss(Touch* touch, Event* ev);
	void onTouchEndedss(Touch* touch, Event* ev);
	void onTouchCancelledss(Touch* touch, Event* ev);
};


