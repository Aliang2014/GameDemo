#pragma once
#include "cocos2d.h"

class Hero :public cocos2d::Sprite
{
public:
	static Hero* create();

	void runFlyAnimation(float delay);
	void runDeadAnimation(float delay);

private:
	Hero();
	~Hero();

private:
	bool init() override;

	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	CC_SYNTHESIZE(bool, _dead, DeadState);
};
