#pragma once
#include "cocos2d.h"

class EntityManager;
class GameLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static GameLayer* create();

	void refreshScore(float new_score);
	void addAvailableBomb();
	void touchPauseButton();

	void gameOver();

private:
	GameLayer();
	virtual ~GameLayer();

	bool init() override;

	void createBackGround();
	void createManager();
	void createScoreUI();
	void createGoodsUI();
	void createPauseButton();

	void update(float dt) override;

	bool touchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void touchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	cocos2d::Size _visible_size;
	cocos2d::Sprite* _bg_sp1;
	cocos2d::Sprite* _bg_sp2;
	cocos2d::Label* _score_label;
	cocos2d::MenuItemSprite* _pause_button;
	cocos2d::Sprite* _bomb_sp;
	bool _avaiable_bomb;
	EntityManager* _manager;
	bool _game_pause;
	cocos2d::LayerColor* _color_layer;
};