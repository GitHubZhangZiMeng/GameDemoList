#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DemoSceneOne.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Scene,public TableViewDataSource,public TableViewDelegate
{

	Size visibleSize;
	Vec2 origin;
	//Vector<char*> dataVector;
	Array *dataArr;


public:
	

	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	virtual Size cellSizeForTable(TableView *table) {
		return Size(300, 50);
	};
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx)
	{

		TableViewCell* cell = table->dequeueCell();
		LabelTTF * label;
		if (cell == NULL)
		{
			cell = TableViewCell::create();
			label = LabelTTF::create();
			label->setTag(2);
			label->setFontSize(20);
			label->setAnchorPoint(Point(0.5, 0));
			label->setPosition(visibleSize.width/2, 0);
			label->setVerticalAlignment(TextVAlignment::CENTER);
			label->setHorizontalAlignment(TextHAlignment::CENTER);
			cell->addChild(label);
		}
		else
		{
			label = (LabelTTF*)cell->getChildByTag(2);
		}

		//label->setString(StringUtils::format("Label %ld", idx));

		String *labText = (String *)dataArr->objectAtIndex(dataArr->count()-idx-1);

		label->setString(labText->getCString());

		log("idx====%d labString====%s",idx, label->getString().c_str());

		return cell;
	}
	virtual ssize_t numberOfCellsInTableView(TableView *table)
	{
		return dataArr->count();
	}
public:

	virtual void tableCellTouched(TableView* table, TableViewCell* cell)
	{
		LabelTTF * label = (LabelTTF*)cell->getChildByTag(2);
		log("Lable-->%s", label->getString().c_str());
		switch (dataArr->count()-cell->getIdx())
		{
		case 1:
		{
			auto demoSceneOne = TransitionJumpZoom::create(1.2, DemoSceneOne::createScene());
			Director::sharedDirector()->pushScene(demoSceneOne);
		}
			break;
		default:
			break;
		}
		
	}


	virtual void tableCellHighlight(TableView* table, TableViewCell* cell) {};

	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) {};

	virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) {};



};

#endif // __HELLOWORLD_SCENE_H__
