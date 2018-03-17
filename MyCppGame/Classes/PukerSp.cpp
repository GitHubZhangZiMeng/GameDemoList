#include "PukerSp.h" 

PukerSp* PukerSp::create(const std::string &filename)
{
	PukerSp * ret = new (std::nothrow) PukerSp();
	if (ret&&ret->init(filename))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool PukerSp::init(const std::string &filename)
{
	puker = Sprite::create(filename);
	//funEventTouch(puker);
	float spriteWidth = Director::getInstance()->getVisibleSize().width / (17 + 2);
	puker->setContentSize(Size(spriteWidth * 2, 80));
	//puker->setTextureRect(Rect(spriteWidth*2, 45, spriteWidth * 2, 90));
	this->addChild(puker);
	return true;
}

void PukerSp::funEventTouch(Sprite *sp)
{
	//this->_eventDispatcher->removeAllEventListeners();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(PukerSp::onTouchBeganss, this);
	listener->onTouchMoved = CC_CALLBACK_2(PukerSp::onTouchMovedss, this);
	listener->onTouchEnded = CC_CALLBACK_2(PukerSp::onTouchEndedss, this);
	listener->onTouchCancelled = CC_CALLBACK_2(PukerSp::onTouchCancelledss, this);
	listener->setSwallowTouches(false);//是否向下传递
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sp);
}

CCRect PukerSp::getRect()

{

	//log("getPositionX()====%f,getPositionY()===%f,getContentSize().height===%f,getContentSize().height===%f", getPositionX() - getContentSize().width / 2, getPositionY() - getContentSize().height / 2, getContentSize().width, getContentSize().height);

	return CCRectMake(getPositionX() - getContentSize().width /2,getPositionY() - getContentSize().height /2,getContentSize().width, getContentSize().height);
}


bool PukerSp::onTouchBeganss(Touch* touch, Event* ev)
{
	log("touch->getLocation.x====%f touch->getLocation.y====%f", touch->getLocation().x, touch->getLocation().y);
	log("getPositionX()====%f,getPositionY()===%f,getContentSize().width===%f,getContentSize().height===%f", getPositionX(), getPositionY(), getContentSize().width, getContentSize().height);
	log("getPositionX()- getContentSize().width/2 ====%f,getPositionX()+ getContentSize().width/2===%f, getPositionY()- getContentSize().height/2===%f,getPositionY()+ getContentSize().height/2===%f", 
		getPositionX() - getContentSize().width / 2, getPositionX() + getContentSize().width / 2, getPositionY() - getContentSize().height / 2, getPositionY() + getContentSize().height / 2);
	CCPoint touchLocation = touch->getLocation(); // 返回GL坐标
	
	if ( touchLocation.x > getPositionX() - getContentSize().width/2 && 
		touchLocation.x< getPositionX() + getContentSize().width/2 && 
		touchLocation.y > getPositionY() - getContentSize().height/2 && 
		touchLocation.y< getPositionY() + getContentSize().height/2)
	{
		auto sp =(Sprite*) ev->getCurrentTarget();
		sp->setPosition(Point(0,10));
		log("touch isTouched true");
		return true;
	}

	else
	{
		log("touch isTouched false");
		return false;
	}



	//float x = this->getPositionX;
	//float y = this->getPositionY + 20;
	//this->setPosition(x,y);
	
}
void PukerSp::onTouchMovedss(Touch* touch, Event* ev)
{

}
void PukerSp::onTouchEndedss(Touch* touch, Event* ev)
{

}
void PukerSp::onTouchCancelledss(Touch* touch, Event* ev)
{

}