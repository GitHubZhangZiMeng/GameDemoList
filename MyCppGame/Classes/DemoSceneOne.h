#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "Puker.h"
#include "PukerSp.h"

USING_NS_CC;
USING_NS_CC_EXT;

class DemoSceneOne:public cocos2d::Scene
{


public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(DemoSceneOne);

	void adjustPokerOrder();
	void sendPokerOutEveryPlayer();
	void displayPuker();
	void orderPukerComparison(Vector <Puker *> &vector);
	bool gameRules(Dictionary *SelectValueDic);

	char* FontToUTF8(const char* font);

	void funEventTouch(PukerSp *sp);
	bool onTouchBeganss(Touch* touch, Event* ev);
	void onTouchMovedss(Touch* touch, Event* ev);
	void onTouchEndedss(Touch* touch, Event* ev);
	void onTouchCancelledss(Touch* touch, Event* ev);

	void outOfTheCards(Ref* pSender); //符合规则出牌

	void passThisRecycle(Ref* pSender);
	void menuCloseCallback(Ref* pSender);
//	CREATE_FUNC(DemoSceneOne);
private:

	/*
	-----red 桃 1-13 red 方 14 - 26  black 桃 27 - 39 black flower 40 - 52  大王 54 小王 53
	*/
	int pokerOrderNumArr[54];
	int pokerAdjustOrderNumArr[54];

	int recodeOne;

	Map <int, PukerSp *> pokerMap;
	Map <int, Sprite *> playerUpMap;//地主上家
	Map <int, Sprite *> playerOwerMap;//地主
	Map <int, Sprite *> playerDownMap;//地主下家
	Dictionary *pokerDic;
	Dictionary *playerUpDic;
	Dictionary *playerOwerDic;
	Dictionary *playerDownDic;
	Vector <Puker *> playerUpVector;
	Vector <Puker *> playerOwerVector;
	Vector <Puker *> playerDownVector;

	Dictionary *playUpSelectValueDic;

	//Map <int, Sprite *> threeMap;
};

