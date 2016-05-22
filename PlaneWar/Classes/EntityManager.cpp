#include "EntityManager.h"
#include "Hero.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameLayer.h"
using namespace cocos2d;

EntityManager * EntityManager::createWithLayer(GameLayer * layer)
{
	EntityManager* ret = new EntityManager();
	if (ret && ret->initWithLayer(layer))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

void EntityManager::addHero()
{
	_hero = Hero::create();
	_hero->setPosition(100, 200);
	addChild(_hero);
}

void EntityManager::addNormalBullet(float dt)
{
	Vec2 hero_point = _hero->getPosition() + Vec2(0, _hero->getContentSize().height / 2);
	auto bullet = BulletNormal::create(hero_point);
	addChild(bullet);
	bullet->saveToVector(_bullet_vec);
}

void EntityManager::addSuperBullet(float dt)
{
	Vec2 hero_point = _hero->getPosition() + Vec2(0, _hero->getContentSize().height / 2);
	auto bullet = BulletSuper::create(hero_point);
	addChild(bullet);
	bullet->saveToVector(_bullet_vec);
}

void EntityManager::addEnemy(float dt)
{
	if (_total_points <= 1500)
	{
		refreshEnemyLow();
	}
	else if (_total_points > 1500 && _total_points <= 5500)
	{
		refreshEnemyMid();
	}
	else if (_total_points > 5500)
	{
		refreshEnemyHigh();
	}
}

void EntityManager::addGoods(float dt)
{
	auto goods = Goods::create();
	addChild(goods);
	goods->setPosition(goods->getStartPoint());

	_goods_vec.pushBack(goods);
}

void EntityManager::getGoodsBullet()
{
	this->unschedule(schedule_selector(EntityManager::addNormalBullet));
	this->schedule(schedule_selector(EntityManager::addSuperBullet), 0.3f);
	CallFunc* call = CallFunc::create([this] (){
		this->unschedule(schedule_selector(EntityManager::addSuperBullet));
		this->schedule(schedule_selector(EntityManager::addNormalBullet), 0.5f);
	});
	runAction(Sequence::create(DelayTime::create(3.0f), call, nullptr));
}

void EntityManager::getGoodsBomb()
{
	_layer->addAvailableBomb();
}

void EntityManager::useGoodsBomb()
{
	for (auto i = _enemy_vec.begin(); i != _enemy_vec.end();)
	{
		_total_points += (*i)->getPoints();
		_layer->refreshScore(_total_points);
		(*i)->enemyDead();
		i = _enemy_vec.erase(i);
	}
}

EntityManager::EntityManager()
	: _hero(nullptr)
	, _layer(nullptr)
	, _total_points(0)
{

}

EntityManager::~EntityManager()
{
}

bool EntityManager::initWithLayer(GameLayer* layer)
{
	_layer = layer;

	addHero();
	schedule(schedule_selector(EntityManager::addNormalBullet), 0.5f);
    schedule(schedule_selector(EntityManager::addEnemy), 1.0f);
	schedule(schedule_selector(EntityManager::addGoods), 5.0f);

    schedule(schedule_selector(EntityManager::crashDetectionInEnemy));
	schedule(schedule_selector(EntityManager::crashDetectionInGoods));

	return true;
}

void EntityManager::refreshEnemyLow()
{
	EnemySmall* enemy = EnemySmall::createWithLevel(1);
	addChild(enemy);
	enemy->setPosition(enemy->getStartPoint());

	_enemy_vec.pushBack(enemy);
}

void EntityManager::refreshEnemyMid()
{
	int ran = RandomHelper::random_int(1, 100);
	if (ran <= 60)
	{
		EnemySmall* enemy = EnemySmall::createWithLevel(2);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
	else if (ran > 60 && ran <= 90)
	{
		EnemyMiddle* enemy = EnemyMiddle::createWithLevel(2);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
	else if (ran > 90)
	{
		EnemyLarge* enemy = EnemyLarge::createWithLevel(2);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
}

void EntityManager::refreshEnemyHigh()
{
	int ran = RandomHelper::random_int(1, 100);
	if (ran <= 35)
	{
		EnemySmall* enemy = EnemySmall::createWithLevel(3);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
	else if (ran > 35 && ran <= 75)
	{
		EnemyMiddle* enemy = EnemyMiddle::createWithLevel(3);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
	else if (ran > 75)
	{
		EnemyLarge* enemy = EnemyLarge::createWithLevel(3);
		addChild(enemy);
		enemy->setPosition(enemy->getStartPoint());

		_enemy_vec.pushBack(enemy);
	}
}

void EntityManager::crashDetectionInEnemy(float dt)
{	
	for (size_t i = _enemy_vec.size(); i > 0; --i)
	{
		auto enemy = _enemy_vec.at(i - 1);
		if (enemy->getBoundingBox().intersectsRect(_hero->getBoundingBox()))
		{
			_hero->runDeadAnimation(0.25f);
			this->unscheduleAllSelectors();
			CallFunc* call = CallFunc::create([this](){
				_layer->gameOver();
			});
			runAction(Sequence::create(DelayTime::create(1.5f), call, nullptr));
		}
		if (enemy->isOutSide())
		{
			_enemy_vec.eraseObject(enemy);
		}
		else
		{
			for (size_t j = _bullet_vec.size(); j > 0; --j)
			{
				auto bullet = _bullet_vec.at(j - 1);
				if (bullet->isOutSide())
				{
					_bullet_vec.eraseObject(bullet);
				}
				else
				{
					if (enemy->getBoundingBox().containsPoint(bullet->getPosition()))
					{
						if (enemy->getHp() > bullet->getAttack())
						{
							enemy->enemyHit();
							enemy->setHp(enemy->getHp() - bullet->getAttack());
							bullet->removeFromParent();
							_bullet_vec.eraseObject(bullet);
						}
						else
						{
							_total_points += enemy->getPoints();
							_layer->refreshScore(_total_points);
							enemy->enemyDead();
							_enemy_vec.eraseObject(enemy);
							bullet->removeFromParent();
							_bullet_vec.eraseObject(bullet);
							break;
						}
					}
				}
			}
		}
	}
}

void EntityManager::crashDetectionInGoods(float dt)
{
	if (!_goods_vec.empty())
	{
		for (size_t i = _goods_vec.size(); i > 0; --i)
		{
			auto goods = _goods_vec.at(i - 1);
			if (goods->getOutSide())
			{
				_goods_vec.eraseObject(goods);
			}
			else
			{
				if (_hero->getBoundingBox().intersectsRect(goods->getBoundingBox()))
				{
					if (goods->getGoodsType() == Goods::enBullet)
					{
						getGoodsBullet();
					}
					else if(goods->getGoodsType() == Goods::enBomb)
					{
						getGoodsBomb();
					}
					goods->removeFromParent();
					_goods_vec.eraseObject(goods);
				}
			}
		}
	}
}