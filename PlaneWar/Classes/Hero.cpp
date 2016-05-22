#include "Hero.h"
#include "AnimateUtil.h"
using namespace cocos2d;

Hero * Hero::create()
{
	Hero* ret = new Hero();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void Hero::runFlyAnimation(float delay)
{
	auto animation = AnimateUtil::createWithEmptySpriteFrame(delay, -1);
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero2.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero.png"));
	Animate* animate = Animate::create(animation);
	runAction(animate);
}

void Hero::runDeadAnimation(float delay)
{
	_dead = true;
	auto animate = AnimateUtil::createWithSpriteFrameTypeAndNum("hero_blowup_n", 4, delay, 1);
	CallFunc* remove = CallFunc::create([this] {
		removeFromParent();
	});
	runAction(Sequence::create(animate, remove, nullptr));
}

Hero::Hero()
	: _dead(false)
{
	EventListenerTouchOneByOne* hero_listener = EventListenerTouchOneByOne::create();
	hero_listener->setSwallowTouches(true);
	hero_listener->onTouchBegan = CC_CALLBACK_2(Hero::touchBegan, this);
	hero_listener->onTouchMoved = CC_CALLBACK_2(Hero::touchMoved, this);
	hero_listener->onTouchEnded = CC_CALLBACK_2(Hero::touchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(hero_listener, this);
}

Hero::~Hero()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool Hero::init()
{
	if (!initWithSpriteFrameName("hero.png"))
	{
		return false;
	}
	setScale(0.9f);

	runFlyAnimation(0.3f);

	return true;
}

bool Hero::touchBegan(Touch * touch, Event * event)
{
	if (!_dead)
	{
		Vec2 touch_pos = touch->getLocation();
		Size hero_size = getContentSize();
		Rect hero_rect(-hero_size.width * 0.1f, -hero_size.height * 0.1f, hero_size.width * 1.2f, hero_size.height * 1.2f);
		if (hero_rect.containsPoint(this->convertToNodeSpace(touch_pos)))
		{
			return true;
		}
	}
	return false;
}

void Hero::touchMoved(Touch * touch, Event * event)
{
	Size hero_size = getContentSize() * 0.8f;
	Size win_size = Director::getInstance()->getVisibleSize();
	Rect range_of_activity(hero_size.width / 2, hero_size.height / 2, win_size.width - hero_size.width, win_size.height - hero_size.height);
	Vec2 dest = getPosition() + touch->getDelta();
	if (range_of_activity.containsPoint(dest))
	{
		setPosition(dest);
	}
}

void Hero::touchEnded(Touch* touch, Event* event)
{
	log("hero_touch_end!");
}