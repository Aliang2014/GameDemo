#pragma once
#include "cocos2d.h"
#include "MapHelper.h"

class CatSprite;
class GameLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	static GameLayer* createWithMapName(const char* name);

	void catMeetDog(const cocos2d::Point& tileCoord);
	void catMeetExit(const cocos2d::Point& tileCoord);
	void catMeetBone(const cocos2d::Point& tileCoord);
	void catMeetNothing(const cocos2d::Point& tileCoord);

	void winGame();
	void loseGame();
	void showNumBones(int numBones);

	void setViewPointOfCenter(const cocos2d::Point& catPos);

public:
	GameLayer();
	virtual ~GameLayer();

private:
	virtual bool initWithMapName(const char* name);

	void loadGameResource();
	void createMap(const char* name);
	void createBonesUI();
	void createCatSprite();

	// touch
	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void showRestartMenu();
	void endScene();

	virtual void update(float dt);

private:
	CC_SYNTHESIZE(cocos2d::Size, _visibleSize, VisibleSize);
	CC_SYNTHESIZE_READONLY(MapHelper*, _mapHelper, MapHelper);
	CC_SYNTHESIZE_READONLY(CatSprite*, _cat, CatSprite);

	bool _gameOver;
	bool _won;
	int _bonesCount;
	cocos2d::Label* _bonesUI;

};