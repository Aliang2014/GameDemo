#include "AnimateUtil.h"

Animation * AnimateUtil::createWithEmptySpriteFrame(float delay, unsigned int loops)
{
	Animation*  animation = Animation::create();
	animation->setDelayPerUnit(delay);
	animation->setLoops(loops);
	return animation;
}

Animate * AnimateUtil::createWithSpriteFrameTypeAndNum(const char * name, int num, float delay, unsigned int loops)
{
	SpriteFrame* frame = nullptr;
	Animation* animation = Animation::create();
	for (int i = 1; i <= num; ++i)
	{
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("%s%d.png", name, i));
		if (frame != nullptr)
		{
			animation->addSpriteFrame(frame);
		}
		else
		{
			break;
		}
	}
	animation->setDelayPerUnit(delay);
	animation->setLoops(loops);
	return Animate::create(animation);
}
