#pragma once
#include "cocos2d.h"
#include "BulletBase.h"
#include "EnemyBase.h"
#include "Goods.h"

class Hero;
class GameLayer;
class EntityManager :public cocos2d::Node
{
public:
	static EntityManager* createWithLayer(GameLayer* layer);

	void addHero();
	void addNormalBullet(float dt);
	void addSuperBullet(float dt);
	void addEnemy(float dt);
	void addGoods(float dt);

	void getGoodsBullet();
	void getGoodsBomb();

	void useGoodsBomb();

	float getTotalPoints() const { return _total_points; }

private:
	EntityManager();
	~EntityManager();

	bool initWithLayer(GameLayer* layer);

	void refreshEnemyLow();
	void refreshEnemyMid();
	void refreshEnemyHigh();

	void crashDetectionInEnemy(float dt);
	void crashDetectionInGoods(float dt);

private:
	Hero* _hero;
	GameLayer* _layer;
	float _total_points;
	cocos2d::Vector<BulletBase*> _bullet_vec;
	cocos2d::Vector<EnemyBase*> _enemy_vec;
	cocos2d::Vector<Goods*> _goods_vec;
};
