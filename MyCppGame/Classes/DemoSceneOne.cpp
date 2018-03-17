#include "DemoSceneOne.h"


Scene* DemoSceneOne::createScene()
{
	return DemoSceneOne::create();
}

bool DemoSceneOne::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto layerColor = LayerColor::create(Color4B(255,255,255,255));
	layerColor->setContentSize(visibleSize);
	layerColor->setPosition(origin.x, origin.y);
	this->addChild(layerColor);
	auto startGameBtn = cocos2d::ui::Button::create();
	startGameBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	startGameBtn->setTitleText(this->FontToUTF8("开始游戏"));
	startGameBtn->setTitleColor(Color3B::YELLOW);
	startGameBtn->setTitleFontSize(32);
	
	startGameBtn->addTouchEventListener([&](Ref *sensor, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
		{
			cocos2d::ui::Button *btn = (cocos2d::ui::Button *)sensor;
			//btn->setTitleColor(Color3B::RED);
			//btn->setEnabled(false);
			btn->setVisible(false);
			this->adjustPokerOrder();
			this->sendPokerOutEveryPlayer();
			this->orderPukerComparison(playerUpVector);
		}


		default:
			break;
		}
	});
	this->addChild(startGameBtn);
	//MenuItemImage *item = MenuItemImage::create("", "");
	
	
	auto closeItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",CC_CALLBACK_1(DemoSceneOne::menuCloseCallback,this));

	closeItem->setPosition(Point(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(closeItem, NULL);
	pMenu->setPosition(Point(0,0));
	this->addChild(pMenu, 1);


	CCMenuItemFont * start = CCMenuItemFont::create("YES", CC_CALLBACK_1(DemoSceneOne::outOfTheCards, this));
	start->setColor(Color3B::BLACK);
	start->setPosition(Point(150, 150));
	pMenu->addChild(start, 1);
	//出牌按钮
	CCMenuItemFont * pass = CCMenuItemFont::create("NO", CC_CALLBACK_1(DemoSceneOne::passThisRecycle, this));
	pass->setColor(Color3B::BLACK);
	pass->setPosition(Point(220, 150));
	pMenu->addChild(pass, 1);

	
	
	for (int i = 0; i < 54; i++)
	{
		pokerOrderNumArr[i] = i;
	}


	for (int i = 0; i < 54; i++)
	{
		auto pokerFileStr = String::createWithFormat("%d.png",i+1);
		log("%s",pokerFileStr->getCString());
		PukerSp *pokerSprite = PukerSp::create(pokerFileStr->getCString());
		funEventTouch(pokerSprite);
		pokerMap.insert(i,pokerSprite);
	}
	playerUpDic = Dictionary::create();
	playerOwerDic = Dictionary::create();
	playerDownDic = Dictionary::create();
	playUpSelectValueDic = Dictionary::create();
	playUpSelectValueDic->retain();
	this->adjustPokerOrder();
	this->sendPokerOutEveryPlayer();
	this->orderPukerComparison(playerUpVector);
	//this->orderPukerComparison(playerOwerVector);
	//this->orderPukerComparison(playerDownVector);
	return true;
}
#pragma mark ------洗牌------
void DemoSceneOne::adjustPokerOrder()
{

	for (int i = 0; i < 300; i++)
	{
		Vector <String *> num[54];		
		int randomNum = random() % (53 - 0 + 1);
		int randomNum2 = random() % (53 - 0 + 1);
		int swap = pokerOrderNumArr[randomNum];
		pokerOrderNumArr[randomNum] = pokerOrderNumArr[randomNum2];
		pokerOrderNumArr[randomNum2] = swap;


	}
	for (int i = 0; i < 54; i++)
	{
		pokerAdjustOrderNumArr[i] = pokerOrderNumArr[i];
		log("pokerAdjustOrderNumArr[%d]===%d",i, pokerAdjustOrderNumArr[i]);
	}
	
}

#pragma mark ------发牌------
void DemoSceneOne::sendPokerOutEveryPlayer()
{
	for (int i = 0; i < 54; i++)
	{
		if (i < 17)
		{
			int pokerNum = pokerAdjustOrderNumArr[i];
			PukerSp *pokerSprite = pokerMap.at(pokerNum);
			Puker *pukerClass = Puker::create();
			pukerClass->pikerNum = pokerNum;
			pukerClass->pukerSprite = pokerSprite;
			playerUpVector.pushBack(pukerClass);
			log("pokerNum === %d", pokerNum);
		}
		else if(i>=17&&i<34)
		{

			int pokerNum = pokerAdjustOrderNumArr[i];
			PukerSp *pokerSprite = pokerMap.at(pokerNum);
			Puker *pukerClass = Puker::create();
			pukerClass->pikerNum = pokerNum;
			pukerClass->pukerSprite = pokerSprite;
			playerOwerVector.pushBack(pukerClass);
		}
		else if (i>=34&&i<51)
		{
			int pokerNum = pokerAdjustOrderNumArr[i];
			PukerSp *pokerSprite = pokerMap.at(pokerNum);
			Puker *pukerClass = Puker::create();
			pukerClass->pikerNum = pokerNum;
			pukerClass->pukerSprite = pokerSprite;
			playerDownVector.pushBack(pukerClass);

		}
		else
		{
			int pokerNum = pokerAdjustOrderNumArr[i];
			PukerSp *pokerSprite = pokerMap.at(pokerNum);
			Puker *pukerClass = Puker::create();
			pukerClass->pikerNum = pokerNum;
			pukerClass->pukerSprite = pokerSprite;
			playerOwerVector.pushBack(pukerClass);
		}
	}
	log("playerOwerMap.size()===%d", playerOwerMap.size());
	log("playerDownMap.size()===%d", playerDownMap.size());
	log("playerUpMap.size()===%d", playerUpMap.size());

	int randomPokerOwner = random() % 3;

	log("randomPokerOwner == %d", randomPokerOwner);


	
}

#pragma mark ------展示棋牌------
void DemoSceneOne::displayPuker()
{

	this->orderPukerComparison(playerUpVector);

}

#pragma make ---------大小比较排序------

void DemoSceneOne::orderPukerComparison(Vector <Puker *> &vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		log("i===%d", i);
		for (int j = i+1; j < vector.size(); j++)
		{
			log("vector.at(i)->pikerNum ===%d, vector.at(j)->pikerNum====%d ", vector.at(i)->pikerNum, vector.at(j)->pikerNum);
			if (vector.at(i)->pikerNum > vector.at(j)->pikerNum)
			{
				vector.swap(i, j);
			}
		}
	}
	for (int i = 0; i < vector.size(); i++)
	{
		//log("playerUpVector.at(i)->pikerNum ===%d,", playerUpVector.at(i)->pikerNum);
		auto pukerSprite = vector.at(i)->pukerSprite;
		float spriteWidth = Director::getInstance()->getVisibleSize().width / (vector.size() + 2);
		pukerSprite->setPosition(Point((i + 2)*spriteWidth, 47));
		pukerSprite->setContentSize(Size(spriteWidth*2, 80));
		//pukerSprite->setTextureRect(Rect((i + 2)*spriteWidth, 45, spriteWidth * 2,90));
		this->addChild(pukerSprite);
	}
}

#pragma make ---------选择牌面--------------

void DemoSceneOne::funEventTouch(PukerSp *sp)
{
	//this->_eventDispatcher->removeAllEventListeners();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(DemoSceneOne::onTouchBeganss, this);
	listener->onTouchMoved = CC_CALLBACK_2(DemoSceneOne::onTouchMovedss, this);
	listener->onTouchEnded = CC_CALLBACK_2(DemoSceneOne::onTouchEndedss, this);
	listener->onTouchCancelled = CC_CALLBACK_2(DemoSceneOne::onTouchCancelledss, this);
	listener->setSwallowTouches(false);//是否向下传递
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sp);
}
bool DemoSceneOne::onTouchBeganss(Touch* touch, Event* ev)
{
	auto sp = (PukerSp*)ev->getCurrentTarget();


	//log("touch->getLocation.x====%f touch->getLocation.y====%f", touch->getLocation().x, touch->getLocation().y);
	//log("getPositionX()====%f,getPositionY()===%f,getContentSize().width===%f,getContentSize().height===%f", getPositionX(), getPositionY(), getContentSize().width, getContentSize().height);
	//log("getPositionX()- getContentSize().width/2 ====%f,getPositionX()+ getContentSize().width/2===%f, getPositionY()- getContentSize().height/2===%f,getPositionY()+ getContentSize().height/2===%f",
		//getPositionX() - getContentSize().width / 2, getPositionX() + getContentSize().width / 2, getPositionY() - getContentSize().height / 2, getPositionY() + getContentSize().height / 2);
	CCPoint touchLocation = touch->getLocation(); // 返回GL坐标

	if (touchLocation.x >sp->getPositionX() - sp->getContentSize().width / 2 &&
		touchLocation.x< sp->getPositionX() - sp->getContentSize().width / 2 + Director::getInstance()->getVisibleSize().width / (playerUpVector .size()+2) &&
		touchLocation.y > sp->getPositionY() - sp->getContentSize().height / 2 &&
		touchLocation.y< sp->getPositionY() + sp->getContentSize().height / 2)
	{	

		for (int i = 0; i < playerUpVector.size(); i++)
		{
			if (sp&&sp== playerUpVector.at(i)->pukerSprite)
			{
				if (sp->getPositionY()>47)
				{
					sp->setPosition(Point(sp->getPositionX(), sp->getPositionY() - 10));
					if (playUpSelectValueDic->count()>0)
					{
						String *str = String::createWithFormat("%d", playerUpVector.at(i)->pikerNum);
						playUpSelectValueDic->removeObjectForKey(playerUpVector.at(i)->pikerNum);
						log("playerUpVector.at(i)->pikerNum====%d", playerUpVector.at(i)->pikerNum);
					}
				}
				else
				{
					sp->setPosition(Point(sp->getPositionX(), sp->getPositionY() + 10));
					String *str = String::createWithFormat("%d",playerUpVector.at(i)->pikerNum);
					playUpSelectValueDic->setObject(playerUpVector.at(i)->pukerSprite, playerUpVector.at(i)->pikerNum);
					log("playerUpVector.at(i)->pikerNum====%d", playerUpVector.at(i)->pikerNum);

				}
			}
			
		}
		//log("touch isTouched true");
		return true;	
	}

	else
	{
		//log("touch isTouched false");
		return false;
	}
}
void DemoSceneOne::onTouchMovedss(Touch* touch, Event* ev)
{

}
void DemoSceneOne::onTouchEndedss(Touch* touch, Event* ev)
{

}
void DemoSceneOne::onTouchCancelledss(Touch* touch, Event* ev)
{

}

#pragma make ------规则定义------
void DemoSceneOne::outOfTheCards(Ref* pSender) //true出牌
{
	this->gameRules(playUpSelectValueDic);
}
void DemoSceneOne::passThisRecycle(Ref* pSender)
{

}

bool DemoSceneOne::gameRules(Dictionary *SelectValueDic)
{
	if (SelectValueDic->count()==1)
	{
		return true;
	}
	if (SelectValueDic->count() == 2)
	{
		Array *keys = SelectValueDic->allKeys();

		auto value = (cocos2d::Integer *)(keys->getObjectAtIndex(0));
		log("keys->getObjectAtIndex(0)====%d,,,keys->getObjectAtIndex(1)===%d", keys->getObjectAtIndex(0), keys->getObjectAtIndex(1));
		int n = value->getValue();
		//log("keys->getObjectAtIndex(0)====%d,,,keys->getObjectAtIndex(1)===%d", value->getValue(), keys->getObjectAtIndex(1)->getValue());
		if (((cocos2d::Integer *)(keys->getObjectAtIndex(0)))->getValue()/4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(1)))->getValue() / 4)
		{
			log("true");
			return true;
		}
		else
		{
			log("notrue");
			return false;
		}
	}
	if (SelectValueDic->count() == 3)//////111
	{
		Array *keys = SelectValueDic->allKeys();
		if (((cocos2d::Integer *)(keys->getObjectAtIndex(0)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(1)))->getValue() / 4&& ((cocos2d::Integer *)(keys->getObjectAtIndex(1)))->getValue() / 4== ((cocos2d::Integer *)(keys->getObjectAtIndex(2)))->getValue() / 4)
		{
			log("111 true");
			return true;
		}
		else
		{
			log("no true");
			return false;
		}
	}
	if (SelectValueDic->count()==4)///////1111 1112
	{
		
		Array *keys = SelectValueDic->allKeys();
		if (((cocos2d::Integer *)(keys->getObjectAtIndex(0)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(1)))->getValue() / 4 && 
			((cocos2d::Integer *)(keys->getObjectAtIndex(1)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(2)))->getValue() / 4 && 
			((cocos2d::Integer *)(keys->getObjectAtIndex(2)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(3)))->getValue() / 4)
		{
			log("1111 true");
			return true;
		}
		for (int i = 0; i < keys->count()-2; i++)
		{
			for (int j = i+1; j < keys->count(); j++)
			{
				for (int k = i+2; k < keys->count(); k++)
				{
					if (((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(j)))->getValue() / 4 && 
						((cocos2d::Integer *)(keys->getObjectAtIndex(j)))->getValue() / 4 == ((cocos2d::Integer *)(keys->getObjectAtIndex(k)))->getValue() / 4)
					{
						log("1112 true");
						return true;
					}
					else
					{
						log("notrue");
						return false;
					}
				}
			}
		}
	}
	if (SelectValueDic->count() == 5)///////11122 12345
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[5];

		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		if (keyNum[0] == keyNum[1] &&
			keyNum[1] == keyNum[2] &&
			keyNum[3] == keyNum[4])
		{
			log("11122 true");
			return true;
		}

		auto keysCopy = Array::createWithArray(keys);
		int keysInt[5];
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			if (key < 12)
			{
				keysInt[i] = key;
			}
			else
			{
				log("包含2222大小 notrue");
				return false;
			}

		}
		for (int i = 0; i < 5; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (keysInt[i] > keysInt[j])
				{
					int swap = keysInt[i];
					keysInt[i] = keysInt[j];
					keysInt[j] = swap;
				}
			}
		}
		
		if (keysInt[0]+1== keysInt[1]&& keysInt[1]+1 == keysInt[2] && keysInt[2]+1 == keysInt[3]&& keysInt[3]+1== keysInt[4])
		{
			log("12345 true");
			return true;
		}
		else
		{
			log("5 notrue");
			return false;
		}
	}
	if (SelectValueDic->count() == 6)///////111123  123456 112233 111222
	{
		Array *keys = SelectValueDic->allKeys();
		int keyFourNum = 0;
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			int keyRecord = 0;
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (key == ((cocos2d::Integer *)(keys->getObjectAtIndex(j)))->getValue() / 4)
				{
					keyRecord++;
				}
			}
			if (keyRecord == 3)
			{
				keyFourNum = 1;
			}
		}

		if (keyFourNum == 1)
		{
			log("111122 true");
			return true;
		}

		int keyTwoNum1 = 0;
		int keyTwoNum2 = 0;
		int keyTwoNum3 = 0;
		int keyNum[6];
		bool pukerIsUnder12 = true;
		
		
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i+1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}

		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			if (key >= 12)
			{
				log("112233 包含2222 大小");
				pukerIsUnder12 = false;
				break;
			}
			else
			{
				keyNum[i] = key;
			}
		}

		if (pukerIsUnder12)
		{
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5])
			{
				log("112233 true");
				return true;
			}
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] == keyNum[4] &&
				keyNum[4] == keyNum[5])
			{
				log("111222 true");
				return true;
			}
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] == keyNum[4] &&
				keyNum[4] == keyNum[5])
			{
				log("111222 true");
				return true;
			}
		}
	}

	if (SelectValueDic->count() == 7) ///1234567
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[7];

		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			if (key >=12)
			{
				log("1234567 include 2222 wang flase");
				return false;
			}
			else
			{
				keyNum[i] = key;
			}
			
		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		if (keyNum[0] + 1 == keyNum[1] &&
			keyNum[1] + 1 == keyNum[2] && 
			keyNum[2] + 1 == keyNum[3] && 
			keyNum[3] + 1 == keyNum[4] && 
			keyNum[4] + 1 == keyNum[5] &&
			keyNum[5] + 1 == keyNum[6])
		{
			log("1234567 true");
			return true;
		}
		else
		{
			log("1234567 flase");
			return false;
		}
	}
	if (SelectValueDic->count() == 8) ///12345678  12223334 11223344 
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[8];
		bool pukerIsUnder12 = true;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}

		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}

		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
				break;
			}

		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] + 1 == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] + 1 == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] + 1 == keyNum[7])
			{
				log("12345678 true");
				return true;
			}
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7])
			{
				log("11223344 true");
				return true;
			}
		}
		int numRecode = 0;
		int keyNodeNum = 0;
		for (int i = 0; i < SelectValueDic->count()-1; i++)
		{
			numRecode = 0;
			for (int j = i + 1; j < SelectValueDic->count()-1; j++)
			{
				if (keyNum[i] == keyNum[j] && keyNum[i] < 12 && keyNum[j] == keyNum[j+1] )
				{
					numRecode ++;
				}
			}
			if (numRecode == 2)
			{
				keyNodeNum++;
			}
		}
		if (keyNodeNum == 2)
		{
			for (int i = 0; i < SelectValueDic->count() - 4; i++)
			{
				if (keyNum[i] == keyNum[i + 1] && keyNum[i] == keyNum[i + 2] && keyNum[i] == keyNum[i + 3] && keyNum[i] + 1 == keyNum[i + 4])
				{
					log("12223334 true");
					return true;
				}
			}
			log("12223334 false");
			return false;
		}
		else
		{
			log("12223334 false");
			return false;
		}

	}
	if (SelectValueDic->count() == 9) ///123456789 111222333
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[8];

		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("123456789 include 2222 wang flase");
				return false;
			}

		}

		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		if (keyNum[0] + 1 == keyNum[1] &&
			keyNum[1] + 1 == keyNum[2] &&
			keyNum[2] + 1 == keyNum[3] &&
			keyNum[3] + 1 == keyNum[4] &&
			keyNum[4] + 1 == keyNum[5] &&
			keyNum[5] + 1 == keyNum[6] &&
			keyNum[6] + 1 == keyNum[7] &&
			keyNum[7] + 1 == keyNum[8])
		{
			log("123456789 true");
			return true;
		}

		if (keyNum[0] == keyNum[1] &&
			keyNum[1] == keyNum[2] &&
			keyNum[2] + 1 == keyNum[3] &&
			keyNum[3] == keyNum[4] &&
			keyNum[4] == keyNum[5] &&
			keyNum[5] + 1 == keyNum[6] &&
			keyNum[6] == keyNum[7] &&
			keyNum[7] == keyNum[8])
		{
			log("123456789 true");
			return true;
		}
		log("123456789 false");
		return false;


	}
	if (SelectValueDic->count() == 10) ///12345678910 1122334455  1112223344
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[10];
		bool pukerIsUnder12 = true;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
			}

		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] + 1 == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] + 1 == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] + 1 == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] + 1 == keyNum[9])
			{
				log("12345678910 true");
				return true;
			}
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] == keyNum[9])
			{
				log("1122334455 true");
				return true;
			}
		}
		
		int numRecode3 = 0;
		int numRecode2 = 0;
		int keyNodeNum = 0;
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			numRecode3 = 0;
			for (int j = i + 1; j < SelectValueDic->count() - 1; j++)
			{
				if (keyNum[i] == keyNum[j] && keyNum[i] < 12 && keyNum[j] == keyNum[j + 1])
				{
					numRecode3 ++;
				}
			}
			if (numRecode3 == 2)
			{
				keyNodeNum++;
			}
		}

		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			if (keyNum[i]==keyNum[i+1])
			{
				numRecode2++;
			}
		}

		if (keyNodeNum == 2 && numRecode2 == 6)
		{
			for (int i = 0; i < SelectValueDic->count() - 4; i++)
			{
				if (keyNum[i] == keyNum[i + 1] && keyNum[i] == keyNum[i + 2] && keyNum[i] == keyNum[i + 3] && keyNum[i] + 1 == keyNum[i + 4])
				{
					log("12223334 true");
					return true;
				}
			}
			log("12223334 false");
			return false;
		}
		else
		{
			log("12223334 false");
			return false;
		}

	}

	if (SelectValueDic->count() == 11) ///12345678910j
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[11];
		bool pukerIsUnder12 = true;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
			}

		}

		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] + 1 == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] + 1 == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] + 1 == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] + 1 == keyNum[9] &&
				keyNum[9] + 1 == keyNum[10] &&
				keyNum[10] + 1 == keyNum[11])
			{
				log("12345678910j true");
				return true;
			}
		}
		log("12345678910j false");
		return false;
		
	}
	if (SelectValueDic->count() == 12) ///112233445566 111222333444 111222333456
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[12];
		bool pukerIsUnder12 = true;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
			}

		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] == keyNum[9] &&
				keyNum[9] + 1 == keyNum[10] &&
				keyNum[10] == keyNum[11])
			{
				log("112233445566 true");
				return true;
			}
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] == keyNum[2] &&
				keyNum[2] + 1 == keyNum[3] &&
				keyNum[3] == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7] &&
				keyNum[7] == keyNum[8] &&
				keyNum[8] + 1 == keyNum[9]&&
				keyNum[9] == keyNum[10] &&
				keyNum[10]== keyNum[11] )
			{
				log("111222333444 true");
				return true;
			}
		}
		
		int numCode = 0;

		for (int i = 0; i < SelectValueDic->count()-3; i+=3)
		{
			if (keyNum[i] == keyNum[i+1] && keyNum[i] == keyNum[i + 2] && keyNum[i] + 1 == keyNum[i+3])
			{
				numCode++;
			}
		}
		if (numCode=3)
		{
			log("111222333456 true");
			return true;
		}
		log("111222333456 false");
		return false;
	}
	if (SelectValueDic->count() == 13)
	{

	}

	if (SelectValueDic->count() == 14)///11223344556677
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[12];
		bool pukerIsUnder12 = true;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
			}

		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] == keyNum[9] &&
				keyNum[9] + 1 == keyNum[10] &&
				keyNum[10] == keyNum[11] &&
				keyNum[11] + 1 == keyNum[12] &&
				keyNum[12] == keyNum[13])
			{
				log("11223344556677 true");
				return true;
			}
		}
	}
	if (SelectValueDic->count() == 15)///111222333445566
	{
		Array *keys = SelectValueDic->allKeys();
		int keyNum[15];
		bool pukerIsUnder12 = true;
		int pukerCode = 0;
		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = ((cocos2d::Integer *)(keys->getObjectAtIndex(i)))->getValue() / 4;
			keyNum[i] = key;
		}
		for (int i = 0; i < SelectValueDic->count() - 1; i++)
		{
			for (int j = i + 1; j < SelectValueDic->count(); j++)
			{
				if (keyNum[i] > keyNum[j])
				{
					int swap = keyNum[i];
					keyNum[i] = keyNum[j];
					keyNum[j] = swap;
				}

			}
		}
		for (int i = 0; i < SelectValueDic->count() - 3; i++)
		{
			if (keyNum[i] == keyNum[i+1]&& keyNum[i]== keyNum[i + 2]&& keyNum[i]+1 == keyNum[i+3] && keyNum[i] < 12)
			{
				pukerCode++;
			}
		}
		for (int i = 0; i < SelectValueDic->count() - 3; i++)
		{
			if (keyNum[i] == keyNum[i + 1] && keyNum[i] == keyNum[i + 2] && keyNum[i] + 1 == keyNum[i + 3])
			{
				pukerCode++;
			}
		}


		if (pukerCode==3)
		{

		}
		


		for (int i = 0; i < SelectValueDic->count(); i++)
		{
			int key = keyNum[i];
			if (key >= 12)
			{
				log("1234567 include 2222 wang flase");
				pukerIsUnder12 = false;
			}

		}
		if (pukerIsUnder12)
		{
			if (keyNum[0] == keyNum[1] &&
				keyNum[1] + 1 == keyNum[2] &&
				keyNum[2] == keyNum[3] &&
				keyNum[3] + 1 == keyNum[4] &&
				keyNum[4] == keyNum[5] &&
				keyNum[5] + 1 == keyNum[6] &&
				keyNum[6] == keyNum[7] &&
				keyNum[7] + 1 == keyNum[8] &&
				keyNum[8] == keyNum[9] &&
				keyNum[9] + 1 == keyNum[10] &&
				keyNum[10] == keyNum[11] &&
				keyNum[11] + 1 == keyNum[12] &&
				keyNum[12] == keyNum[13])
			{
				log("11223344556677 true");
				return true;
			}
		}
	}
}




#pragma mark ------中文转utf8------
char *DemoSceneOne::FontToUTF8(const char* font)
{
	int len = MultiByteToWideChar(CP_ACP, 0, font, -1, NULL, 0);
	wchar_t *wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, font, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char *str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr)delete[] wstr;
	return str;
}


void DemoSceneOne::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}