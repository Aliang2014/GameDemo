#pragma once
#include "EnemyBase.h"

/*
****************************************************
**  EnemyLarge
****************************************************
*/
class EnemyLarge :public EnemyBase
{
public:
	static EnemyLarge* createWithLevel(int level);

	virtual void enemyDead();
	virtual void enemyHit();

private:
	EnemyLarge();
	virtual ~EnemyLarge();

	bool initWithLevel(int level);

	void update(float dt) override;
};


/*
****************************************************
**  EnemyMiddle
****************************************************
*/
class EnemyMiddle :public EnemyBase
{
public:
	static EnemyMiddle* createWithLevel(int level);

	virtual void enemyDead();
	virtual void enemyHit();

private:
	EnemyMiddle();
	virtual ~EnemyMiddle();

	bool initWithLevel(int level);

	void update(float dt) override;
};


/*
****************************************************
**  EnemySmall
****************************************************
*/
class EnemySmall :public EnemyBase
{
public:
	static EnemySmall* createWithLevel(int level);

	virtual void enemyDead();
	virtual void enemyHit();

private:
	EnemySmall();
	virtual ~EnemySmall();

	bool initWithLevel(int level);

	void update(float dt) override;
};
