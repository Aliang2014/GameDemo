#pragma once
#include "cocos2d.h"

class BulletBase :public cocos2d::Sprite
{
public:
	static BulletBase* create(const char* spriteFramePath, const cocos2d::Vec2& start_point, const cocos2d::Vec2& speed, float attack);

	bool isOutSide() const;

private:
	BulletBase();
	virtual ~BulletBase();

	bool init(const char * spriteFramePath, const cocos2d::Vec2 & start_point, const cocos2d::Vec2 & speed, float attack);

	void update(float dt) override;

private:
	CC_SYNTHESIZE(cocos2d::Vec2, _start_point, StartPoint);
	CC_SYNTHESIZE(cocos2d::Vec2, _speed, Speed);
	CC_SYNTHESIZE(float, _attack, Attack);
	CC_SYNTHESIZE(bool, _out_side, isOutSide);
};