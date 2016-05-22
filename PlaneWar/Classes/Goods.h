#pragma once
#include "cocos2d.h"

class Goods :public cocos2d::Sprite
{
public:
	enum enGoodsType
	{
		enBullet,
		enBomb
	};
	CREATE_FUNC(Goods);

private:
	bool init();

	void initWithTypeBullet();
	void initWithTypeBomb();
	void goodsMoveAction();

private:
	CC_SYNTHESIZE(enGoodsType, _type, GoodsType);
	CC_SYNTHESIZE(cocos2d::Vec2, _start_point, StartPoint);
	CC_SYNTHESIZE(bool, _out_side, OutSide);
};