#include "Enemy.h"
#include "AnimateUtil.h"
using namespace cocos2d;

/*
****************************************************
**  EnemyLarge
****************************************************
*/
EnemyLarge * EnemyLarge::createWithLevel(int level)
{
	EnemyLarge* enemy = new EnemyLarge();
	if (enemy && enemy->initWithLevel(level))
	{
		enemy->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(enemy);
	}
	return enemy;
}

void EnemyLarge::enemyDead()
{
	setDeadState(true);

	Animate* animate = AnimateUtil::createWithSpriteFrameTypeAndNum("enemy3_down", 6, 0.15f, 1);
	CallFunc* remove = CallFunc::create([this]() {
		this->removeFromParent();
	});
	runAction(Sequence::create(animate, remove, nullptr));
}

void EnemyLarge::enemyHit()
{
	CallFunc* call_1 = CallFunc::create([this]() {
		initWithSpriteFrameName("enemy3_hit.png");
	});
	DelayTime* delay = DelayTime::create(0.2f);
	CallFunc* call_2 = CallFunc::create([this]() {
		initWithSpriteFrameName("enemy3_n1.png");
	});
	Sequence* ac = Sequence::create(call_1, delay, call_2, nullptr);
	ac->setTag(10000);
	stopActionByTag(10000);
	runAction(ac);
}

EnemyLarge::EnemyLarge()
	: EnemyBase(7.0f, 100.0f, 1000.0f)
{

}

EnemyLarge::~EnemyLarge()
{

}

bool EnemyLarge::initWithLevel(int level)
{
	if (!initWithSpriteFrameName("enemy3_n1.png"))
		return false;

	setScale(0.9f);
	setType(enLarge);
	// reset speed accodring with level
	if (level == 2)
	{
		setSpeedY(getSpeedY() + 50);
	}
	else if(level == 3)
	{
		setSpeedY(getSpeedY() + 100);
	}

	// set start_point
	int range_high = Director::getInstance()->getVisibleSize().width - getContentSize().width;
	int range_low = getContentSize().width / 2;
	float x = RandomHelper::random_int(range_low, range_high);
	setStartPoint(x, 750 + getContentSize().height / 2);

	scheduleUpdate();

	return true;
}

void EnemyLarge::update(float dt)
{
	if (!isDead())
	{
		Vec2 next_point = this->getPosition() - Vec2(0, this->getSpeedY()*dt);
		if (next_point.y < -this->getContentSize().height / 2)
		{
			setOutSide(true);
			removeFromParent();
		}
		else
		{
			this->setPosition(next_point);
		}
	}
}


/*
****************************************************
**  EnemyMiddle
****************************************************
*/
EnemyMiddle* EnemyMiddle::createWithLevel(int level)
{
	EnemyMiddle* enemy = new EnemyMiddle();
	if (enemy && enemy->initWithLevel(level))
	{
		enemy->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(enemy);
	}
	return enemy;
}

void EnemyMiddle::enemyDead()
{
	setDeadState(true);

	Animate* animate = AnimateUtil::createWithSpriteFrameTypeAndNum("enemy2_down", 4, 0.15f, 1);
	CallFunc* remove = CallFunc::create([this]() {
		this->removeFromParent();
	});
	runAction(Sequence::create(animate, remove, nullptr));
}

void EnemyMiddle::enemyHit()
{
	CallFunc* call_1 = CallFunc::create([this]() {
		initWithSpriteFrameName("enemy2_hit.png");
	});
	DelayTime* delay = DelayTime::create(0.2f);
	CallFunc* call_2 = CallFunc::create([this]() {
		initWithSpriteFrameName("enemy2.png");
	});
	Sequence* ac = Sequence::create(call_1, delay, call_2, nullptr);
	ac->setTag(10000);
	stopActionByTag(10000);
	runAction(ac);
}

EnemyMiddle::EnemyMiddle()
	: EnemyBase(3.0f, 300.0f, 400.0f)
{

}

EnemyMiddle::~EnemyMiddle()
{

}

bool EnemyMiddle::initWithLevel(int level)
{
	if (!initWithSpriteFrameName("enemy2.png"))
		return false;

	setScale(0.9f);
	setType(enMiddle);

	if (level == 2)
	{
		setSpeedY(getSpeedY() + 50);
	}
	else if (level == 3)
	{
		setSpeedY(getSpeedY() + 100);
	}

	// set start_point
	int range_high = Director::getInstance()->getVisibleSize().width - getContentSize().width;
	int range_low = getContentSize().width / 2;
	float x = RandomHelper::random_int(range_low, range_high);
	setStartPoint(x, 750 + getContentSize().height / 2);

	scheduleUpdate();

	return true;
}

void EnemyMiddle::update(float dt)
{
	if (!isDead())
	{
		Vec2 next_point = this->getPosition() - Vec2(0, this->getSpeedY()*dt);
		if (next_point.y < -this->getContentSize().height / 2)
		{
			setOutSide(true);
			removeFromParent();
		}
		else
		{
			this->setPosition(next_point);
		}
	}
}


/*
****************************************************
**  EnemySmall
****************************************************
*/
EnemySmall * EnemySmall::createWithLevel(int level)
{
	EnemySmall* enemy = new EnemySmall();
	if (enemy && enemy->initWithLevel(level))
	{
		enemy->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(enemy);
	}
	return enemy;
}

void EnemySmall::enemyDead()
{
	setDeadState(true);

	Animate* animate = AnimateUtil::createWithSpriteFrameTypeAndNum("enemy1_down", 4, 0.15f, 1);
	CallFunc* remove = CallFunc::create([this]() {
		removeFromParent();
	});
	runAction(Sequence::create(animate, remove, nullptr));
}

void EnemySmall::enemyHit()
{
	// none
	;
}

EnemySmall::EnemySmall()
	: EnemyBase(1.0f, 400.0f, 100.0f)
{

}

EnemySmall::~EnemySmall()
{

}

bool EnemySmall::initWithLevel(int level)
{
	if (!initWithSpriteFrameName("enemy1.png"))
		return false;

	setType(enSmall);

	if (level == 2)
	{
		setSpeedY(getSpeedY() + 50);
	}
	else if (level == 3)
	{
		setSpeedY(getSpeedY() + 100);
	}

	// set start_point
	int range_high = Director::getInstance()->getVisibleSize().width - getContentSize().width;
	int range_low = getContentSize().width / 2;
	float x = RandomHelper::random_int(range_low, range_high);
	setStartPoint(x, 750 + getContentSize().height / 2);

	scheduleUpdate();

	return true;
}

void EnemySmall::update(float dt)
{
	if (!isDead())
	{
		Vec2 next_point = this->getPosition() - Vec2(0, this->getSpeedY()*dt);
		if (next_point.y < -this->getContentSize().height / 2)
		{
			setOutSide(true);
			removeFromParent();
		}
		else
		{
			this->setPosition(next_point);
		}
	}
}
