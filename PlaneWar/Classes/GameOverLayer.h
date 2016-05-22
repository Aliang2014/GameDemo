#pragma once
#include "cocos2d.h"

class GameOverLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(float points);
	static GameOverLayer* createWithPoints(float points);

	void touchButtonRetry(cocos2d::Ref*);
	void touchButtonExit(cocos2d::Ref*);

private:
	bool initWithPoints(float points);

	void createBackGround();
	void createMenuUI();
	void createPointsUI(float points);
};