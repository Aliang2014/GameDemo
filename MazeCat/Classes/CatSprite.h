#pragma once
#include "cocos2d.h"
#include "PathMoveController.h"

class GameLayer;
class CatSprite :public cocos2d::Sprite, public MoveControllerListener
{
public:
	static CatSprite* createWithLayer(GameLayer* layer);

	void towardToDestination(const cocos2d::Point& origin, const cocos2d::Point& dest);

	cocos2d::Point getCurTileCoord() const;
	
	// 监控事件
	virtual void onStartMoving();
	virtual void onArrivedTiledCenter();
	virtual void onChangeDirection(PathDirection dir);
	virtual void onArrived();

public:
	CatSprite(); 
	virtual ~CatSprite();

private:
	bool initWithLayer(GameLayer* layer);

	void createPathMoveController();

	cocos2d::Animation* createMoveAnimation(const char* type);
	void runMoveAnimate(cocos2d::Animation* animtion);

	virtual void update(float dt);

private:
	GameLayer* _layer;
	cocos2d::Animation* _moveUpAnimation;
	cocos2d::Animation* _moveDownAnimation;
	cocos2d::Animation* _moveLeftAnimation;
	cocos2d::Animation* _moveRightAnimation;
	cocos2d::Animation* _curAnimation;
	cocos2d::Animation* _nextAnimation;
	cocos2d::Action* _curAnimate;
	
	PathMoveController* _pathMoveController;

	bool _enableMoveAnimate;

};