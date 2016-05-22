#include "Goods.h"
using namespace cocos2d;

bool Goods::init()
{
	bool bRet = false;
	do
	{
		float ran = RandomHelper::random_int(0, 1);
		if (ran == 0)
		{
			initWithTypeBullet();
			bRet = true;
		}
		else
		{
			initWithTypeBomb();
			bRet = true;
		}

		int low = getContentSize().width / 2;
		int high = Director::getInstance()->getVisibleSize().width - low * 2;
		int x = RandomHelper::random_int(low, high);
		setStartPoint(Vec2(x, Director::getInstance()->getVisibleSize().height + getContentSize().height / 2));

		goodsMoveAction();

	} while (0);
	return bRet;
}

void Goods::initWithTypeBullet()
{
	initWithSpriteFrameName("ufo1.png");
	setGoodsType(enBullet);
}

void Goods::initWithTypeBomb()
{
	initWithSpriteFrameName("ufo2.png");
	setGoodsType(enBomb);
}

void Goods::goodsMoveAction()
{
	_out_side = false;
	MoveTo* move = MoveTo::create(4.0f, Vec2(getStartPoint().x, -getContentSize().height / 2));
	CallFunc* remove = CallFunc::create([this]() {
		this->_out_side = true;
		removeFromParent();
	});
	runAction(Sequence::create(move, remove, nullptr));
}
