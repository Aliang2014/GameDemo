#pragma once
#include "cocos2d.h"
USING_NS_CC;

class AnimateUtil
{
public:
	static Animation* createWithEmptySpriteFrame(float delay, unsigned int loops);

	static Animate* createWithSpriteFrameTypeAndNum(const char* name, int num, float delay, unsigned int loops);
};